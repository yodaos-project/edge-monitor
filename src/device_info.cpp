//
// Created by ximin.chen@rokid.com on 2018-11-30.
//

#include "device_info.h"
#include "util.h"

YODA_NS_BEGIN

std::string DeviceInfo::sn = "sn-unknown";
std::string DeviceInfo::imageVersion = "version-unknown";
std::string DeviceInfo::hardware = "hardware-unknown";
std::string DeviceInfo::turenVersion = "turen-unknown";
std::string DeviceInfo::vspVersion = "vsp-unknown";
std::string DeviceInfo::typeId = "typeId-unknown";
std::string DeviceInfo::productName = "productname-unknown";
const char *key_sn = "ro.boot.serialno";
const char *key_imageVersion = "ro.build.version.release";
const char *key_hardware = "ro.boot.hardware";
const char *key_vspVersion = "ro.rokid.build.vsp";
const char *key_productName = "ro.rokid.build.productname";
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
    READ_PROP(productName);
  }

  auto aifeVersion =
    Util::exec("lothalproc -v | grep lothal.so | awk '{printf $3}'");
  if (aifeVersion.empty()) {
    LOG_ERROR("get aife version error");
  } else {
    LOG_INFO("aife version: %s", aifeVersion.c_str());
    turenVersion = aifeVersion;
  }
  return 0;
}

YODA_NS_END
