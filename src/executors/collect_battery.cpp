/*
 * Created by xiaofei.lan@rokid.com on 2019/04/26.
 */

#include "collect_battery.h"

#define CHARGER_PATH "/sys/class/power_supply/bq25890-charger"
#define BATTERY_PATH "/sys/class/power_supply/battery"

#define GET_CHARGER_FILE(path) CHARGER_PATH path
#define GET_BATTERY_FILE(path) BATTERY_PATH path

static void readFile(const char *path, char *buf, int32_t *value) {
  FILE *file;
  int size = 9;
  file = fopen(path, "rb");
  if (file == NULL) {
    LOG_VERBOSE("No such file: %s. skip collect info.", path);
    if (value != NULL) {
      *value = 0;
    }
    return;
  }
  size = fread(buf, 1, size, file);
  fclose(file);
  if (size <= 0) {
    if (value != NULL) {
      *value = 0;
    }
    return;
  }
  if (value != NULL) {
    *value = atoi(buf);
  }
}

YODA_NS_BEGIN

CollectBattery::CollectBattery() : IJobExecutor("CollectBattery"),
                                   _workReq(nullptr) {

}

CollectBattery::~CollectBattery() {
  ASSERT(_workReq == nullptr, "%s uv_work_t* _workReq not nullptr", _name.c_str());
}

void CollectBattery::execute() {
  ASSERT(!_workReq, "CollectBattery is running");
  _workReq = new uv_work_t;
  UV_CB_WRAP1(_workReq, cb1, CollectBattery, doCollect, uv_work_t);
  UV_CB_WRAP2(_workReq, cb2, CollectBattery, afterCollect, uv_work_t, int);
  uv_queue_work(uv_default_loop(), _workReq, cb1, cb2);
}

void CollectBattery::doCollect(uv_work_t *req) {
  LOG_VERBOSE("========== CollectBattery startup  ==========");
  char buffer[10];
  memset(buffer, 0, sizeof buffer);
  _timestamp = time(nullptr);

  readFile(GET_CHARGER_FILE("/temp"), buffer, &_bat_temp);

  readFile(GET_CHARGER_FILE("/constant_charge_current"), buffer, &_current);

  readFile(GET_CHARGER_FILE("/voltage_now"), buffer, &_usb_voltage);

  readFile(GET_CHARGER_FILE("/status"), buffer, NULL);
  strcpy(_status, buffer);

  readFile(GET_CHARGER_FILE("/online"), buffer, &_online);

  readFile(GET_CHARGER_FILE("/present"), buffer, &_present);

  readFile(GET_BATTERY_FILE("/capacity"), buffer, &_capacity);

  readFile(GET_BATTERY_FILE("/voltage_now"), buffer, &_bat_voltage);

  readFile("/sys/devices/virtual/thermal/thermal_zone0/temp", buffer, &_cpu_temp);

  LOG_VERBOSE("========== CollectBattery finish  ==========");
}

void CollectBattery::afterCollect(uv_work_t *req, int status) {
  LOG_VERBOSE("========== Battery Info  ============");
  LOG_VERBOSE("-> bat-temp: %d", _bat_temp);
  LOG_VERBOSE("-> cpu-temp: %d", _cpu_temp);
  LOG_VERBOSE("-> bat-volt: %d", _bat_voltage);
  LOG_VERBOSE("-> usb-volt: %d", _usb_voltage);
  LOG_VERBOSE("->  cur-now: %d", _current);
  LOG_VERBOSE("-> capacity: %d", _capacity);
  LOG_VERBOSE("->   status: %s", _status);
  LOG_VERBOSE("->   online: %d", _online);
  LOG_VERBOSE("->  present: %d", _present);

  rokid::BatteryInfosPtr data(new rokid::BatteryInfos);
  data->setBatTemp(_bat_temp);
  data->setCpuTemp(_cpu_temp);
  data->setVoltageBat(_bat_voltage);
  data->setVoltageUsb(_usb_voltage);
  data->setCurrentNow(_current);
  data->setCapacity(_capacity);
  data->setStatus(_status);
  data->setOnline(_online);
  data->setPresent(_present);
  data->setTimestamp(_timestamp);

  std::shared_ptr<Caps> caps;
  data->serialize(caps);
  this->sendData(caps, "battery info");

  YODA_SIXSIX_SAFE_DELETE(_workReq);
  this->onJobDone(0);
}

int CollectBattery::stop() {
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
