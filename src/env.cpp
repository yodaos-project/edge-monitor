//
// Created by ximin.chen@rokid.com on 2019-04-10.
//

#include "env.h"
#include "options.h"

YODA_NS_BEGIN

int32_t Env::setup() {
  close(0);
  open("/dev/null", O_RDWR);
  int32_t pid = getpid();
  int32_t pgid = getpgid(pid);
  pthread_atfork(nullptr, nullptr, nullptr);
  size_t logMaxSize = Options::get<size_t>("l", 0);
  LOG_INFO("log file size %ld", logMaxSize);
  set_logger_file_size(logMaxSize);
  if (Options::exists("b")) {
    pid = fork();
    ASSERT(pid >= 0, "fork self error: %d", pid);
    if (pid > 0) {
      LOG_INFO("forked child with pid %d, exiting ...", pid);
      exit(0);
    }
    setsid();
    pid = getpid();
    pgid = getpgid(pid);
    LOG_INFO("child running with pid %d, pgid %d", pid, pgid);
    if (logMaxSize == 0) {
      dup2(0, 1);
      dup2(0, 2);
    }
  }
  return 0;
}

YODA_NS_END
