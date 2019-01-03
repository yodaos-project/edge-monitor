//
// Created by ximin.chen@rokid.com on 2018/12/31.
//

#ifndef YODA_SIXSIX_MULTI_THREAD_EXECUTOR_H
#define YODA_SIXSIX_MULTI_THREAD_EXECUTOR_H

#include "executor_def.h"

YODA_NS_BEGIN

class IMultiThreadExecutor : public IJobExecutor {
public:
  IMultiThreadExecutor() = delete;

  explicit IMultiThreadExecutor(const std::string &name);

  ~IMultiThreadExecutor() override;

  void execute() override;

  int stop() override;

protected:

  virtual void doExecute(uv_work_t *req) = 0;

  virtual void afterExecute(uv_work_t *req, int status) = 0;

  void onThreadStart(uv_work_t *req);

  void onThreadEnd(uv_work_t *req, int status);
  
  uv_work_t *_workReq;
};

YODA_NS_END

#endif //YODA_SIXSIX_MULTI_THREAD_EXECUTOR_H
