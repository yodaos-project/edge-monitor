#include "logger.h"
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <sys/time.h>
#include <stdlib.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

static const int file_max_count = 2;
static int file_index = 0;
static pthread_t adjust_th;
static FILE *redirect_file = NULL;
static const char* file_name = NULL;

static const char *level_colors[] = {
  "", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};

static FILE **log_files[] = {
  &stdout, &stdout, &stdout, &stderr, &stderr,
};

static size_t log_max_size = 5 * 1024;

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
  FILE *log_file = *(log_files[level]);
  const char *color;
  if (redirect_file) {
    color = "";
  } else {
    color = level_colors[level];
  }
  sprintf(fmt_buf,
    "%s%04d-%02d-%02d %02d:%02d:%02d:%03d %s%s\n",
    color, year, mon, day, hour, min, sec, ms, fmt, color
  );

  va_list args;
  va_start(args, fmt);
  vfprintf(log_file, fmt_buf, args);
  va_end(args);
}

void update_log_file() {
  if (redirect_file) {
    fclose(redirect_file);
    redirect_file = NULL;
  }
  char path[256];
  if (file_index < file_max_count) {
    ++file_index;
  } else {
    sprintf(path, "%d.%s",file_index, file_name);
    unlink(path);
    file_index = 1;
  }
  sprintf(path, "%d.%s",file_index, file_name);
  redirect_file = fopen(path, "a+");
  if(!redirect_file) {
    LOG_ERROR("open log file %s error: %s\n", path, strerror(errno));
    return;
  }
  int fd = fileno(redirect_file);
  LOG_INFO("redirect 1,2 to %s, fd: %d", path, fd);
  dup2(fd, 1);
  dup2(fd, 2);
}

void* adjust_file_size(void *data) {
  while(redirect_file) {
    fseek(redirect_file, 0L, SEEK_END);
    size_t file_size = ftell(redirect_file);
    LOG_INFO("log file size %ld", file_size);
    if (file_size >= log_max_size) {
      update_log_file();
    }
    sleep(1);
  }
  LOG_INFO("adjust thread exit");
  pthread_exit(0);
}

void set_logger_file(const char *path, size_t max_size) {
  file_name = strdup(path);
  if (max_size > 0) {
    log_max_size = max_size;
    LOG_INFO("log file size %ld", log_max_size);
  } else {
    LOG_INFO("log file using default size %ld bytes", log_max_size);
  }
  update_log_file();
  pthread_create(&adjust_th, NULL, adjust_file_size, NULL);
  pthread_detach(adjust_th);
}
