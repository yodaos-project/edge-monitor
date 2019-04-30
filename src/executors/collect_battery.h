/*
 * Created by xiaofei.lan@rokid.com on 2019/04/26.
 */

#ifndef YODA_SIXSIX_COLLECT_BATTERY_H
#define YODA_SIXSIX_COLLECT_BATTERY_H

#include "executor_def.h"

#define CHARGER_PATH "/sys/class/power_supply/bq25890-charger"
#define BATTERY_PATH "/sys/class/power_supply/battery"

#define GET_CHARGER_FILE(path) CHARGER_PATH path
#define GET_BATTERY_FILE(path) BATTERY_PATH path

using namespace std;

YODA_NS_BEGIN

class CollectBattery : public IJobExecutor {
public:
  CollectBattery();

  ~CollectBattery() override;

  void execute() override;

  int stop() override;

protected:

  virtual void doCollect(uv_work_t *req);

  void afterCollect(uv_work_t *req, int status);

  uv_work_t *_workReq;
  int bat_temp;
  int cpu_temp;
  int current;
  int capacity;
  int bat_voltage;
  int usb_voltage;
  char status[10];
  int online;
  int present;
  int64_t timestamp;
};

YODA_NS_END

#endif //YODA_SIXSIX_COLLECT_BATTERY_H
