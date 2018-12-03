//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#include "./util.h"
#include "util.h"
#include <random>

YODA_NS_BEGIN

std::string Util::getFormattedTime() {
  std::string buf(30, '\0');
  timeval curTime = {};
  gettimeofday(&curTime, nullptr);
  tm *tm = localtime(&curTime.tv_sec);
  int32_t year = tm->tm_year + 1900;
  int32_t mon = tm->tm_mon + 1;
  int32_t day = tm->tm_mday;
  int32_t hour = tm->tm_hour;
  int32_t min = tm->tm_min;
  int32_t sec = tm->tm_sec;
  auto ms = int32_t(curTime.tv_usec / 1000);
  sprintf(&buf[0],
          "%d-%d-%d %02d:%02d:%02d:%03d",
          year, mon, day, hour, min, sec, ms
  );
  return buf;
}

bool Util::isDir(const std::string &path) {
  struct stat buf = {};
  stat(path.c_str(), &buf);
  return S_ISDIR(buf.st_mode);
}

bool Util::isFile(const std::string &path) {
  struct stat buf = {};
  stat(path.c_str(), &buf);
  return S_ISREG(buf.st_mode);
}

std::vector<std::string> Util::getFileList(const std::string &path) {
  std::vector<std::string> files;
  DIR *dir = opendir(path.c_str());
  if (!dir) {
    return files;
  }
  dirent *ent;
  while ((ent = readdir(dir))) {
    if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
      files.emplace_back(ent->d_name);
    }
  }
  closedir(dir);
  return files;
}

std::string Util::readSmallFile(const std::string &path) {
  std::ifstream ifs(path);
  if (!ifs.is_open()) {
    return "";
  }
  std::string str((std::istreambuf_iterator<char>(ifs)),
                  std::istreambuf_iterator<char>());
  return str;
}

void Util::scanDir(const std::string &path, const Util::ScanDirCallback &cb) {
  DIR *dir = opendir(path.c_str());
  if (!dir) {
    return;
  }
  dirent *ent;
  while ((ent = readdir(dir))) {
    cb(ent->d_name);
  }
  closedir(dir);
}

void Util::replaceChar(std::string &src, const std::string &chs, char r) {
  for (auto &c : src) {
    if (chs.find(c) != std::string::npos) {
      c = r;
    }
  }
}

uint32_t Util::random(uint32_t start, uint32_t end) {
  std::random_device rd;
  std::mt19937 rng(rd());
  std::uniform_int_distribution<std::mt19937::result_type> dist(start, end);
  return dist(rng);
}

std::string Util::exec(const std::string &cmd) {
  static const int32_t bufSize = 1024;
  char buffer[bufSize];
  std::string result;
  FILE *pipe = popen(cmd.c_str(), "r");
  if (!pipe) {
    return result;
  }
  while (fgets(buffer, bufSize, pipe) != nullptr) {
    result += buffer;
  }
  pclose(pipe);
  return result;
}

time_t Util::getTimeMS() {
  timeval tv{0};
  gettimeofday(&tv, nullptr);
  time_t ms = (tv.tv_sec) * 1000 + (tv.tv_usec) / 1000;
  return ms;
}

//const int indexFullNameStart = 49;
//const int indexFullNameSearchStart = 39;
//int Util::getFullName(FILE *f, int posStart, std::string &fullName) {
//  if (fseek(f, posStart + indexFullNameSearchStart, SEEK_SET) == 0) {
//    int c;
//    int count = indexFullNameSearchStart;
//    fullName = "";
//    //find start
//    while(true) {
//      count++;
//      c = fgetc(f);
//      if (c == -1)
//        return -1;
//      if (c == '\n') {
//        return count;
//      }
//      else {
//        if (c == '[')
//          break;
//        else if (c == '/') {
//          fullName.push_back('/');
//          break;
//        }
//      }
//    }
//    //find end
//    bool isEnd = false;
//    while(true) {
//      count++;
//      c = fgetc(f);
//      if (c == -1)
//        return -1;
//      if (c == '\n') {
//        return count;
//      }
//      else {
//        if (c == ']')
//          isEnd = true;
//        else if (!isEnd) {
//          fullName.push_back((char)c);
//        }
//      }
//    }
//  } else
//    return -1;
//}
//
//const int indexEndOfNum = 24;
//int Util::getNum(FILE *f, int posStart) {
//  if (fseek(f, posStart + indexEndOfNum - 1, SEEK_SET) == 0) {
//    int c;
//    int num = 0;
//    int p = 1;
//    //find start
//    while (true) {
//      c = fgetc(f);
//      if (c == -1)
//        return -1;
//      else if (c == ' ')
//        return num;
//      else {
//        if (std::isdigit(c)) {
//          num += (c - '0')*p;
//          p *= 10;
//          fseek(f, -2, SEEK_CUR);
//        } else
//          return -1;
//      }
//    }
//  }
//  return -1;
//}


YODA_NS_END
