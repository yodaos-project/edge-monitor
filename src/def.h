//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#ifndef YODA_SIXSIX_DEF_H
#define YODA_SIXSIX_DEF_H

#include <string>
#include <list>
#include <vector>
#include <map>
#include <sstream>
#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <memory>
#include <functional>
#include <fstream>
#include <cmath>
#include <regex.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/resource.h>
#include <uv.h>
#include <rapidjson/document.h>
#include "logger.h"

#define YODA_NS_BEGIN namespace yoda {
#define YODA_NS_END }

#define YODA_SINGLETON_IMPL(class_name)\
  public:\
    static class_name* getInstance() {\
      static class_name *instance = nullptr;\
      if (!instance) {\
        instance = new class_name();\
      }\
      return instance;\
    }\
private:\

#define _1 std::placeholders::_1
#define _2 std::placeholders::_2

#define UV_CB_WRAP1(reqPtr, cbName, className, funcName, reqType) \
  reqPtr->data = this;\
  auto cbName = [](reqType *req){\
    auto target = (className*)req->data;\
    target->funcName(req);\
  }

#define UV_CB_WRAP2(reqPtr, cbName, className, funcName, reqType, \
  arg2Type) \
  reqPtr->data = this;\
  auto cbName = [](reqType *req, arg2Type arg2){\
    auto target = (className*)req->data;\
    target->funcName(req, arg2);\
  }

#define UV_CB_WRAP3(reqPtr, cbName, className, funcName, reqType, \
  arg2Type, arg3Type) \
  reqPtr->data = this;\
  auto cbName = [](reqType *req, arg2Type arg2, arg3Type arg3){\
    auto target = (className*)req->data;\
    target->funcName(req, arg2, arg3);\
  }

#define UV_CLOSE_HANDLE(p, className, cbName) \
  if (p && !uv_is_closing((uv_handle_t *) p)) {\
    UV_CB_WRAP1(p, cb_##p, className, cbName, uv_handle_t); \
    uv_close((uv_handle_t *) p, cb_##p); \
  }

#define YODA_SIXSIX_SAFE_DELETE(p) \
  do {\
    if (p) { \
      delete p;\
      p = nullptr; \
    } \
  } while(0)

#define YODA_SIXSIX_SAFE_FREE(p) \
  do {\
    if (p) { \
      free(p);\
      p = nullptr; \
    } \
  } while(0)

#define YODA_SIXSIX_MALLOC(type) \
  (type*)malloc(sizeof(type))

#define ASSERT(exp, ...) \
  do { if (!(exp)) { LOG_FATAL(__VA_ARGS__); assert(0); } } while (0)

#define SASSERT(exp) \
  do { if (!(exp)) { LOG_FATAL("%s", strerror(errno)); assert(0); } } while (0)

#endif //YODA_SIXSIX_DEF_H
