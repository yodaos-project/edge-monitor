#include "logger.h"
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

static const char *level_colors[] = {
  "", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
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
  const char *color = level_colors[level];
  sprintf(fmt_buf,
    "%s%04d-%02d-%02d %02d:%02d:%02d:%03d %s%s\n",
    color, year, mon, day, hour, min, sec, ms, fmt, color
  );

  va_list args;
  va_start(args, fmt);
  vfprintf(stdout, fmt_buf, args);
  va_end(args);
}