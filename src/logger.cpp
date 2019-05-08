#include "logger.h"
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>
#include "options.h"


static FILE *log_stdout = stdout;
static FILE *log_stderr = stderr;

static const char *level_colors[] = {
  "", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};

static FILE **log_fds[] {
  &log_stdout, &log_stdout, &log_stdout, &log_stderr, &log_stderr,
};

void do_log(log_level level, const char *file, int line, const char *fmt, ...) {
  struct timeval cur_time = {};
  gettimeofday(&cur_time, NULL);
  struct tm *tm = localtime(&cur_time.tv_sec);
  int year = tm->tm_year + 1900;
  int mon = tm->tm_mon + 1;
  int day = tm->tm_mday;
  int hour = tm->tm_hour;
  int min = tm->tm_min;
  int sec = tm->tm_sec;
  int ms = cur_time.tv_usec / 1000;
  size_t fmt_len = 64 + strlen(fmt);
  char fmt_buf[fmt_len];
  FILE **log_fd = log_fds[level];
  const char *color = level_colors[level];
  if (*log_fd != stdout && *log_fd != stderr) {
    color = "";
  }
  sprintf(fmt_buf,
    "%s%04d-%02d-%02d %02d:%02d:%02d:%03d %s%s\n",
    color, year, mon, day, hour, min, sec, ms, fmt, color
  );

  va_list args;
  va_start(args, fmt);
  vfprintf(*log_fd, fmt_buf, args);
  va_end(args);
}

void set_log_file(const char *stdout_path, const char *stderr_path) {
  log_stdout = fopen(stdout_path, "w+");
  if(!log_stdout) {
    SASSERT(log_stdout != NULL);
  }
  if (strcmp(stdout_path, stderr_path) == 0) {
    log_stderr = log_stdout;
  } else {
    log_stderr = fopen(stderr_path, "w+");
    SASSERT(log_stderr != NULL);
  }
}

void close_log_file() {
  fclose(log_stdout);

  fclose(log_stderr);
}
