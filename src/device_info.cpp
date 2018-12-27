//
// Created by ximin.chen@rokid.com on 2018-11-30.
//

#include "device_info.h"

YODA_NS_BEGIN

std::string DeviceInfo::sn = "sn-test-001";
std::string DeviceInfo::imageVersion = "version-test-001";
std::string DeviceInfo::hardware = "hardware-test-001";

int32_t DeviceInfo::init() {
  const char *snPrefix = "[ro.boot.serialno]: [";
  const char *versionPrefix = "[ro.build.version.release]: [";
  const char *hardwarePrefix = "[ro.boot.hardware]: [";
  size_t versionPrefixLen = strlen(versionPrefix);
  size_t snPrefixLen = strlen(snPrefix);
  size_t hardwarePrefixLen = strlen(hardwarePrefix);

  std::istringstream ss(Util::exec("getprop"));
  std::string line;
  while (std::getline(ss, line)) {
    auto len = line.size();
    if (line.find(snPrefix) == 0) {
      sn = line.substr(snPrefixLen, len - snPrefixLen - 1);
    } else if (line.find(versionPrefix) == 0) {
      imageVersion = line.substr(versionPrefixLen, len - versionPrefixLen - 1);
    } else if (line.find(hardwarePrefix) == 0) {
      hardware = line.substr(hardwarePrefixLen, len - hardwarePrefixLen - 1);
    }
  }
  YODA_SIXSIX_FLOG("device sn: %s", sn.c_str());
  YODA_SIXSIX_FLOG("device image version: %s", imageVersion.c_str());
  YODA_SIXSIX_FLOG("hardware: %s", hardware.c_str());
  return 0;
}

YODA_NS_END
