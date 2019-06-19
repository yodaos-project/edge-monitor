//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#include "job_manager.h"
#include "options.h"
#include "job_runner.h"
#include "WebSocketClient.h"
#include "MessageCommon.h"
#include "device_info.h"
#include "util.h"
#include <rapidjson/istreamwrapper.h>

YODA_NS_BEGIN

JobManager::JobManager() :
  _runners(),
  _taskRunner(nullptr),
  _pendingTaskCommand(nullptr),
  _ws(nullptr),
  _disableUpload(false),
  _wsConnected(false) {

}

void JobManager::setWsClient(WebSocketClient *ws) {
  _ws = ws;
  ws->setRecvCallback(std::bind(&JobManager::onWSMessage, this, _1));
  ws->setEventCallback(std::bind(&JobManager::onWSEvent, this, _1));
}

std::shared_ptr<JobRunner> JobManager::addRunnerWithConf(
  const std::shared_ptr<JobConf> &conf) {
  LOG_INFO("add job type %d with conf", (int32_t) conf->type);
  auto callback = std::bind(&JobManager::onRunnerStop, this, _1);
  std::shared_ptr<JobRunner> runner(new JobRunner(this));
  runner->setJobCallback(callback);
  LOG_INFO("runner count %d", (uint32_t) _runners.size());
  runner->initWithConf(conf);
  runner->run();
  return runner;
}

void JobManager::onRunnerStop(JobRunner *runner) {
  auto name = runner->getJobName().c_str();
  auto isStopped = runner->getState() != JobState::RUNNING;
  ASSERT(isStopped, "runner %s is not stopped ", name);
  LOG_INFO("removing runner %s", name);
  for (auto ite = _runners.begin(); ite != _runners.end(); ++ite) {
    if ((*ite).get() == runner) {
      _runners.erase(ite);
      break;
    }
  }
  LOG_INFO("runner left %zu", _runners.size());
  if (_taskRunner.get() == runner) {
    auto task = runner->getConf()->task;
    char msg[256] = {0};
    if (task->status == TaskStatus::RUNNING) {
      if (runner->getState() == JobState::FAILED) {
        task->status = TaskStatus::FAILED;
      } else {
        task->status = TaskStatus::SUCCEED;
      }
    }
    sprintf(msg, "end task with code: %d", task->status);
    LOG_INFO(msg); 
    auto taskStatus = rokid::TaskStatus::create();
    taskStatus->setTaskId(task->id);
    taskStatus->setShellId(task->shellId);
    taskStatus->setTimestamp(task->timestampMs);
    taskStatus->setStatus((int32_t) task->status);
    taskStatus->setMessage(std::make_shared<std::string>(msg));
    auto caps = Caps::new_instance();
    taskStatus->serialize(caps);
    this->sendMsg(caps, "end task");
    _taskRunner.reset();
    if (_pendingTaskCommand) {
      this->startNewTask(_pendingTaskCommand);
      _pendingTaskCommand.reset();
    }
  }
}

void JobManager::endTask(TaskStatus status) {
  LOG_INFO("end task with code %d", (int32_t)status);
  if (!_taskRunner) {
    LOG_ERROR("not task, ignored");
    return;
  }
  if (_taskRunner->getConf()->task->status != TaskStatus::RUNNING) {
    LOG_ERROR("task is not running, ignored");
    return;
  }
  _taskRunner->getConf()->task->status = status;
  _taskRunner->stop();
}

void JobManager::onWSMessage(std::shared_ptr<Caps> &caps) {
  rokid::MessageType type = rokid::get_msg_type(caps);
  switch (type) {
    case rokid::MessageType::TYPE_TASKCOMMAND:
      this->onTaskCommand(caps);
      break;
    default:
      LOG_ERROR("unknown ws message type %d, ignored", type);
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
      LOG_ERROR("unhandled ws event code %d, ignored", code);
      break;
  }
}

void JobManager::onWSConnected() {
  _wsConnected = true;
  auto deviceStatus = rokid::DeviceStatus::create();
  deviceStatus->setTimestamp(Util::getTimeMS());
  deviceStatus->setSn(DeviceInfo::sn.c_str());
  deviceStatus->setVersion(DeviceInfo::imageVersion.c_str());
  deviceStatus->setVspVersion(DeviceInfo::vspVersion.c_str());
  deviceStatus->setTurenVersion(DeviceInfo::turenVersion.c_str());
  TaskStatus status = TaskStatus::IDLE;
  int32_t shellId = 0;
  if (_taskRunner) {
    status = _taskRunner->getConf()->task->status;
    shellId = _taskRunner->getConf()->task->shellId;
  }
  deviceStatus->setStatus((int32_t) status);
  deviceStatus->setShellId(shellId);
  LOG_INFO("ws connected, status %d, shell %d", status, shellId);
  std::shared_ptr<Caps> caps;
  deviceStatus->serialize(caps);
  this->sendMsg(caps, "upload device status");
}

void JobManager::onWSDisconnected() {
  LOG_ERROR("ws disconnected");
  _wsConnected = false;
}

void JobManager::onTaskCommand(std::shared_ptr<Caps> &caps) {
  std::shared_ptr<rokid::TaskCommand> command = rokid::TaskCommand::create();
  int r = command->deserializeForCapsObj(caps);
  if (r != CAPS_SUCCESS) {
    LOG_ERROR("task command deserializeForCapsObj error");
    return;
  }
  auto type = command->getTaskType();
  LOG_INFO("on task command %s", type->c_str());
  if (*type == "CANCEL") {
    this->endTask(TaskStatus::USER_CANCEL);
  } else if (*type == "START") {
    this->startNewTask(command);
  }
}

void JobManager::startNewTask(const std::shared_ptr<rokid::TaskCommand> &taskCommand) {
  LOG_INFO("start new task with id %d", taskCommand->getTaskId());
  if (_taskRunner) {
    int runningId= _taskRunner->getConf()->task->id;
    if (runningId != taskCommand->getTaskId()) {
      LOG_ERROR("multi task %d %d", taskCommand->getTaskId(), runningId);
      _pendingTaskCommand = taskCommand;
      this->endTask(TaskStatus::MULTI_TASK_CANCEL);
      return;
    } else {
      LOG_ERROR("task %d is running, ignore start", runningId);
      return;
    }
  }
  auto taskInfo = std::make_shared<TaskInfo>();
  taskInfo->status = TaskStatus::RUNNING;
  taskInfo->id = taskCommand->getTaskId();
  taskInfo->type = taskCommand->getTaskType();
  taskInfo->shellId = taskCommand->getShellId();
  taskInfo->shell = taskCommand->getShellContent();
  taskInfo->shellType = taskCommand->getShellType();
  taskInfo->timestampMs = taskCommand->getTimestamp();

  std::shared_ptr<JobConf> shellConf(new JobConf);
  shellConf->task = taskInfo;
  shellConf->type = JobType::SPAWN_CHILD;
  shellConf->data = taskInfo->shell;
  shellConf->enable = true;
  shellConf->isRepeat = false;
  shellConf->loopCount = 0;
  shellConf->timeout = 0;
  shellConf->interval = 0;

  _taskRunner = this->addRunnerWithConf(shellConf);
}

void JobManager::startMonitor() {
  _disableUpload = Options::get<uint32_t>("disableUpload", 0) != 0;
  std::shared_ptr<JobConf> topConf(new JobConf);
  topConf->type = JobType::COLLECT_TOP;
  topConf->enable = true;
  topConf->isRepeat = true;
  topConf->loopCount = 0;
  topConf->timeout = 500;
  topConf->interval = 1000;
  _runners.push_back(this->addRunnerWithConf(topConf));

  std::shared_ptr<JobConf> smapConf(new JobConf);
  smapConf->type = JobType::COLLECT_SMAP;
  smapConf->enable = true;
  smapConf->isRepeat = true;
  smapConf->loopCount = 0;
  smapConf->timeout = 1000;
  smapConf->interval = Options::get<uint64_t>("smapInterval", 300 * 1000);
  LOG_INFO("smap interval %" PRIu64 "ms", smapConf->interval);
  _runners.push_back(this->addRunnerWithConf(smapConf));

  std::shared_ptr<JobConf> crashReporterConf(new JobConf);
  crashReporterConf->type = JobType::CRASH_REPORTER;
  crashReporterConf->enable = true;
  crashReporterConf->isRepeat = true;
  crashReporterConf->loopCount = 0;
  crashReporterConf->timeout = 5000;
  crashReporterConf->interval = 5000;
  _runners.push_back(this->addRunnerWithConf(crashReporterConf));

  std::shared_ptr<JobConf> batteryConf(new JobConf);
  batteryConf->type = JobType::COLLECT_BATTERY;
  batteryConf->enable = true;
  batteryConf->isRepeat = true;
  batteryConf->loopCount = 0;
  batteryConf->timeout = 3000;
  batteryConf->interval = 3000;
  _runners.push_back(this->addRunnerWithConf(batteryConf));
}

void JobManager::sendCollectData(std::shared_ptr<Caps> &caps, const char *hint){
  if (!_disableUpload) {
    this->sendMsg(caps, hint);
  }
}

void JobManager::sendMsg(std::shared_ptr<Caps> &caps, const char *hint) {
  if (!_wsConnected) {
    LOG_ERROR("ws disconnected, ignore send message %s", hint);
    return;
  }
  if (_ws) {
    _ws->sendMsg(caps, [hint](SendResult sr, void *) {
      LOG_VERBOSE("send ws %s result %u", hint, sr);
    });
  } else {
    LOG_ERROR("ws is null, drop %s data", hint);
  }
}

void JobManager::onUVHandleClosed(uv_handle_t *handle) {
  LOG_INFO("manager receive unknown handle close, free it");
  YODA_SIXSIX_SAFE_FREE(handle);
}

YODA_NS_END
