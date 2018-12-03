//
// Created by ximin.chen@rokid.com on 2018-11-30.
//

#include "device_info.h"

YODA_NS_BEGIN
//-serverAddress cd-perf-2 -serverPort 9997
std::string DeviceInfo::sn = "sn-test-001";
std::string DeviceInfo::imageVersion = "version-test-001";

int32_t DeviceInfo::init() {
  const char *snPrefix = "[ro.boot.serialno]: [";
  const char *imageVersionPrefix = "[ro.build.version.release]: [";
  size_t imageVersionPrefixLen = strlen(imageVersionPrefix);
  size_t snPrefixLen = strlen(snPrefix);

  std::istringstream ss(Util::exec("getprop"));
  std::string line;
  while (std::getline(ss, line)) {
    if (line.find(snPrefix) == 0) {
      sn = line.substr(snPrefixLen, line.size() - snPrefixLen - 1);
    } else if (line.find(imageVersionPrefix) == 0) {
      imageVersion = line.substr(imageVersionPrefixLen,
                                 line.size() - imageVersionPrefixLen - 1);
    }
  }
  YODA_SIXSIX_FLOG("device sn: %s", sn.c_str());
  YODA_SIXSIX_FLOG("device image version: %s", imageVersion.c_str());
  return 0;
}

YODA_NS_END
