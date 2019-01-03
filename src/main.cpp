//
// Created by ximin.chen@rokid.com on 2018/11/19.
//
#include "job_manager.h"
#include "options.h"
#include "WebSocketClient.h"
#include "MessageCommon.h"
#include "device_info.h"

using namespace rokid;

static void parseExitCmd(int argc, char **argv);
static void makeUVHappy();

int main(int argc, char **argv) {
  setpriority(PRIO_PGRP, 0, 19);
  parseExitCmd(argc, argv);
  YODA_SIXSIX_SLOG("starting app");
  yoda::DeviceInfo::init();
  yoda::Options::parseCmdLine(argc, argv);

  WebSocketClient wsc;
  wsc.init();
  yoda::JobManager manager;
  manager.initWithWS(&wsc);

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  wsc.stop();

  makeUVHappy();

  YODA_SIXSIX_SLOG("all tasks have been completed, app exit");
  return 0;
}

static const char *helpStr =
  "Usage: \n"
  "[-version]         print version\n"
  "[-sysroot]         set sysroot\n"
  "[-taskJson]        start task from local json path\n"
  "[-disableUpload]   set none-zero to disable upload data\n"
  "[-serverAddress]   set server address\n"
  "[-serverPort]      set server port\n"
  "[-sn]              mock an sn\n"
  "[-hardware]        mock a hardware\n"
  "[-unzipRoot]       set task files unzip path\n"
  "[-smapInterval]    set smap collect interval in millisecond"
                      ", default is 3001000ms\n"
  "[-smapSleep]       set usleep time after collected a process in millisecond"
                      ", default is 1000ms\n"
  "[-bufferCount]     set max count of websocket message list, 100 by default\n"
  "[-coredumpDir]     set coredump output directory, use commas to separate\n"
  "\n\n"
  "Hints: it is recommend to set smapSleep to 200 for Rokid Glass\n"
  "                          set smapSleep to 500 for A113\n"
  "                          set smapSleep to 1000 for Rokid Kamino\n";

void parseExitCmd(int argc, char **argv) {
  if (argc >= 2) {
    const char *print = nullptr;
    if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0) {
      print = helpStr;
    } else if (strcmp(argv[1], "-version") == 0) {
      print = version;
    }
    if (print) {
      printf("%s\n", print);
      exit(0);
    }
  }
}

void makeUVHappy() {
  uv_walk(uv_default_loop(), [](uv_handle_t *handle, void *) {
    if (!uv_is_closing(handle)) {
      uv_close(handle, nullptr);
    }
  }, nullptr);
  uv_run(uv_default_loop(), UV_RUN_DEFAULT);
  int r = uv_loop_close(uv_default_loop());
  YODA_SIXSIX_FASSERT(r == 0, "uv close error: %s", uv_err_name(r));
}
