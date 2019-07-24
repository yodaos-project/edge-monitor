//
// Created by ximin.chen@rokid.com on 2018-11-30.
//

#include "device_info.h"
#include "util.h"

YODA_NS_BEGIN

std::string DeviceInfo::sn = "sn-test-001";
std::string DeviceInfo::imageVersion = "version-test-001";
std::string DeviceInfo::hardware = "hardware-test-001";
std::string DeviceInfo::turenVersion = "turen-test-001";
std::string DeviceInfo::vspVersion = "vsp-test-001";
std::string DeviceInfo::typeId = "typeId-test-001";
const char *key_sn = "ro.boot.serialno";
const char *key_imageVersion = "ro.build.version.release";
const char *key_hardware = "ro.boot.hardware";
const char *key_vspVersion = "ro.rokid.build.vsp";
const char *key_typeId = "ro.boot.devicetypeid";

int32_t DeviceInfo::init() {
  std::istringstream ss(Util::exec("getprop"));
  std::string line;

  #define READ_PROP(name) \
    if (line.find(key_##name) == 1 /* skip first '[' */) { \
      size_t prefixLen = strlen(key_##name) + 5; /* skip "[" and "]: [" */ \
      name = line.substr(prefixLen, len - prefixLen - 1); \
      LOG_INFO("device " #name ": %s", name.c_str()); \
      continue; \
    }
  while (std::getline(ss, line)) {
    auto len = line.size();
    READ_PROP(sn);
    READ_PROP(imageVersion);
    READ_PROP(hardware);
    READ_PROP(vspVersion);
    READ_PROP(typeId);
  }

  turenVersion = Util::exec("lothalproc -v | grep lothal.so | awk '{printf $3}'");
  if (turenVersion.empty()) {
    LOG_ERROR("get turen info error");
    turenVersion = "unknownTurenVersion";
  } else {
    LOG_INFO("turen version: %s", turenVersion.c_str());
  }
  return 0;
}

YODA_NS_END
