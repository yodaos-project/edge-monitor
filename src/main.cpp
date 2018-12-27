//
// Created by ximin.chen@rokid.com on 2018/11/19.
//
#include "./job_manager.h"
#include "./conf.h"
#include "./websocket/WebSocketClient.h"
#include "./message/MessageCommon.h"
#include "./device_info.h"

using namespace rokid;

static const char *version = "v1.0.0\n";
static const char *helpStr =
  "Usage: \n"
  "[-version] print version\n"
  "[-sysroot] to specific samples sysroot\n"
  "[-taskJson] to start task from local json path\n"
  "[-disableUpload] set none-zero to disable upload data\n"
  "[-serverAddress] specific server address\n"
  "[-serverPort] specific server port\n"
  "[-sn] mock an sn\n"
  "[-hardware] mock a hardware\n";

static void parseExitCmd(int argc, char **argv) {
  if (argc >= 2) {
    const char *print = nullptr;
    if (strcmp(argv[1], "-help") == 0) {
      print = helpStr;
    } else if (strcmp(argv[1], "-version") == 0) {
      print = version;
    }
    if (print) {
      printf("%s", print);
      exit(0);
    }
  }
}

int main(int argc, char **argv) {
  setpriority(PRIO_PGRP, 0, 19);
  parseExitCmd(argc, argv);
  YODA_SIXSIX_SLOG("starting app");
  yoda::DeviceInfo::init();
  yoda::Conf::parseCmdLine(argc, argv);

  yoda::JobManager manager;
  manager.startTaskFromCmdConf();

  WebSocketClient wsc(uv_default_loop(), 5);

  char path[128];
  auto serverAddress = yoda::Conf::get<std::string>("serverAddress", "");
  auto serverPort = yoda::Conf::get<uint32_t>("serverPort", 0);
  auto mockSN = yoda::Conf::get<std::string>("sn", "");
  auto mockHardware = yoda::Conf::get<std::string>("hardware", "");
  std::string sn = mockSN.empty() ? yoda::DeviceInfo::sn : mockSN;
  std::string hardware = mockHardware.empty() ? 
    yoda::DeviceInfo::hardware : mockHardware;
  sprintf(path, "/websocket?sn=%s&hardware=%s", sn.c_str(), hardware.c_str());
  if (serverAddress.empty() || serverPort == 0) {
    YODA_SIXSIX_FERROR("ws connect error, server: %s, port: %d",
                       serverAddress.c_str(), serverPort);
  } else {
    YODA_SIXSIX_FLOG("ws connect to %s:%d%s",
                     serverAddress.c_str(), serverPort, path);
    manager.setWs(&wsc);
    wsc.start(serverAddress.c_str(), serverPort, path);
  }

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  int32_t r = uv_loop_close(uv_default_loop());
  YODA_SIXSIX_FASSERT(r == 0, "uv loop close error %s", uv_err_name(r));
  wsc.stop();
  YODA_SIXSIX_SLOG("app finished");
  return 0;
}
