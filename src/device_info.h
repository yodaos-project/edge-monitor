//
// Created by ximin.chen@rokid.com on 2018-11-30.
//

#ifndef YODA_SIXSIX_DEVICE_INFO_H
#define YODA_SIXSIX_DEVICE_INFO_H

#include "def.h"

YODA_NS_BEGIN

class DeviceInfo {
public:
  static int32_t init();
  static std::string sn;
  static std::string imageVersion;
  static std::string hardware;
  static std::string turenVersion;
  static std::string vspVersion;
  static std::string typeId;
  static std::string productName;
};

YODA_NS_END

#endif //YODA_SIXSIX_DEVICE_INFO_H
