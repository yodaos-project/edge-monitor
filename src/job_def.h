//
// Created by ximin.chen@rokid.com on 2018/11/19.
//

#ifndef YODA_SIXSIX_JOB_DEF_H
#define YODA_SIXSIX_JOB_DEF_H

#include "./common.h"

YODA_NS_BEGIN

typedef enum class TaskErrorCodes {
  NO_ERROR = 0,
  MULTI_TASK,
  NO_RESOURCE,
} TaskErrorCodes;

typedef enum class TaskStatus {
  RUNNING = 0,
  FAILED = 1,
  SUCCEED = 2,
  PREPARING = 3,
} TaskStatus;

typedef enum class DeviceStatus {
  IDLE = 0,
  RUNNING = 1,
} DeviceStatus;

typedef struct TaskInfo {
  int32_t id;
  std::shared_ptr<std::string> type;
  uint32_t shellId;
  std::shared_ptr<std::string> shell;
  std::shared_ptr<std::string> shellType;
  int64_t timestampMs;
  time_t timeoutMs;
  TaskStatus status;
  TaskErrorCodes errorCode;
} TaskInfo;

typedef enum class JobType {
  COLLECT_TOP = 0,
  COLLECT_SMAP = 1,
  SPAWN_CHILD = 2,
} JobType;

typedef enum class JobState {
  STOP = 0,
  RUNNING,
} JobState;

typedef struct JobConf {
  JobType type;
  bool isRepeat;
  int32_t loopCount;
  uint64_t interval;
  uint64_t timeout;
  bool enable;
  std::string regex;
  std::shared_ptr<void> data;
  std::shared_ptr<TaskInfo> task;
} JobConf;

YODA_NS_END

#endif //YODA_SIXSIX_JOB_DEF_H
