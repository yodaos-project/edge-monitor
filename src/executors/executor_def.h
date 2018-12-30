//
// Created by ximin.chen@rokid.com on 2018/11/22.
//

#ifndef YODA_SIXSIX_EXECUTOR_DEF_H
#define YODA_SIXSIX_EXECUTOR_DEF_H

#include "job_i_executor.h"

struct ProcessSmapInfo {
  uint32_t pid = 0;
  std::string comm;
  std::string cmdline;
  std::string fullname;
  uint64_t rw = 0;
  uint64_t ro = 0;
  uint64_t shared_clean = 0;
  uint64_t shared_dirty = 0;
  uint64_t private_clean = 0;
  uint64_t private_dirty = 0;
  uint64_t stack = 0;
  uint64_t pss = 0;
  uint64_t swap = 0;
  uint64_t rss = 0;
  uint64_t size = 0;
  uint64_t start = 0;
  char smap_mode[5] = {0};
  std::map<std::string, std::shared_ptr<ProcessSmapInfo>> sections;
};

struct ProcessTopInfo {
  uint32_t pid = 0;
  char state[4];
  std::string comm;
  std::string cmdline;
  std::string fullname;
  uint32_t ppid = 0;
  uint32_t pgid = 0;
  uint32_t sid = 0;
  uint32_t ttyMinor = 0;
  uint32_t ttyMajor = 0;
  uint64_t utime = 0;
  uint64_t stime = 0;
  uint64_t ticks = 0;
  uint64_t startTime = 0;
  uint64_t vsz = 0;
  uint64_t rss = 0;
  int32_t nice = 0;
  int32_t lastSeenOnCPU = 0;
  uint64_t ticksDelta = 0;
  float cpuUsagePercent = 0.0f;
};

struct SystemCPUInfo {
  uint64_t usr = 0;
  float usrPercent = 0.0f;
  uint64_t nic = 0;
  uint64_t sys = 0;
  float sysPercent = 0.0f;
  uint64_t idle = 0;
  float idlePercent = 0.0f;
  uint64_t iowait = 0;
  float iowaitPercent = 0.0f;
  uint64_t irq = 0;
  uint64_t softirq = 0;
  uint64_t steal = 0;
  uint64_t total = 0;
  uint64_t totalDelta = 0;
  uint64_t busy = 0;
  uint64_t busyDelta = 0;
  uint64_t processTickTotal = 0;
  float busyPercent = 0;
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
  uint64_t total = 0;
  uint64_t free = 0;
  uint64_t buffers = 0;
  uint64_t available = 0;
  uint64_t cached = 0;
};

#endif //YODA_SIXSIX_EXECUTOR_DEF_H
