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
  _wsFirstConnected(true) {

}

void JobManager::setWsClient(WebSocketClient *ws) {
  _ws = ws;
  ws->setRecvCallback(std::bind(&JobManager::onWSMessage, this, _1));
  ws->setEventCallback(std::bind(&JobManager::onWSEvent, this, _1));
}

std::shared_ptr<JobRunner> JobManager::addRunnerWithConf(
  const std::shared_ptr<JobConf> &conf) {
  LOG_INFO("add job type %d with conf", (int32_t) conf->type);
  auto callback = std::bind(&JobManager::onRunnerStop, this, _1, _2);
  std::shared_ptr<JobRunner> runner(new JobRunner(this));
  runner->setJobCallback(callback);
  runner->initWithConf(conf);
  runner->run();
  return runner;
}

void JobManager::onRunnerStop(JobRunner *runner, int32_t exitCode) {
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
      if (runner->getState() == JobState::FAILED || exitCode != 0) {
        task->status = TaskStatus::FAILED;
      } else {
        task->status = TaskStatus::SUCCEED;
      }
    }
    sprintf(msg, "end task %d with status: %d", task->id, (int)task->status);
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
  LOG_INFO("ws connected, is first time: %d", _wsFirstConnected);
  if (_wsFirstConnected) {
    _wsFirstConnected = false;
    this->sendDeviceStatus();
  }
}

void JobManager::sendDeviceStatus() {
  LOG_INFO("send device info");
  auto deviceStatus = rokid::DeviceStatus::create();
  deviceStatus->setTimestamp(Util::getTimeMS());
  deviceStatus->setSn(DeviceInfo::sn.c_str());
  deviceStatus->setVersion(DeviceInfo::imageVersion.c_str());
  deviceStatus->setVspVersion(DeviceInfo::vspVersion.c_str());
  deviceStatus->setTurenVersion(DeviceInfo::turenVersion.c_str());
  deviceStatus->setStatus(0);
  deviceStatus->setShellId(0);
  std::shared_ptr<Caps> caps;
  deviceStatus->serialize(caps);
  this->sendMsg(caps, "upload device status");
}

void JobManager::onWSDisconnected() {
  LOG_ERROR("ws disconnected");
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
  } else {
    LOG_ERROR("unknown command %s", type->c_str());
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
  shellConf->timeout = 1000;
  shellConf->interval = 0;

  char msg[256] = {0};
  sprintf(msg, "task id: %d, shell id: %d", taskInfo->id, taskInfo->shellId);
  _taskRunner = this->addRunnerWithConf(shellConf);
  auto taskStatus = rokid::TaskStatus::create();
  taskStatus->setTaskId(taskInfo->id);
  taskStatus->setShellId(taskInfo->shellId);
  taskStatus->setTimestamp(Util::getTimeMS());
  taskStatus->setStatus((int32_t) taskInfo->status);
  taskStatus->setMessage(std::make_shared<std::string>(msg));
  auto caps = Caps::new_instance();
  taskStatus->serialize(caps);
  this->sendMsg(caps, "start task");
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

void JobManager::stopMonitor() {
  LOG_INFO("stop monitor");
  for (auto& _runner: _runners) {
    _runner->stop();
  }
}

void JobManager::sendCollectData(std::shared_ptr<Caps> &caps, const char *hint){
  if (!_disableUpload) {
    this->sendMsg(caps, hint);
  }
}

void JobManager::sendMsg(std::shared_ptr<Caps> &caps, const char *hint) {
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
