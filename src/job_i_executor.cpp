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

rokid::CollectDataPtr &IJobExecutor::getCollectData() {
  YODA_SIXSIX_FASSERT(_manager, "manager from %s is null", _name.c_str());
  return _manager->getCollectData();
}

void IJobExecutor::onJobDone() {
  if (_executeCb) {
    _executeCb();
  }
}

YODA_NS_END
