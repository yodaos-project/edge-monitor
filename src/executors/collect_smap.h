//
// Created by ximin.chen@rokid.com on 2018/11/22.
//

#ifndef YODA_SIXSIX_COLLECT_SMAP_H
#define YODA_SIXSIX_COLLECT_SMAP_H

#include "./executor_def.h"

YODA_NS_BEGIN

class CollectSmap : public IJobExecutor {
public:
  CollectSmap();

  ~CollectSmap() override;

  void execute() override;

  int stop() override;

protected:

  virtual void doCollect(uv_work_t *req);

  void afterCollect(uv_work_t *req, int code);

  std::string _scanDir;
  uv_work_t *_workReq;
  std::list<std::shared_ptr<ProcessSmapInfo>> _smaps;
  std::shared_ptr<SystemMemoryInfo> _sysMem;
};

YODA_NS_END

#endif //YODA_SIXSIX_COLLECT_SMAP_H
