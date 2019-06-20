//
// Created by ximin.chen@rokid.com on 2018-11-28.
//

#ifndef YODA_SIXSIX_CHILD_PROCESS_H
#define YODA_SIXSIX_CHILD_PROCESS_H

#include "executor_def.h"

YODA_NS_BEGIN

class ChildProcess : public IJobExecutor {
public:
  ChildProcess() = delete;

  explicit ChildProcess(const std::shared_ptr<JobConf> &conf);

  ~ChildProcess() override;

  void execute() override;

  int stop() override;

private:

  void onChildProcessExit(uv_process_t *req, int64_t exitStatus, int32_t sig);

  void onUVHandleClosed(uv_handle_t *handle);

  uv_process_t *_cp;
  std::shared_ptr<JobConf> _conf;
  char _filePath[128];
  int _code;
};

YODA_NS_END

#endif //YODA_SIXSIX_CHILD_PROCESS_H
