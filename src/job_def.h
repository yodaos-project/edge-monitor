//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#ifndef YODA_SIXSIX_JOB_DEF_H
#define YODA_SIXSIX_JOB_DEF_H

#include "def.h"

YODA_NS_BEGIN

typedef enum class TaskStatus {
  RUNNING = 0,
  FAILED = 1,
  SUCCEED = 2,
  READY = 3,
  USER_CANCEL = 4,
  MULTI_TASK_CANCEL = 5,
} TaskStatus;

typedef enum class JobType {
  COLLECT_TOP = 0,
  COLLECT_SMAP = 1,
  SPAWN_CHILD = 2,
  CRASH_REPORTER = 3,
  COLLECT_BATTERY = 4
} JobType;

typedef enum class JobState {
  STOP = 0,
  RUNNING,
  FAILED,
} JobState;

typedef struct TaskInfo {
  int32_t id;
  std::shared_ptr<std::string> type;
  uint32_t shellId;
  std::shared_ptr<std::string> shell;
  std::shared_ptr<std::string> shellType;
  int64_t timestampMs;
  TaskStatus status;
} TaskInfo;

typedef struct JobConf {
  JobType type;
  bool isRepeat;
  uint32_t loopCount;
  uint64_t interval;
  uint64_t timeout;
  bool enable;
  std::string regex;
  std::shared_ptr<void> data;
  std::shared_ptr<TaskInfo> task;
} JobConf;

YODA_NS_END

#endif //YODA_SIXSIX_JOB_DEF_H
