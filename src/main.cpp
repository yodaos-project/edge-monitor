//
// Created by ximin.chen@rokid.com on 2018/11/19.
//
#include "./job_manager.h"
#include "./conf.h"
#include "./websocket/WebSocketClient.h"
#include "./CapsMsg.h"
#include "./device_info.h"

using namespace rokid;

static const char *version = "v1.0.0\n";
static const char *helpStr = "Usage: \n"
                             "[-version] print version\n"
                             "[-sysroot] to specific samples sysroot\n"
                             "[-taskJson]	to start task from local json path\n"
                             "[-disableUpload] set 1 to disable upload data\n"
                             "[-serverAddress] specific server address\n"
                             "[-serverPort] specific server port\n";

int main(int argc, char **argv) {
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
  YODA_SIXSIX_SLOG_INFO("starting app");
  yoda::DeviceInfo::init();
  yoda::Conf::parseCmdLine(argc, argv);

  WebSocketClient wsc(uv_default_loop(), 5);

  auto manager = yoda::JobManager();
  manager.setWs(&wsc);
  manager.startTaskFromCmdConf();

  char urlPath[128];
  auto serverAddress = yoda::Conf::get<std::string>("serverAddress", "0.0.0.0");
  auto serverPort = yoda::Conf::get<uint32_t>("serverPort", 8080);
  auto sn = yoda::DeviceInfo::sn;
  sprintf(urlPath, "/websocket/%s", sn.c_str());
  YODA_SIXSIX_FLOG_INFO("ws connect to %s:%d%s",
                        serverAddress.c_str(), serverPort, urlPath
  );
  wsc.start(serverAddress.c_str(), serverPort, urlPath);

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  uv_loop_close(uv_default_loop());
  wsc.stop();
  YODA_SIXSIX_SLOG_INFO("app finished");
  return 0;
}
