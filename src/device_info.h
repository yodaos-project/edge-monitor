//
// Created by ximin.chen@rokid.com on 2018-11-30.
//

#ifndef YODA_SIXSIX_DEVICE_INFO_H
#define YODA_SIXSIX_DEVICE_INFO_H

#include "./common.h"

YODA_NS_BEGIN

class DeviceInfo {
public:
  static int32_t init();
  static std::string sn;
  static std::string imageVersion;
  static std::string hardware;
};

YODA_NS_END

#endif //YODA_SIXSIX_DEVICE_INFO_H
