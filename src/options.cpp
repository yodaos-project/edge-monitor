//
// Created by ximin.chen@rokid.com on 2018/11/15.
//

#include "options.h"
#include <rapidjson/istreamwrapper.h>

YODA_NS_BEGIN

std::map<std::string, std::string> Options::cmdArgs;

void Options::parseCmdLine(int32_t argc, char **argv) {
  for (int32_t i = 1; i < argc; ) {
    const char *key = argv[i++] + 1;
    const char *value;
    if (i >= argc || argv[i][0] == '-') {
      value = "";
      LOG_INFO("cmdline args: -%s", key);
    } else {
      value = argv[i++];
      LOG_INFO("cmdline args: %s=%s", key, value);
    }
    cmdArgs.insert({key, value});
  }
  auto ite = cmdArgs.find("conf");
  if (ite != cmdArgs.end()) {
    parseConf(ite->second.c_str());
  }
}

void Options::parseConf(const char *confpath) {
  std::ifstream ifs(confpath);
  rapidjson::IStreamWrapper ifsWrapper(ifs);
  ASSERT(ifs.is_open(), "cannot load conf from %s", confpath);
  rapidjson::Document doc;
  doc.ParseStream(ifsWrapper);
  ASSERT(!doc.HasParseError(), "conf parse error %s", confpath);
  for (rapidjson::Value::ConstMemberIterator ite = doc.MemberBegin();
    ite != doc.MemberEnd(); ++ite) {
    const char *key = ite->name.GetString();
    if (strcmp(key, "task") == 0) {
      continue;
    }
    if (ite->value.IsString()) {
      cmdArgs.insert({key, ite->value.GetString()});
    } else {
      ASSERT(ite->value.IsInt(), "%s is not string or int32", key);
      cmdArgs.insert({key, std::to_string(ite->value.GetInt())});
    }
    continue;
  }
}

YODA_NS_END
