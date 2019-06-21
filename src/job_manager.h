//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#ifndef YODA_SIXSIX_JOB_MANAGER_H
#define YODA_SIXSIX_JOB_MANAGER_H

#include "job_def.h"
#include "MessageCommon.h"

class WebSocketClient;
enum class EventCode;

YODA_NS_BEGIN

class JobRunner;

class JobManager {
public:
  JobManager();

  void setWsClient(WebSocketClient *ws);

  void sendCollectData(std::shared_ptr<Caps> &caps, const char *hint = "");

  void startMonitor();

  void stopMonitor();

private:

  void startNewTask(const std::shared_ptr<rokid::TaskCommand> &taskCommand);

  std::shared_ptr<JobRunner> addRunnerWithConf(
    const std::shared_ptr<JobConf> &conf);

  void onRunnerStop(JobRunner *runner, int32_t exitCode);

  void endTask(TaskStatus status);

  void onWSMessage(std::shared_ptr<Caps> &caps);

  void onWSEvent(EventCode code);

  void onTaskCommand(std::shared_ptr<Caps> &caps);

  void sendDeviceStatus();

  void onWSConnected();

  void onWSDisconnected();

  void sendMsg(std::shared_ptr<Caps> &caps, const char *hint = "");

  void onUVHandleClosed(uv_handle_t *handle);

  std::list<std::shared_ptr<JobRunner>> _runners;
  std::shared_ptr<JobRunner> _taskRunner;
  std::shared_ptr<rokid::TaskCommand> _pendingTaskCommand;
  WebSocketClient *_ws;
  bool _disableUpload;
  bool _wsFirstConnected;
};

YODA_NS_END

#endif //YODA_SIXSIX_JOB_MANAGER_H
