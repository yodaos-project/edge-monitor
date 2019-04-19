//
// Created by ximin.chen@rokid.com on 2018/11/19.
//
#include "job_manager.h"
#include "options.h"
#include "WebSocketClient.h"
#include "MessageCommon.h"
#include "device_info.h"
#include "env.h"

using namespace rokid;

static void parseExitCmd(int argc, char **argv);
static void makeUVHappy();

int main(int argc, char **argv) {
  setpriority(PRIO_PGRP, 0, 19);
  parseExitCmd(argc, argv);
  YODA_SIXSIX_SLOG("starting app");
  yoda::Env::setup();
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
  "[-conf]            set configure json, Please refer to"
                      " https://github.com/yodaos-project/edge-monitor#Configure-json-structure"
                      " for details\n";

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
