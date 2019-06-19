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

static const char *version = "v1.3.0";

static void parseExitCmd(int argc, char **argv);
static void makeUVHappy();

int main(int argc, char **argv) {
  setpriority(PRIO_PGRP, 0, 19);
  parseExitCmd(argc, argv);
  LOG_INFO("starting app");
  yoda::Options::parseCmdLine(argc, argv);
  yoda::Env::setup();
  yoda::DeviceInfo::init();

  WebSocketClient *wsc = new WebSocketClient();
  wsc->init();
  yoda::JobManager *manager = new yoda::JobManager();
  manager->startMonitor();
  manager->setWsClient(wsc);

  uv_run(uv_default_loop(), UV_RUN_DEFAULT);

  wsc->stop();

  makeUVHappy();

  delete wsc;
  delete manager;
  LOG_INFO("all tasks have been completed, app exit");
  return 0;
}

static const char *helpStr =
  "Usage:                \n"
  "[-v]     print version\n"
  "[-b]     running in background\n"
  "[-l]     set log file directory to write log to files\n"
  "[-conf]  https://github.com/yodaos-project/edge-monitor#Configure-json-structure\n";

void parseExitCmd(int argc, char **argv) {
  if (argc >= 2) {
    const char *print = nullptr;
    if (strcmp(argv[1], "-help") == 0 || strcmp(argv[1], "-h") == 0) {
      print = helpStr;
    } else if (strcmp(argv[1], "-version") == 0 || strcmp(argv[1], "-v") == 0) {
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
  ASSERT(r == 0, "uv close error: %s", uv_err_name(r));
}
