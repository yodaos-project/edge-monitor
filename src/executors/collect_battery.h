/*
 * Created by xiaofei.lan@rokid.com on 2019/04/26.
 */

#ifndef YODA_SIXSIX_COLLECT_BATTERY_H
#define YODA_SIXSIX_COLLECT_BATTERY_H

#include "executor_def.h"

YODA_NS_BEGIN

class CollectBattery : public IJobExecutor {
public:
  CollectBattery();

  ~CollectBattery() override;

  void execute() override;

  int stop() override;

protected:

  void doCollect(uv_work_t *req);

  void afterCollect(uv_work_t *req, int status);

  uv_work_t *_workReq;
  int32_t _bat_temp;
  int32_t _cpu_temp;
  int32_t _current;
  int32_t _capacity;
  int32_t _bat_voltage;
  int32_t _usb_voltage;
  char _status[10];
  int32_t _online;
  int32_t _present;
  int64_t _timestamp;
};

YODA_NS_END

#endif //YODA_SIXSIX_COLLECT_BATTERY_H
