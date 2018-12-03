//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#ifndef YODA_SIXSIX_JOB_RUNNER_H
#define YODA_SIXSIX_JOB_RUNNER_H

#include "./job_def.h"

YODA_NS_BEGIN

class IJobExecutor;

class JobRunner;

class JobManager;

typedef std::function<void(JobRunner *runner)> RunnerExecuteCallback;

class JobRunner {
public:
  explicit JobRunner(JobManager *manager);

  ~JobRunner();

  int initWithConf(const std::shared_ptr<JobConf> &conf);

  void run();

  int32_t stop();

  JobState getState() { return _state; }

  JobType getType() { return _conf->type; }

  void setJobCallback(const RunnerExecuteCallback &cb) { _cb = cb; }

private:

  void onExecuteFinish();

  void onTimer(uv_timer_t *req);

  RunnerExecuteCallback _cb;
  std::shared_ptr<JobConf> _conf;
  std::shared_ptr<IJobExecutor> _executor;
  uv_timer_t *_timer;

  JobState _state;
  uint32_t _executeCount;

  JobManager *_manager;
};

YODA_NS_END

#endif //YODA_SIXSIX_JOB_RUNNER_H
