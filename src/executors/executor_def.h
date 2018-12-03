//
// Created by ximin.chen@rokid.com on 2018/11/22.
//

#ifndef YODA_SIXSIX_EXECUTOR_DEF_H
#define YODA_SIXSIX_EXECUTOR_DEF_H

#include "../job_i_executor.h"

struct ProcessSmapInfo {
  uint32_t pid;
  std::string fullname;
  uint64_t rw;
  uint64_t ro;
  uint64_t shared_clean;
  uint64_t shared_dirty;
  uint64_t private_clean;
  uint64_t private_dirty;
  uint64_t stack;
  uint64_t pss;
  uint64_t swap;
  uint64_t rss;
  uint64_t size;
  uint64_t start;
  char smap_mode[5];
  std::map<std::string, std::shared_ptr<ProcessSmapInfo>> sections;
};

struct ProcessTopInfo {
  uint32_t pid;
  char state[4];
  std::string comm;
  std::string cmdline;
  std::string fullname;
  uint32_t ppid;
  uint32_t pgid;
  uint32_t sid;
  uint32_t ttyMinor;
  uint32_t ttyMajor;
  uint64_t utime;
  uint64_t stime;
  uint64_t ticks;
  uint64_t startTime;
  uint64_t vsz;
  uint64_t rss;
  int32_t nice;
  int32_t lastSeenOnCPU;
  uint64_t ticksDelta;
  float cpuUsagePercent;
};

struct SystemCPUInfo {
  uint64_t usr;
  float usrPercent;
  uint64_t nic;
  uint64_t sys;
  float sysPercent;
  uint64_t idle;
  float idlePercent;
  uint64_t iowait;
  float iowaitPercent;
  uint64_t irq;
  uint64_t softirq;
  uint64_t steal;
  uint64_t total;
  uint64_t totalDelta;
  uint64_t busy;
  uint64_t busyDelta;
  uint64_t processTickTotal;
  float busyPercent;
};

struct SystemCPUDetailInfo {
  std::shared_ptr<SystemCPUInfo> total;
  std::vector<std::shared_ptr<SystemCPUInfo>> cores;
};

struct SystemTopInfo {
  std::shared_ptr<SystemCPUDetailInfo> cpu;
  std::map<int32_t, std::shared_ptr<ProcessTopInfo>> processes;
};

struct SystemMemoryInfo {
  uint64_t total;
  uint64_t free;
  uint64_t buffers;
  uint64_t available;
  uint64_t cached;
};

#endif //YODA_SIXSIX_EXECUTOR_DEF_H
