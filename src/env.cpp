//
// Created by ximin.chen@rokid.com on 2019-04-10.
//

#include "env.h"
#include "options.h"

YODA_NS_BEGIN

static void runInBackground() {
  if (!Options::exists("b")) {
    return;
  }
  int pid = fork();
  ASSERT(pid >= 0, "fork self error: %d", pid);
  if (pid > 0) {
    LOG_INFO("forked child with pid %d, exiting ...", pid);
    exit(0);
  }
  setsid();
  pid = getpid();
  int pgid = getpgid(pid);
  LOG_INFO("child running with pid %d, pgid %d", pid, pgid);
  dup2(0, 1);
  dup2(0, 2);
}

static void setupLogger() {
  std::string levelStr = Options::get<std::string>("d", "info");
  LOG_INFO("log level %s", levelStr.c_str());
  log_level level = LOG_LEVEL_INFO;
  if (levelStr == "verbose") {
    level = LOG_LEVEL_VERBOSE;
  } else if (levelStr == "warn") {
    level = LOG_LEVEL_WARN;
  } else if (levelStr == "error") {
    level = LOG_LEVEL_ERROR;
  } else if (levelStr == "fatal") {
    level = LOG_LEVEL_FATAL;
  } else if (levelStr != "info") {
    LOG_ERROR("unknown log level %s, using info level", levelStr.c_str());
  }
  set_logger_level(level);
  std::string logDirectory = Options::get<std::string>("l", "");
  if (!logDirectory.empty()) {
    set_logger_file_directory(logDirectory.c_str());
  }
}

int32_t Env::setup() {
  close(0);
  open("/dev/null", O_RDWR);
  runInBackground();
  setupLogger();
  return 0;
}

YODA_NS_END
