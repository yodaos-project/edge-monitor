/*
 * Created by xiaofei.lan@rokid.com on 2019/05/09.
 */

#ifndef YODA_SIXSIX_WATCH_PID_H
#define YODA_SIXSIX_WATCH_PID_H

#include "executor_def.h"

YODA_NS_BEGIN

class WatchPID : public IJobExecutor {
public:
  WatchPID();

  ~WatchPID() override;

  void execute() override;

  int stop() override;

protected:

  void doCollect(uv_work_t *req);

  void afterCollect(uv_work_t *req, int status);

  void initConf(std::string path);

  uv_work_t *_workReq;

  std::string _scanDir;

  rapidjson::Document _doc;
};

YODA_NS_END

#endif //YODA_SIXSIX_WATCH_PID_H
