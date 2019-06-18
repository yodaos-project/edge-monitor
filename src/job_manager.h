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

  int initWithWS(WebSocketClient *ws);

  void addRunnerWithConf(const std::shared_ptr<JobConf> &conf,
                         bool autoRun = false);

  void startNewTask(const std::shared_ptr<TaskInfo> &task);

  void sendCollectData(std::shared_ptr<Caps> &caps, const char *hint = "");

private:

  void onRunnerStop(JobRunner *runner);

  void endTask(TaskErrorCodes errorCode);

  void onWSMessage(std::shared_ptr<Caps> &caps);

  void onWSEvent(EventCode code);

  void onTaskCommand(std::shared_ptr<Caps> &caps);

  void manuallyStartJobs(
    const std::shared_ptr<std::string> &shell, int32_t shellId);

  void onWSConnected();

  void onWSDisconnected();

  void sendMsg(std::shared_ptr<Caps> &caps, const char *hint = "");

  void onUVHandleClosed(uv_handle_t *handle);

  std::list<std::shared_ptr<JobRunner>> _runners;
  WebSocketClient *_ws;
  std::shared_ptr<TaskInfo> _task;
  bool _disableUpload;
};

YODA_NS_END

#endif //YODA_SIXSIX_JOB_MANAGER_H
