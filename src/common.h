//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#ifndef YODA_SIXSIX_COMMON_H
#define YODA_SIXSIX_COMMON_H

#include "./util.h"
#include "./CapsMsg.h"

#ifndef ENABLE_LOG

#define YODA_SIXSIX_SASSERT(exp, msg) \
  do { \
    if (!(exp)) {\
      fprintf(stderr, "[%s][assert] %s:%d: %s\n", \
      yoda::Util::getFormattedTime().c_str(), __FILE__, __LINE__, msg); \
      std::abort();\
    }\
  } while (0)

#define YODA_SIXSIX_FASSERT(exp, fmt, ...) \
  do { \
    if (!(exp)) {\
      fprintf(stderr, "[%s][assert] %s:%d: " fmt "\n", \
      yoda::Util::getFormattedTime().c_str(), __FILE__, \
      __LINE__, __VA_ARGS__); \
      std::abort();\
    }\
  } while (0)

#define YODA_SIXSIX_FLOG(fd, type, fmt, ...) \
  do { \
    fprintf(fd, "[%s][" type "]: " fmt "\n", \
    yoda::Util::getFormattedTime().c_str(), __VA_ARGS__); \
  } while (0)
#define YODA_SIXSIX_FLOG_INFO(fmt, ...) \
  YODA_SIXSIX_FLOG(stdout, "info", fmt, __VA_ARGS__)
#define YODA_SIXSIX_FLOG_ERROR(fmt, ...) \
  YODA_SIXSIX_FLOG(stderr, "error", fmt, __VA_ARGS__)

#define YODA_SIXSIX_SLOG(fd, type, str) \
  do { \
    fprintf(fd, "[%s][" type "]: %s\n", \
    yoda::Util::getFormattedTime().c_str(), str); \
  } while (0)
#define YODA_SIXSIX_SLOG_INFO(str) YODA_SIXSIX_SLOG(stdout, "info", str)
#define YODA_SIXSIX_SLOG_ERROR(str) YODA_SIXSIX_SLOG(stderr, "error", str)

#else
#define LOG_F
#define LOG_FINFO
#define LOG_FERROR
#define LOG_S
#define LOG_SINFO
#define LOG_SERROR

#endif

#endif //YODA_SIXSIX_COMMON_H
