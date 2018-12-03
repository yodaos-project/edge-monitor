//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#ifndef YODA_SIXSIX_JOB_MANAGER_H
#define YODA_SIXSIX_JOB_MANAGER_H

#include "./job_def.h"

class WebSocketClient;
enum class EventCode;

YODA_NS_BEGIN

class JobRunner;

class JobManager {
public:
  JobManager();

  int setWs(WebSocketClient *ws);

  void addRunnerWithConf(const std::shared_ptr<JobConf> &conf,
                         bool autoRun = false);

  void startTaskFromCmdConf();

  void startNewTask(const std::shared_ptr<TaskInfo> &task);

  rokid::CollectDataPtr& getCollectData() { return _collectData; }

private:

  void onRunnerResult(JobRunner *runner);

  void forceRemoveRunner(JobRunner *runner);

  void endTask(TaskErrorCodes errorCode);

  void onWSMessage(std::shared_ptr<Caps> &caps);

  void onWSEvent(EventCode code);

  void onTaskCommand(std::shared_ptr<Caps> &caps);

  void onTaskTimeout(uv_timer_t *timeoutReq);

  void manuallyStartJobs(
    const std::shared_ptr<std::string> &shell, int32_t shellId);

  void onCollectData(uv_timer_t *);

  void resetCollectData();

  void onWSConnected();

  void onWSDisconnected();

  rokid::CollectDataPtr _collectData;
  std::list<std::shared_ptr<JobRunner>> _runners;
  WebSocketClient *_ws;
  std::shared_ptr<TaskInfo> _task;
  uv_timer_t *_collectTimer;
  uv_timer_t *_taskTimer;
  bool _disableUpload;
};

YODA_NS_END

#endif //YODA_SIXSIX_JOB_MANAGER_H
