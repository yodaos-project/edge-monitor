//
// Created by ximin.chen@rokid.com on 2018/12/31.
//

#include "multi_thread_executor.h"

YODA_NS_BEGIN

IMultiThreadExecutor::IMultiThreadExecutor(const std::string &name) :
  IJobExecutor(name),
  _workReq(nullptr) {

}

IMultiThreadExecutor::~IMultiThreadExecutor() {
  ASSERT(_workReq == nullptr, "%s work not null", _name.c_str());
}

void IMultiThreadExecutor::execute() {
  ASSERT(!_workReq, "%s is running", _name.c_str());
  _workReq = new uv_work_t;
  UV_CB_WRAP1(_workReq, cb1, IMultiThreadExecutor, onThreadStart, uv_work_t);
  UV_CB_WRAP2(_workReq, cb2, IMultiThreadExecutor, onThreadEnd, uv_work_t, int);
  uv_queue_work(uv_default_loop(), _workReq, cb1, cb2);
}

void IMultiThreadExecutor::onThreadStart(uv_work_t *req) {
  this->doExecute(req);
}

void IMultiThreadExecutor::onThreadEnd(uv_work_t *req, int status) {
  int code = this->afterExecute(req, status);
  YODA_SIXSIX_SAFE_DELETE(_workReq);
  this->onJobDone(code);
}

int IMultiThreadExecutor::stop() {
  if (!_workReq) {
    return 0;
  }
  int r = uv_cancel((uv_req_t *) _workReq);
  if (r == 0) {
    YODA_SIXSIX_SAFE_DELETE(_workReq);
  }
  return r;
}

YODA_NS_END
