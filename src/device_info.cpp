//
// Created by ximin.chen@rokid.com on 2018-11-30.
//

#include "device_info.h"

YODA_NS_BEGIN

std::string DeviceInfo::sn = "sn-test-001";
std::string DeviceInfo::imageVersion = "version-test-001";
std::string DeviceInfo::hardware = "hardware-test-001";
const char *key_sn = "ro.boot.serialno";
const char *key_imageVersion = "ro.build.version.release";
const char *key_hardware = "ro.boot.hardware";

int32_t DeviceInfo::init() {
  size_t key_len_sn = strlen(key_sn);
  size_t key_len_imageVersion = strlen(key_imageVersion);
  size_t key_len_hardware = strlen(key_hardware);

  std::istringstream ss(Util::exec("getprop"));
  std::string line;

  #define READ_PROP(name) \
    if (line.find(key_##name) == 1 /* skip first '[' */) { \
      name = line.substr(key_len_##name + 5, len - key_len_##name - 1); \
      YODA_SIXSIX_FLOG("device " #name ": %s", name.c_str()); \
      continue; \
    }
  while (std::getline(ss, line)) {
    auto len = line.size();
    READ_PROP(sn)
    READ_PROP(imageVersion)
    READ_PROP(hardware)
  }
  return 0;
}

YODA_NS_END
