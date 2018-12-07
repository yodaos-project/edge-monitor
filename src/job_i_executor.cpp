//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#include "./job_i_executor.h"
#include "./job_manager.h"

YODA_NS_BEGIN

IJobExecutor::IJobExecutor(const std::string &name) : _name(name),
                                                      _executeCb(nullptr),
                                                      _manager(nullptr) {

}

IJobExecutor::~IJobExecutor() {
  YODA_SIXSIX_FLOG("executor %s exit", _name.c_str());
}

void IJobExecutor::sendData(std::shared_ptr<Caps> &caps, const char *hint) {
  _manager->sendCollectData(caps, hint);
}

void IJobExecutor::onJobDone() {
  if (_executeCb) {
    _executeCb();
  }
}

YODA_NS_END
