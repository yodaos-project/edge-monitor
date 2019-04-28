/*
 * Created by xiaofei.lan@rokid.com on 2019/04/26.
 */

#include "collect_battery.h"

static char* readFile(const char *path) {
  FILE *file;
  int size = 0;
  char *buffer;
  file = fopen(path, "rb");
  if (file == NULL)
  {
    return NULL;
  }
  fseek(file, 0, SEEK_END);
  size = ftell(file);
  buffer = (char *)malloc((size + 1) * sizeof(char));
  rewind(file);
  size = fread(buffer, 1, size, file);
  buffer[size] = '\0';
  return buffer;
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
  char *bat_temp_char = readFile(GET_CHARGER_FILE("/temp"));
  bat_temp = atoi(bat_temp_char);
  free(bat_temp_char);

  char *bat_current_char = readFile(GET_CHARGER_FILE("/constant_charge_current"));
  current = atoi(bat_current_char);
  free(bat_current_char);

  char *usb_voltage_char = readFile(GET_CHARGER_FILE("/voltage_now"));
  usb_voltage = atoi(usb_voltage_char);
  free(usb_voltage_char);

  status = readFile(GET_CHARGER_FILE("/status"));

  char *bat_online_char = readFile(GET_CHARGER_FILE("/online"));
  online = atoi(bat_online_char);
  free(bat_online_char);

  char *bat_present_char = readFile(GET_CHARGER_FILE("/present"));
  present = atoi(bat_present_char);
  free(bat_present_char);

  char *bat_capacity_char = readFile(GET_BATTERY_FILE("/capacity"));
  capacity = atoi(bat_capacity_char);
  free(bat_capacity_char);

  char *bat_voltage_char = readFile(GET_BATTERY_FILE("/voltage_now"));
  bat_voltage = atoi(bat_voltage_char);
  free(bat_voltage_char);

  char *cpu_temp_char = readFile("/sys/devices/virtual/thermal/thermal_zone0/temp");
  cpu_temp = atoi(cpu_temp_char);
  free(cpu_temp_char);

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
  free(this->status);
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