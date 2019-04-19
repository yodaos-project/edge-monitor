//
// Created by ximin.chen@rokid.com on 2018/11/15.
//

#ifndef YODA_SIXSIX_OPTIONS_H
#define YODA_SIXSIX_OPTIONS_H

#include "common.h"

YODA_NS_BEGIN

struct JobConf;

class Options {
public:
  static void parseCmdLine(int32_t argc, char **argv);

  template<typename T>
  static T get(const std::string &key, const T &defaultValue) {
    auto ite = cmdArgs.find(key);
    if (ite == cmdArgs.end()) {
      return defaultValue;
    }
    std::stringstream ss(ite->second);
    T p;
    bool isSucceed = !(ss >> p).fail() && (ss >> std::ws).eof();
    YODA_SIXSIX_FASSERT(isSucceed, "get value %s error", key.c_str());
    return p;
  }

  static bool exists(const std::string &key) {
    return cmdArgs.find(key) != cmdArgs.end();
  }

private:
  static void parseConf(const char *confpath);

  static std::map<std::string, std::string> cmdArgs;
};

YODA_NS_END

#endif //YODA_SIXSIX_OPTIONS_H
