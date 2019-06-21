#include "logger.h"
#include <string.h>
#include <time.h>
#include <stdarg.h>
#include <sys/time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>

static const int file_max_count = 3;
static int file_index = 0;
static pthread_t adjust_th;
static FILE *redirect_file = NULL;
static char* log_file_directory = NULL;
static log_level min_level = LOG_LEVEL_VERBOSE;

static const char *level_colors[] = {
  "\x1b[0m", "\x1b[32m", "\x1b[33m", "\x1b[31m", "\x1b[35m"
};

static const char *level_des[] = {
  "verbose", "info", "warn", "error", "fatal"
};

static FILE **log_files[] = {
  &stdout, &stdout, &stdout, &stderr, &stderr,
};

static size_t log_max_size = 2 * 1024 * 1024;

void do_log(log_level level, const char *file, int line, const char *fmt, ...) {
  if (level < min_level) {
    return;
  }
  if (level < LOG_LEVEL_VERBOSE || level > LOG_LEVEL_FATAL) {
    level = LOG_LEVEL_INFO;
  }
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
  const char *start_color;
  const char *end_color;
  const char *des = level_des[level];
  if (redirect_file) {
    start_color = "";
    end_color = "";
  } else {
    start_color = level_colors[level];
    end_color = level_colors[0];
  }
  sprintf(fmt_buf,
    "%s%04d-%02d-%02d %02d:%02d:%02d:%03d [%s] %s%s\n",
    start_color, year, mon, day, hour, min, sec, ms, des, fmt, end_color
  );

  va_list args;
  va_start(args, fmt);
  vfprintf(log_file, fmt_buf, args);
  va_end(args);
}

void get_file_name(char *path, int index) {
  sprintf(path, "%s/yoda666.%d.log", log_file_directory, index);
}

void update_log_file() {
  if (file_index < file_max_count) {
    ++file_index;
  } else {
    char old_name[256];
    char new_name[256];
    // remove first log file
    get_file_name(old_name, 1);
    unlink(old_name);
    for (int i = 2; i <= file_max_count; ++i) {
      get_file_name(old_name, i);
      get_file_name(new_name, i - 1);
      rename(old_name, new_name);
    }
  }
  char path[256];
  get_file_name(path, file_index);
  FILE *new_redirect_file = fopen(path, "a+");
  if(!new_redirect_file) {
    LOG_ERROR("open log file %s error: %s\n", path, strerror(errno));
    return;
  }
  int fd = fileno(new_redirect_file);
  LOG_INFO("redirect 1,2 to %s, fd: %d", path, fd);
  dup2(fd, 1);
  dup2(fd, 2);
  redirect_file = new_redirect_file;
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

void set_logger_file_directory(const char *directory) {
  if (directory) {
    if (access(directory, W_OK | R_OK) != 0) {
      if (errno == ENOENT) {
        LOG_INFO("create log directory %s", directory);
        if (mkdir(directory, S_IRWXU) != 0) {
          const char *err = strerror(errno);
          LOG_FATAL("unable to create log directory %s: %s", directory, err);
          abort();
        }
      } else {
        const char *err = strerror(errno);
        LOG_FATAL("unable to read/write log directory %s: %s", directory, err);
        abort();
      }
    }
    log_file_directory = strdup(directory);
    LOG_INFO("log file directory %s", directory);
    update_log_file();
    pthread_create(&adjust_th, NULL, adjust_file_size, NULL);
    pthread_detach(adjust_th);
  }
}

void set_logger_level(log_level level) {
  min_level = level;
}
