//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#include "job_runner.h"
#include "job_manager.h"
#include "collect_top.h"
#include "collect_smap.h"
#include "child_process.h"
#include "crash_reporter.h"

YODA_NS_BEGIN

JobRunner::JobRunner(JobManager *manager) :
  _stopCb(),
  _conf(nullptr),
  _executor(nullptr),
  _timer(nullptr),
  _state(JobState::STOP),
  _executeCount(0),
  _manager(manager),
  _name("unknown") {

}

JobRunner::~JobRunner() {
  ASSERT(_timer == nullptr, "%s timer is not null", _name.c_str());
  LOG_INFO("runner %s exit", _name.c_str());
}

int JobRunner::initWithConf(const std::shared_ptr<JobConf> &conf) {
  _conf = conf;
  switch (conf->type) {
    case JobType::COLLECT_TOP:
      _executor = std::shared_ptr<IJobExecutor>(new CollectTop());
      break;
    case JobType::COLLECT_SMAP:
      _executor = std::shared_ptr<IJobExecutor>(new CollectSmap());
      break;
    case JobType::SPAWN_CHILD:
      _executor = std::shared_ptr<IJobExecutor>(new ChildProcess(conf));
      break;
    case JobType::CRASH_REPORTER:
      _executor = std::shared_ptr<IJobExecutor>(new CrashReporter());
      break;
    default:
      ASSERT(0, "unknown job type %d", conf->type);
  }
  _name = _executor->getName();
  _executor->setExecuteCb(std::bind(&JobRunner::onExecuteFinish, this));
  _executor->setManager(_manager);
  return 0;
}

void JobRunner::run() {
  ASSERT(_state == JobState::STOP, "job runner is running");
  LOG_INFO(
    "running job %s: timeout %"
    PRIu64
    ", interval: %"
    PRIu64
    ", repeat: %d, loopCount: %d",
    _executor->getName().c_str(),
    _conf->timeout,
    _conf->interval,
    _conf->isRepeat,
    _conf->loopCount
  );
  _state = JobState::RUNNING;
  _timer = new uv_timer_t;
  UV_CB_WRAP1(_timer, cb, JobRunner, onTimer, uv_timer_t);
  uv_timer_init(uv_default_loop(), _timer);
  uv_timer_start(_timer, cb, _conf->timeout, 0);
}

int32_t JobRunner::stop() {
  ASSERT(_state == JobState::RUNNING, "job runner is stopped");
  LOG_INFO("stopping job %s", _executor->getName().c_str());
  _state = JobState::STOP;
  uv_timer_stop(_timer);
  UV_CLOSE_HANDLE(_timer, JobRunner, onUVHandleClosed);
  int r = _executor->stop();
  if (r == 0) {
    _executor.reset();
  } else {
    LOG_INFO("executor stop result error %s", uv_err_name(r));
  }
  return 1;
}

void JobRunner::onExecuteFinish() {
  if (_state == JobState::STOP) {
    // timer is closing or closed
    int32_t r = _executor->stop();
    if (r == 0) {
      if (!_timer) {
        _stopCb(this);
      }
    }
  } else if (!_conf->isRepeat && _executeCount >= _conf->loopCount) {
    // timer is running
    this->stop();
  } else {
    UV_CB_WRAP1(_timer, cb, JobRunner, onTimer, uv_timer_t);
    uv_timer_start(_timer, cb, _conf->interval, 0);
  }
}

void JobRunner::onTimer(uv_timer_t *) {
  ASSERT(_state != JobState::STOP, "job runner is stopped");
  ++_executeCount;
  _executor->execute();
}

void JobRunner::onUVHandleClosed(uv_handle_t *) {
  YODA_SIXSIX_SAFE_DELETE(_timer);
  // executor is running or closed
  if (!_executor) {
    _stopCb(this);
  }
}

YODA_NS_END
