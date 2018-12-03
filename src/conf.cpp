//
// Created by ximin.chen@rokid.com on 2018/11/15.
//

#include "./conf.h"

YODA_NS_BEGIN

std::map<std::string, std::string> Conf::cmdArgs;

void Conf::parseCmdLine(int32_t argc, char **argv) {
  if (argc % 2 != 1) {
    YODA_SIXSIX_SLOG("cmdline argc should be paired");
    exit(1);
  }
  for (int32_t i = 1; i < argc; ++i) {
    const char *key = argv[i] + 1;
    cmdArgs.insert({key, argv[++i]});
    YODA_SIXSIX_FLOG("cmdline args: %s=%s", key, argv[i]);
  }
}

YODA_NS_END
