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

#define YODA_SIXSIX_FWRITE(fd, type, fmt, ...) \
  do { \
    fprintf(fd, "[%s][" type "]: " fmt "\n", \
    yoda::Util::getFormattedTime().c_str(), __VA_ARGS__); \
  } while (0)
#define YODA_SIXSIX_FLOG(fmt, ...) \
  YODA_SIXSIX_FWRITE(stdout, "info", fmt, __VA_ARGS__)
#define YODA_SISIX_FERROR(fmt, ...) \
  YODA_SIXSIX_FWRITE(stderr, "error", fmt, __VA_ARGS__)

#define YODA_SIXSIX_SWRITE(fd, type, str) \
  do { \
    fprintf(fd, "[%s][" type "]: %s\n", \
    yoda::Util::getFormattedTime().c_str(), str); \
  } while (0)
#define YODA_SIXSIX_SLOG(str) YODA_SIXSIX_SWRITE(stdout, "info", str)
#define YODA_SIXSIX_SERROR(str) YODA_SIXSIX_SWRITE(stderr, "error", str)

#else
#define LOG_F
#define LOG_FINFO
#define LOG_FERROR
#define LOG_S
#define LOG_SINFO
#define LOG_SERROR

#endif

#endif //YODA_SIXSIX_COMMON_H
