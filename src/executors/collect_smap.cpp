//
// Created by ximin.chen@rokid.com on 2018/11/22.
//

#include "./collect_smap.h"
#include "./busy_box.h"
#include "../conf.h"

YODA_NS_BEGIN

CollectSmap::CollectSmap() : IJobExecutor("CollectSmap"),
                             _scanDir(),
                             _workReq(nullptr),
                             _smaps(),
                             _sysMem(nullptr) {
  auto sysroot = Conf::get<std::string>("sysroot", "");
  _scanDir = sysroot + "/proc";
}

void CollectSmap::execute() {
  YODA_SIXSIX_SASSERT(!_workReq, "CollectSmap is running");
  _workReq = new uv_work_t;
  UV_MAKE_CB_WRAP1(_workReq, cb1, CollectSmap, doCollect, uv_work_t);
  UV_MAKE_CB_WRAP2(_workReq, cb2, CollectSmap, afterCollect, uv_work_t, int);
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
    }
  });

  _sysMem = busybox::getSystemMemory(_scanDir);
}

void CollectSmap::afterCollect(uv_work_t *, int) {
  YODA_SIXSIX_SAFE_DELETE(_workReq);
  auto collectData = this->getCollectData();
  auto memData = collectData->getMem();
  memData->setTimestamp(time(nullptr));

  auto sysMem = memData->getSysMem();
  sysMem->setAvailable(_sysMem->available);
  sysMem->setBuffers(_sysMem->buffers);
  sysMem->setCached(_sysMem->cached);
  sysMem->setFree(_sysMem->free);
  sysMem->setTotal(_sysMem->total);
  YODA_SIXSIX_FLOG("sys mem: %" PRIi64 " %" PRIi64,
                        _sysMem->total, _sysMem->available);

  auto memList = memData->getProcMemInfo();
  for (auto &smap : _smaps) {
    if (smap->pss > 0) {
      memList->emplace_back();
      memList->back().setPss(smap->pss);
      memList->back().setFullName(smap->fullname.c_str());
      memList->back().setPid(smap->pid);
      memList->back().setPrivateClean(smap->private_clean);
      memList->back().setPrivateDirty(smap->private_dirty);
      memList->back().setSharedClean(smap->shared_clean);
      memList->back().setSharedDirty(smap->shared_dirty);
      YODA_SIXSIX_FLOG("pss %d %s: %" PRIi64,
                            smap->pid,
                            smap->fullname.c_str(),
                            smap->pss
      );
    }
  }
  _smaps.clear();
  _sysMem.reset();

  this->onJobDone();
}

YODA_NS_END
