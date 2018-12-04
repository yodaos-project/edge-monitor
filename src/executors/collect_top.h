//
// Created by ximin.chen@rokid.com on 2018/11/20.
//

#ifndef YODA_SIXSIX_COLLECT_TOP_H
#define YODA_SIXSIX_COLLECT_TOP_H

#include "./executor_def.h"

YODA_NS_BEGIN

class CollectTop : public IJobExecutor {
public:
  CollectTop();

  ~CollectTop() override;

  void execute() override;

  int stop() override;

protected:

  virtual void doCollect(uv_work_t *req);

  void afterCollect(uv_work_t *req, int code);

  std::string _scanDir;
  uv_work_t *_workReq;
  std::shared_ptr<SystemTopInfo> _top;
};

YODA_NS_END

#endif //YODA_SIXSIX_COLLECT_TOP_H
