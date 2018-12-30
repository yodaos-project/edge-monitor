//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#ifndef YODA_SIXSIX_UTIL_H
#define YODA_SIXSIX_UTIL_H

#include "def.h"

YODA_NS_BEGIN


class Util {
public:
  static std::string getFormattedTime();

  template<typename T>
  static bool lexicalCast(const std::string &s, T *p) {
    std::stringstream ss(s);
    return !(ss >> *p).fail() && (ss >> std::ws).eof();
  }

  static bool isDir(const std::string &path);

  static bool isFile(const std::string &path);

  static std::vector<std::string> getFileList(const std::string &path);

  typedef std::function<void(const char *filename)> ScanDirCallback;

  static void scanDir(const std::string &path, const ScanDirCallback &cb);

  static std::string readSmallFile(const std::string &path);

  static void replaceChar(std::string &src, const std::string &chs, char r);

  // [start, end]
  static uint32_t random(uint32_t start, uint32_t end);

  static std::string exec(const std::string &cmd);

  static time_t getTimeMS();
};

YODA_NS_END

#endif //YODA_SIXSIX_UTIL_H
