//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#ifndef YODA_SIXSIX_I_EXECUTOR_H
#define YODA_SIXSIX_I_EXECUTOR_H

#include "job_def.h"

YODA_NS_BEGIN

class JobManager;

typedef std::function<void()> ExecuteCallback;

class IJobExecutor {
public:
  IJobExecutor() = delete;

  explicit IJobExecutor(const std::string &name);

  virtual ~IJobExecutor();

  virtual void execute() = 0;

  const std::string &getName() { return _name; }

  virtual int stop() = 0;

  void setExecuteCb(const ExecuteCallback &cb) { _executeCb = cb; }

  void setManager(JobManager *manager) {
    _manager = manager;
  }

protected:

  void sendData(std::shared_ptr<Caps> &caps, const char *hint);

  void onJobDone();

  std::string _name;

  ExecuteCallback _executeCb;

  JobManager *_manager;
};

YODA_NS_END

#endif //YODA_SIXSIX_I_EXECUTOR_H
