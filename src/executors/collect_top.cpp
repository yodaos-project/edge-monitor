//
// Created by ximin.chen@rokid.com on 2018/11/20.
//

#include "./collect_top.h"
#include "../conf.h"
#include "./busy_box.h"

YODA_NS_BEGIN

CollectTop::CollectTop() : IJobExecutor("CollectTop"),
                           _workReq(nullptr),
                           _top(nullptr) {
  auto sysroot = Conf::get<std::string>("sysroot", "");
  _scanDir = sysroot + "/proc";
}

void CollectTop::execute() {
  YODA_SIXSIX_SASSERT(!_workReq, "CollectTop is running");

  _workReq = new uv_work_t;
  UV_MAKE_CB_WRAP1(_workReq, cb1, CollectTop, doCollect, uv_work_t);
  UV_MAKE_CB_WRAP2(_workReq, cb2, CollectTop, afterCollect, uv_work_t, int);
  uv_queue_work(uv_default_loop(), _workReq, cb1, cb2);
}

void CollectTop::doCollect(uv_work_t *) {
  _top = busybox::getSystemTop(_scanDir);
}

void CollectTop::afterCollect(uv_work_t *, int) {
  YODA_SIXSIX_SLOG_INFO("========== busy idle iowait sys usr ==========");
  YODA_SIXSIX_SAFE_DELETE(_workReq);
  auto cpuInfos = this->getCollectData()->getCpu();
  cpuInfos->setTimestamp(time(nullptr));

  auto procList = cpuInfos->getProcCpuList();
  for (auto &pair : _top->processes) {
    if (pair.second->cpuUsagePercent > 0.0f) {
      auto &proc = pair.second;
      YODA_SIXSIX_FLOG_INFO("process %d %s: %f",
                            proc->pid,
                            proc->fullname.c_str(),
                            proc->cpuUsagePercent
      );
      procList->emplace_back();
      procList->back().setPid(proc->pid);
      procList->back().setFullName(proc->fullname.c_str());
      procList->back().setStatus(proc->state);
      procList->back().setCpuUsage(proc->cpuUsagePercent);
      procList->back().setNice(proc->nice);
      procList->back().setStime(proc->stime);
      procList->back().setUtime(proc->utime);
      procList->back().setTicks(proc->ticks);
    }
  }

  auto sysCpu = cpuInfos->getSysCpu();
  auto cores = sysCpu->getCores();
  int32_t i = 0;
  for (auto &core : _top->cpu->cores) {
    YODA_SIXSIX_FLOG_INFO("core: %d %f %f %f %f %f",
                          i++,
                          core->busyPercent,
                          core->idlePercent,
                          core->iowaitPercent,
                          core->sysPercent,
                          core->usrPercent
    );
    cores->emplace_back();
    cores->back().setBusyUsage(core->busyPercent);
    cores->back().setIdleUsage(core->idlePercent);
    cores->back().setIoUsage(core->iowaitPercent);
    cores->back().setSysUsage(core->sysPercent);
    cores->back().setUsrUsage(core->usrPercent);
  }

  auto total = sysCpu->getTotal();
  YODA_SIXSIX_FLOG_INFO("total : %f %f %f %f %f",
                        _top->cpu->total->busyPercent,
                        _top->cpu->total->idlePercent,
                        _top->cpu->total->iowaitPercent,
                        _top->cpu->total->sysPercent,
                        _top->cpu->total->usrPercent
  );
  total->setBusyUsage(_top->cpu->total->busyPercent);
  total->setIdleUsage(_top->cpu->total->idlePercent);
  total->setIoUsage(_top->cpu->total->iowaitPercent);
  total->setSysUsage(_top->cpu->total->sysPercent);
  total->setUsrUsage(_top->cpu->total->usrPercent);

  this->onJobDone();
}

int CollectTop::stop() {
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
