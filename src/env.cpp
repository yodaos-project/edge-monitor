//
// Created by ximin.chen@rokid.com on 2019-04-10.
//

#include "env.h"
#include "options.h"

YODA_NS_BEGIN

int32_t Env::setup() {
  pthread_atfork(nullptr, nullptr, nullptr);
  if (Options::exists("i")) {
    // ...
  } else if (Options::exists("b")) {
    int32_t pid = fork();
    ASSERT(pid >= 0, "fork self error: %d", pid);
    if (pid > 0) {
      LOG_INFO("forked child with pid %d, parent exiting ...", pid);
      exit(0);
    }
    LOG_INFO("forked child running with pid %d", getpid());
    setsid();
    close(0);
    open("/dev/null", O_RDWR);
    dup2(0, 1);
    dup2(0, 2);
  }
  return 0;
}

YODA_NS_END
