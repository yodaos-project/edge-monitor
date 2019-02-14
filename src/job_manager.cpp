//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#include "job_manager.h"
#include "options.h"
#include "job_runner.h"
#include "WebSocketClient.h"
#include "MessageCommon.h"
#include "device_info.h"
#include <rapidjson/istreamwrapper.h>

YODA_NS_BEGIN

JobManager::JobManager() :
  _runners(),
  _ws(nullptr),
  _task(nullptr),
  _taskTimer(nullptr),
  _disableUpload(false) {

}

int JobManager::initWithWS(WebSocketClient *ws) {
  _ws = ws;
  ws->setRecvCallback(std::bind(&JobManager::onWSMessage, this, _1));
  ws->setEventCallback(std::bind(&JobManager::onWSEvent, this, _1));
  _disableUpload = Options::get<uint32_t>("disableUpload", 0) != 0;
  std::string confpath = Options::get<std::string>("conf", "");
  if (confpath.empty()) {
    return 0;
  }
  std::ifstream ifs(confpath);
  YODA_SIXSIX_FASSERT(ifs.is_open(),
                      "cannot load conf from %s",
                      confpath.c_str());
  rapidjson::IStreamWrapper ifsWrapper(ifs);
  rapidjson::Document doc;
  doc.ParseStream(ifsWrapper);
  YODA_SIXSIX_FASSERT(!doc.HasParseError(),
                      "conf parse error %s",
                      confpath.c_str());
  if (!doc.HasMember("task")) {
    return 0;
  }
  auto &obj = doc["task"];
  std::shared_ptr<yoda::TaskInfo> task(new yoda::TaskInfo{0});
  task->id = obj["id"].GetInt();
  task->type = std::make_shared<std::string>(obj["type"].GetString());
  task->shellId = obj["shellId"].GetUint();
  task->shell = std::make_shared<std::string>(obj["shell"].GetString());
  task->shellType = std::make_shared<std::string>(obj["shellType"].GetString());
  task->timestampMs = obj["timestamp"].GetInt64();
  if (task->timestampMs == 0) {
    task->timeoutMs = 86400 * 1000;
    task->timestampMs = Util::getTimeMS() + task->timeoutMs;
  }
  this->startNewTask(task);
  return 0;
}

void JobManager::addRunnerWithConf(const std::shared_ptr<JobConf> &conf,
                                   bool autoRun) {
  YODA_SIXSIX_FLOG("add job type %d with conf", (int32_t) conf->type);
  auto callback = std::bind(&JobManager::onRunnerStop, this, _1);
  std::shared_ptr<JobRunner> runner(new JobRunner(this));
  runner->setJobCallback(callback);
  _runners.push_back(runner);
  YODA_SIXSIX_FLOG("runner count %d", (uint32_t) _runners.size());
  runner->initWithConf(conf);
  if (autoRun) {
    runner->run();
  }
}

void JobManager::onRunnerStop(JobRunner *runner) {
  auto name = runner->getJobName().c_str();
  auto isStopped = runner->getState() == JobState::STOP;
  YODA_SIXSIX_FASSERT(isStopped, "runner %s is not stopped ", name);
  YODA_SIXSIX_FLOG("removing runner %s", name);
  for (auto ite = _runners.begin(); ite != _runners.end(); ++ite) {
    if ((*ite).get() == runner) {
      _runners.erase(ite);
      break;
    }
  }
  YODA_SIXSIX_FLOG("runner left %zu", _runners.size());
  if (_runners.empty()) {
    YODA_SIXSIX_SLOG("stopping task timer");
    uv_timer_stop(_taskTimer);
    UV_CLOSE_HANDLE(_taskTimer, JobManager, onUVHandleClosed);

    char msg[256] = {0};
    sprintf(msg, "end task with code: %d", _task->errorCode);
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
}

void JobManager::endTask(TaskErrorCodes errorCode) {
  if (!_task) {
    YODA_SIXSIX_SERROR("not task, ignored");
    return;
  }
  if (_task->status != TaskStatus::RUNNING) {
    YODA_SIXSIX_SERROR("task is not running, ignored");
    return;
  }

  _task->errorCode = errorCode;
  if (errorCode != TaskErrorCodes::NO_ERROR) {
    _task->status = TaskStatus::FAILED;
  } else {
    _task->status = TaskStatus::SUCCEED;
  }
  YODA_SIXSIX_FLOG("stopping all runners %d", (int32_t) _runners.size());
  for (auto &runner : _runners) {
    runner->stop();
  }
}

void JobManager::onWSMessage(std::shared_ptr<Caps> &caps) {
  rokid::MessageType type = rokid::get_msg_type(caps);
  switch (type) {
    case rokid::MessageType::TYPE_TASKCOMMAND:
      this->onTaskCommand(caps);
      break;
    default:
      YODA_SIXSIX_FERROR("unknown ws message type %d, ignored", type);
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
      YODA_SIXSIX_FERROR("unhandled ws event code %d, ignored", code);
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
      sprintf(msg, "end task with code: %d", TaskErrorCodes::NO_RESOURCE);
      YODA_SIXSIX_FERROR("multi task %d %d", task->id, _task->id);
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
      YODA_SIXSIX_FERROR("task %d is running, ignore start", task->id);
    }
    return;
  }
  _task = task;

  this->manuallyStartJobs(task->shell, task->shellId);

  _taskTimer = (uv_timer_t *) malloc(sizeof(uv_timer_t));
  UV_CB_WRAP1(_taskTimer, cb2, JobManager, onTaskTimeout, uv_timer_t);
  uv_timer_init(uv_default_loop(), _taskTimer);
  uv_timer_start(_taskTimer, cb2, (uint64_t) _task->timeoutMs, 0);
}

void JobManager::onTaskTimeout(uv_timer_t *) {
  YODA_SIXSIX_SLOG("task time timeout");
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
  shellConf->loopCount = 0;
  shellConf->timeout = 0;
  shellConf->interval = 0;
  this->addRunnerWithConf(shellConf, true);

  std::shared_ptr<JobConf> topConf(new JobConf);
  topConf->task = _task;
  topConf->type = JobType::COLLECT_TOP;
  topConf->enable = true;
  topConf->isRepeat = true;
  topConf->loopCount = 0;
  topConf->timeout = 500;
  topConf->interval = 1000;
  this->addRunnerWithConf(topConf, true);

  std::shared_ptr<JobConf> smapConf(new JobConf);
  smapConf->task = _task;
  smapConf->type = JobType::COLLECT_SMAP;
  smapConf->enable = true;
  smapConf->isRepeat = true;
  smapConf->loopCount = 0;
  smapConf->timeout = 1000;
  smapConf->interval = Options::get<uint64_t>("smapInterval", 300 * 1000);
  YODA_SIXSIX_FLOG("smap interval %" PRIu64 "ms", smapConf->interval);
  this->addRunnerWithConf(smapConf, true);

  std::shared_ptr<JobConf> crashReporterConf(new JobConf);
  crashReporterConf->task = _task;
  crashReporterConf->type = JobType::CRASH_REPORTER;
  crashReporterConf->enable = true;
  crashReporterConf->isRepeat = true;
  crashReporterConf->loopCount = 0;
  crashReporterConf->timeout = 1000;
  crashReporterConf->interval = 60 * 1000;
  this->addRunnerWithConf(crashReporterConf, true);
}

void JobManager::sendCollectData(std::shared_ptr<Caps> &caps, const char *hint){
  if (!_disableUpload) {
    this->sendMsg(caps, hint);
  }
}

void JobManager::sendMsg(std::shared_ptr<Caps> &caps, const char *hint) {
  if (_ws) {
    _ws->sendMsg(caps, [hint](SendResult sr, void *) {
      YODA_SIXSIX_FLOG("send ws %s result %u", hint, sr);
    });
  } else {
    YODA_SIXSIX_FERROR("ws is null, drop %s data", hint);
  }
}

void JobManager::onUVHandleClosed(uv_handle_t *handle) {
  if ((uv_handle_t *) _taskTimer == handle) {
    YODA_SIXSIX_SAFE_FREE(_taskTimer);
  } else {
    YODA_SIXSIX_SLOG("manager receive unknown handle close, free it");
    YODA_SIXSIX_SAFE_FREE(handle);
  }
}

YODA_NS_END
