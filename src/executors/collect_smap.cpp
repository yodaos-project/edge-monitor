//
// Created by ximin.chen@rokid.com on 2018/11/22.
//

#include "collect_smap.h"
#include "busy_box.h"
#include "options.h"
#include "util.h"

YODA_NS_BEGIN

CollectSmap::CollectSmap() : IJobExecutor("CollectSmap"),
                             _scanDir(),
                             _usleepTime(0),
                             _workReq(nullptr),
                             _smaps(),
                             _sysMem(nullptr) {
  _scanDir = Options::get<std::string>("sysroot", "") + "/proc";
  _usleepTime = (Options::get<uint64_t>("smapSleep", 1000)) * 1000;
  LOG_INFO("smap sleep time %" PRIu64 "ms", _usleepTime / 1000);
}

CollectSmap::~CollectSmap() {
  ASSERT(_workReq == nullptr, "%s work not null", _name.c_str());
}

void CollectSmap::execute() {
  ASSERT(!_workReq, "CollectSmap is running");
  _workReq = new uv_work_t;
  UV_CB_WRAP1(_workReq, cb1, CollectSmap, doCollect, uv_work_t);
  UV_CB_WRAP2(_workReq, cb2, CollectSmap, afterCollect, uv_work_t, int);
  uv_queue_work(uv_default_loop(), _workReq, cb1, cb2);
}

int CollectSmap::stop() {
  if (!_workReq) {
    return 0;
  }
  int r = uv_cancel((uv_req_t *) _workReq);
  if (r == 0) {
    YODA_SIXSIX_SAFE_DELETE(_workReq);
  }
  return r;
}

void CollectSmap::doCollect(uv_work_t *) {
  Util::scanDir(_scanDir, [this](const char *filename) {
    uint32_t pid;
    // determine if file is pid dir
    if (Util::lexicalCast<uint32_t>(filename, &pid)) {
      std::string pidDir = _scanDir + "/" + filename;
      if (Util::isDir(pidDir)) {
        auto smap = busybox::getProcessSmap(pidDir, pid);
        if (smap) {
          _smaps.emplace_back(smap);
        }
      }
      usleep(_usleepTime);
    }
  });

  _sysMem = busybox::getSystemMemory(_scanDir);
}

void CollectSmap::afterCollect(uv_work_t *, int status) {
  if (status == 0) {
    rokid::MemInfosPtr data(new rokid::MemInfos);
    data->setTimestamp(time(nullptr));

    rokid::SysMemInfoPtr sysMem(new rokid::SysMemInfo);
    sysMem->setAvailable(_sysMem->available);
    sysMem->setBuffers(_sysMem->buffers);
    sysMem->setCached(_sysMem->cached);
    sysMem->setFree(_sysMem->free);
    sysMem->setTotal(_sysMem->total);
    LOG_INFO("sys mem: total %" PRIi64 " available %" PRIi64,
                     _sysMem->total, _sysMem->available);
    data->setSysMem(sysMem);

    std::shared_ptr<std::vector<rokid::ProcMemInfo>> procMems(
      new std::vector<rokid::ProcMemInfo>()
    );
    for (auto &smap : _smaps) {
      if (smap->pss > 0) {
        procMems->emplace_back();
        rokid::ProcMemInfo &mem = procMems->back();
        mem.setPss(smap->pss);
        mem.setFullName(smap->fullname.c_str());
        mem.setPid(smap->pid);
        mem.setPrivateClean(smap->private_clean);
        mem.setPrivateDirty(smap->private_dirty);
        mem.setSharedClean(smap->shared_clean);
        mem.setSharedDirty(smap->shared_dirty);
        LOG_VERBOSE("pss %d %s: %" PRIi64,
                         smap->pid,
                         smap->fullname.c_str(),
                         smap->pss);
      }
    }
    data->setProcMemInfo(procMems);

    std::shared_ptr<Caps> caps;
    data->serialize(caps);
    this->sendData(caps, "smap data");
  } else {
    LOG_ERROR("smap collect error status: %d", status);
  }

  YODA_SIXSIX_SAFE_DELETE(_workReq);
  _smaps.clear();
  _sysMem.reset();

  this->onJobDone();
}

YODA_NS_END
