/*
 * Created by xiaofei.lan@rokid.com on 2019/04/26.
 */

#include "collect_battery.h"

static void readFile(const char *path, char *buf, int *value) {
  *buf = '\0';
  FILE *file;
  int size = 0;
  file = fopen(path, "rb");
  if (file == NULL)
  {
    strcpy(buf, "0");
    return;
  }
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  rewind(file);
  size = fread(buf, 1, size, file);
  if (size <= 0)
  {
    strcpy(buf, "0");
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
  YODA_SIXSIX_FASSERT(_workReq == nullptr, "%s uv_work_t* _workReq not nullptr", _name.c_str());
}

void CollectBattery::execute() {
  YODA_SIXSIX_SASSERT(!_workReq, "CollectBattery is running");
  _workReq = new uv_work_t;
  UV_CB_WRAP1(_workReq, cb1, CollectBattery, doCollect, uv_work_t);
  UV_CB_WRAP2(_workReq, cb2, CollectBattery, afterCollect, uv_work_t, int);
  uv_queue_work(uv_default_loop(), _workReq, cb1, cb2);
}

void CollectBattery::doCollect(uv_work_t *req) {
  YODA_SIXSIX_SLOG("========== CollectBattery startup  ==========");
  char buffer[10];
  readFile(GET_CHARGER_FILE("/temp"), buffer, &bat_temp);

  readFile(GET_CHARGER_FILE("/constant_charge_current"), buffer, &current);

  readFile(GET_CHARGER_FILE("/voltage_now"), buffer, &usb_voltage);

  readFile(GET_CHARGER_FILE("/status"), buffer, NULL);
  strcpy(status, buffer);

  readFile(GET_CHARGER_FILE("/online"), buffer, &online);

  readFile(GET_CHARGER_FILE("/present"), buffer, &present);

  readFile(GET_BATTERY_FILE("/capacity"), buffer, &capacity);

  readFile(GET_BATTERY_FILE("/voltage_now"), buffer, &bat_voltage);

  readFile("/sys/devices/virtual/thermal/thermal_zone0/temp", buffer, &cpu_temp);

  YODA_SIXSIX_SLOG("========== CollectBattery finish  ==========");
}

void CollectBattery::afterCollect(uv_work_t *req, int status) {
  printf("-> bat-temp: %d", bat_temp);
  printf("-> cpu-temp: %d", cpu_temp);
  printf("-> bat-volt: %d", bat_voltage);
  printf("-> usb-volt: %d", usb_voltage);
  printf("->  cur-now: %d", current);
  printf("-> capacity: %d", capacity);
  printf("->   status: %s", this->status);
  printf("->   online: %d", online);
  printf("->  present: %d", present);
  YODA_SIXSIX_SAFE_DELETE(_workReq);
  this->onJobDone();
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