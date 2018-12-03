//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#include "./job_runner.h"
#include "./job_manager.h"
#include "./executors/collect_top.h"
#include "./executors/collect_smap.h"
#include "./executors/child_process.h"

YODA_NS_BEGIN

JobRunner::JobRunner(JobManager *manager) :
  _cb(),
  _conf(nullptr),
  _executor(nullptr),
  _timer(nullptr),
  _state(JobState::STOP),
  _executeCount(0),
  _manager(manager) {

}

JobRunner::~JobRunner() {
  YODA_SIXSIX_SLOG_INFO("runner exit");
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
    default:
      YODA_SIXSIX_FASSERT(0, "unknown job type %d", conf->type);
  }
  _executor->setExecuteCb(std::bind(&JobRunner::onExecuteFinish, this));
  _executor->setManager(_manager);
  return 0;
}

void JobRunner::run() {
  YODA_SIXSIX_SASSERT(_state == JobState::STOP, "job runner is running");
  YODA_SIXSIX_FLOG_INFO(
    "running job %s: timeout %" PRIu64 ", interval: %" PRIu64 ", repeat: %d, loopCount: %d",
    _executor->getName().c_str(),
    _conf->timeout,
    _conf->interval,
    _conf->isRepeat,
    _conf->loopCount
  );
  _state = JobState::RUNNING;
  _timer = new uv_timer_t;
  UV_MAKE_CB_WRAP1(_timer, cb, JobRunner, onTimer, uv_timer_t);
  uv_timer_init(uv_default_loop(), _timer);
  uv_timer_start(_timer, cb, _conf->timeout, _conf->interval);
}

int32_t JobRunner::stop() {
  YODA_SIXSIX_SASSERT(_state == JobState::RUNNING, "job runner is stopped");
  YODA_SIXSIX_FLOG_INFO("stopping job %s", _executor->getName().c_str());
  _state = JobState::STOP;
  uv_timer_stop(_timer);
  YODA_SIXSIX_SAFE_DELETE(_timer);
  int r = _executor->stop();
  return r;
}

void JobRunner::onExecuteFinish() {
  if (_state == JobState::STOP) {
    _executor.reset();
  } else if (!_conf->isRepeat && _executeCount >= _conf->loopCount) {
    this->stop();
  }
  if (_cb) {
    _cb(this);
  }
}

void JobRunner::onTimer(uv_timer_t *) {
  YODA_SIXSIX_SASSERT(_state != JobState::STOP, "job runner is stopped");
  ++_executeCount;
  _executor->execute();
}

YODA_NS_END
