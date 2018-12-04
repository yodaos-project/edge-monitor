//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#include "./job_manager.h"
#include "./conf.h"
#include "./job_runner.h"
#include "./websocket/WebSocketClient.h"
#include "./CapsMsg.h"
#include "./device_info.h"
#include <rapidjson/istreamwrapper.h>

YODA_NS_BEGIN

#define COLLECT_DATA_SEND_INTERVAL 5000

JobManager::JobManager() :
  _collectData(),
  _runners(),
  _ws(nullptr),
  _task(nullptr),
  _collectTimer(nullptr),
  _taskTimer(nullptr),
  _disableUpload(false) {

}

int JobManager::setWs(WebSocketClient *ws) {
  YODA_SIXSIX_SLOG("JobManager inited");
  ws->setRecvCallback(std::bind(&JobManager::onWSMessage, this, _1));
  ws->setEventCallback(std::bind(&JobManager::onWSEvent, this, _1));
  _ws = ws;
  _disableUpload = !!Conf::get<uint32_t>("disableUpload", 0);
  return 0;
}

void JobManager::startTaskFromCmdConf() {
  auto taskJsonPath = Conf::get<std::string>("taskJson", "");
  if (taskJsonPath.empty()) {
    return;
  }
  std::ifstream ifs(taskJsonPath);
  rapidjson::IStreamWrapper ifsWrapper(ifs);
  if (!ifs.is_open()) {
    YODA_SIXSIX_FLOG("cannot load conf from %s", taskJsonPath.c_str());
    return;
  }
  rapidjson::Document doc;
  doc.ParseStream(ifsWrapper);
  if (doc.HasParseError()) {
    YODA_SISIX_FERROR("load conf error from %s", taskJsonPath.c_str());
    return;
  }
  std::shared_ptr<yoda::TaskInfo> task(new yoda::TaskInfo{0});
  task->id = doc["id"].GetInt();
  task->type = std::make_shared<std::string>(doc["type"].GetString());
  task->shellId = doc["shellId"].GetUint();
  task->shell = std::make_shared<std::string>(doc["shell"].GetString());
  task->shellType = std::make_shared<std::string>(doc["shellType"].GetString());
  task->timestampMs = doc["timestamp"].GetInt64();
  if (task->timestampMs == 0) {
    task->timeoutMs = 86400 * 1000;
    task->timestampMs = Util::getTimeMS() + task->timeoutMs;
  }
  this->startNewTask(task);
}

void JobManager::addRunnerWithConf(const std::shared_ptr<JobConf> &conf,
                                   bool autoRun) {
  YODA_SIXSIX_FLOG("add job type %d with conf", (int32_t) conf->type);
  auto callback = std::bind(&JobManager::onRunnerResult, this, _1);
  std::shared_ptr<JobRunner> runner(new JobRunner(this));
  runner->setJobCallback(callback);
  _runners.push_back(runner);
  YODA_SIXSIX_FLOG("runner count %d", (uint32_t) _runners.size());
  runner->initWithConf(conf);
  if (autoRun) {
    runner->run();
  }
}

void JobManager::onRunnerResult(JobRunner *runner) {
  auto state = runner->getState();
  YODA_SIXSIX_FLOG("runner %s result %d", runner->getJobName().c_str(), state);
  if (state == JobState::STOP) {
    auto &name = runner->getJobName();
    YODA_SIXSIX_FLOG("removing runner %s, state %d", name.c_str(), state);
    for (auto ite = _runners.begin(); ite != _runners.end(); ++ite) {
      if ((*ite).get() == runner) {
        _runners.erase(ite);
        break;
      }
    }
    YODA_SIXSIX_FLOG("runner left %ld", _runners.size());
    if (_runners.empty()) {
      this->endTask(TaskErrorCodes::NO_ERROR);
    }
  }
}

void JobManager::endTask(TaskErrorCodes errorCode) {
  char msg[256] = {0};
  sprintf(msg, "end task with code: %d", errorCode);
  YODA_SIXSIX_SLOG(msg);
  if (_taskTimer) {
    YODA_SIXSIX_SLOG("stopping task timer");
    uv_timer_stop(_taskTimer);
    YODA_SIXSIX_SAFE_FREE(_taskTimer);
  }
  if (_collectTimer) {
    YODA_SIXSIX_SLOG("stopping collect timer");
    uv_timer_stop(_collectTimer);
    YODA_SIXSIX_SAFE_FREE(_collectTimer);
  }
  if (!_task) {
    YODA_SIXSIX_SERROR("stop not running task, ignored");
    return;
  }
  if (errorCode != TaskErrorCodes::NO_ERROR) {
    _task->status = TaskStatus::FAILED;
  } else {
    _task->status = TaskStatus::SUCCEED;
  }
  YODA_SIXSIX_FLOG("stopping all runners %d", (int32_t) _runners.size());
  for (auto &runner : _runners) {
    int32_t r = runner->stop();
    YODA_SIXSIX_FLOG("runner %d stop return %d", runner->getType(), r);
  }
  _runners.clear();
  auto taskStatus = rokid::TaskStatus::create();
  taskStatus->setTaskId(_task->id);
  taskStatus->setShellId(_task->shellId);
  taskStatus->setTimestamp(_task->timestampMs);
  taskStatus->setStatus((int32_t) _task->status);
  taskStatus->setMessage(std::make_shared<std::string>(msg));

  auto caps = Caps::new_instance();
  taskStatus->serialize(caps);
  this->sendMsg(caps, "end task");
  _task.reset();
}

void JobManager::onWSMessage(std::shared_ptr<Caps> &caps) {
  rokid::MessageType type = rokid::get_msg_type(caps);
  switch (type) {
    case rokid::MessageType::TYPE_TASKCOMMAND:
      this->onTaskCommand(caps);
      break;
    default:
      YODA_SISIX_FERROR("unknown ws message type %d, ignored", type);
      break;
  }
}

void JobManager::onWSEvent(EventCode code) {
  switch (code) {
    case EventCode::DisConnected:
      this->onWSDisconnected();
      break;
    case EventCode::Connected:
      this->onWSConnected();
      break;
    default:
      YODA_SISIX_FERROR("unhandled ws event code %d, ignored", code);
      break;
  }
}

void JobManager::onWSConnected() {
  auto deviceStatus = rokid::DeviceStatus::create();
  deviceStatus->setTimestamp(Util::getTimeMS());
  deviceStatus->setSn(DeviceInfo::sn.c_str());
  deviceStatus->setVersion(DeviceInfo::imageVersion.c_str());
  DeviceStatus status = DeviceStatus::IDLE;
  int32_t shellId = 0;
  if (_task) {
    status = DeviceStatus::RUNNING;
    shellId = _task->shellId;
  }
  deviceStatus->setStatus((int32_t) status);
  deviceStatus->setShellId(shellId);
  YODA_SIXSIX_FLOG("ws connected, status %d, shell %d", status, shellId);
  std::shared_ptr<Caps> caps;
  deviceStatus->serialize(caps);
  this->sendMsg(caps, "upload device status");
}

void JobManager::onWSDisconnected() {
  YODA_SIXSIX_SERROR("ws disconnected");
}

void JobManager::onTaskCommand(std::shared_ptr<Caps> &caps) {
  std::shared_ptr<rokid::TaskCommand> command = rokid::TaskCommand::create();
  int r = command->deserializeForCapsObj(caps);
  if (r != CAPS_SUCCESS) {
    YODA_SIXSIX_SERROR("task command deserializeForCapsObj error");
    return;
  }
  auto type = command->getTaskType();
  if (*type == "CANCEL") {
    YODA_SIXSIX_SLOG("cancel task");
    this->endTask(TaskErrorCodes::NO_ERROR);
  } else if (*type == "START") {
    auto task = std::make_shared<TaskInfo>();
    task->status = TaskStatus::RUNNING;
    task->id = command->getTaskId();
    task->type = type;
    task->shellId = command->getShellId();
    task->shell = command->getShellContent();
    task->shellType = command->getShellType();
    task->timestampMs = command->getTimestamp();
    task->timeoutMs = task->timestampMs - Util::getTimeMS();
    this->startNewTask(task);
  }
}

void JobManager::startNewTask(const std::shared_ptr<TaskInfo> &task) {
  YODA_SIXSIX_FLOG("start new task with id %d", task->id);
  if (_task) {
    if (_task->id != task->id) {
      char msg[256] = {0};
      sprintf(msg, "multi task %d, %d", _task->id, task->id);
      YODA_SIXSIX_SLOG(msg);
      this->endTask(TaskErrorCodes::MULTI_TASK);
      auto taskStatus = rokid::TaskStatus::create();
      taskStatus->setTaskId(task->id);
      taskStatus->setShellId(task->shellId);
      taskStatus->setTimestamp(Util::getTimeMS());
      taskStatus->setStatus((int32_t) TaskStatus::FAILED);
      taskStatus->setMessage(std::make_shared<std::string>(msg));
      auto caps = Caps::new_instance();
      taskStatus->serialize(caps);
      this->sendMsg(caps, "task error: multi task.");
    } else {
      YODA_SISIX_FERROR("task %d is running, ignore start", task->id);
    }
    return;
  }
  _task = task;
  this->resetCollectData();

  this->manuallyStartJobs(task->shell, task->shellId);

  _collectTimer = (uv_timer_t *) malloc(sizeof(uv_timer_t));
  UV_MAKE_CB_WRAP1(_collectTimer, cb1, JobManager, onCollectData, uv_timer_t);
  uv_timer_init(uv_default_loop(), _collectTimer);
  uint64_t interval = COLLECT_DATA_SEND_INTERVAL;
  uv_timer_start(_collectTimer, cb1, interval, interval);

  _taskTimer = (uv_timer_t *) malloc(sizeof(uv_timer_t));
  UV_MAKE_CB_WRAP1(_taskTimer, cb2, JobManager, onTaskTimeout, uv_timer_t);
  uv_timer_init(uv_default_loop(), _taskTimer);
  uv_timer_start(_taskTimer, cb2, (uint64_t) task->timeoutMs, 0);
}

void JobManager::onCollectData(uv_timer_t *) {
  YODA_SIXSIX_SLOG("sending collect data");
  if (_ws) {
    if (_disableUpload) {
      YODA_SIXSIX_SLOG("collect data is disabled to upload");
    } else {
      std::shared_ptr<Caps> caps;
      _collectData->serialize(caps);
      this->sendMsg(caps, "collect data");
    }
  } else {
    YODA_SIXSIX_SERROR("ws is null");
  }
  this->resetCollectData();
}

void JobManager::resetCollectData() {
  _collectData.reset();
  auto sysCpuInfo = std::make_shared<rokid::SysCPUInfo>();
  sysCpuInfo->setTotal(std::make_shared<rokid::SysCPUCoreInfo>());
  sysCpuInfo->setCores(std::make_shared<std::vector<rokid::SysCPUCoreInfo>>());
  auto cpuInfos = std::make_shared<rokid::CPUInfos>();
  cpuInfos->setSysCpu(sysCpuInfo);
  cpuInfos->setProcCpuList(std::make_shared<std::vector<rokid::ProcCPUInfo>>());
  cpuInfos->setTimestamp((time(nullptr)));

  auto memInfos = std::make_shared<rokid::MemInfos>();
  memInfos->setSysMem(std::make_shared<rokid::SysMemInfo>());
  memInfos->setProcMemInfo(std::make_shared<std::vector<rokid::ProcMemInfo>>());
  memInfos->setTimestamp(time(nullptr));

  _collectData = rokid::CollectData::create();
  _collectData->setTimestamp(time(nullptr));
  _collectData->setCpu(cpuInfos);
  _collectData->setMem(memInfos);
}

void JobManager::onTaskTimeout(uv_timer_t *) {
  YODA_SIXSIX_SLOG("time timeout");
  this->endTask(TaskErrorCodes::NO_ERROR);
}

void JobManager::manuallyStartJobs(
  const std::shared_ptr<std::string> &shell, int32_t shellId) {
  YODA_SIXSIX_FLOG("start shell job with shell id %d", shellId);
  std::shared_ptr<JobConf> shellConf(new JobConf);
  shellConf->task = _task;
  shellConf->type = JobType::SPAWN_CHILD;
  shellConf->data = shell;
  shellConf->enable = true;
  shellConf->isRepeat = false;
  shellConf->timeout = 0;
  shellConf->interval = 0;
  this->addRunnerWithConf(shellConf, true);

  std::shared_ptr<JobConf> topConf(new JobConf);
  topConf->task = _task;
  topConf->type = JobType::COLLECT_TOP;
  topConf->enable = true;
  topConf->isRepeat = true;
  topConf->timeout = 1000;
  topConf->interval = 1000;
  this->addRunnerWithConf(topConf, true);

  std::shared_ptr<JobConf> smapConf(new JobConf);
  smapConf->task = _task;
  smapConf->type = JobType::COLLECT_SMAP;
  smapConf->enable = true;
  smapConf->isRepeat = true;
  smapConf->timeout = 5000;
  smapConf->interval = 5000;
  this->addRunnerWithConf(smapConf, true);
}

void JobManager::sendMsg(std::shared_ptr<Caps> &caps, const char *hint) {
  if (_ws) {
    _ws->sendMsg(caps, [hint](SendResult sr, void *) {
      YODA_SIXSIX_FLOG("send ws %s result %u", hint,  sr);
    });
  } else {
     YODA_SISIX_FERROR("ws is null, send msg error %s", hint);
  }
}

YODA_NS_END

