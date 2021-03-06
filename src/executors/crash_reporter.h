//
// Created by ximin.chen@rokid.com on 2018/12/31.
//

#ifndef YODA_SIXSIX_CRASH_REPORTER_H
#define YODA_SIXSIX_CRASH_REPORTER_H

#include "multi_thread_executor.h"

YODA_NS_BEGIN

class CrashReporter : public IMultiThreadExecutor {
public:
  CrashReporter();

  ~CrashReporter() override;

protected:

  void doExecute(uv_work_t *) override;

  int afterExecute(uv_work_t *, int status) override;

  void compressAndUpload(const std::string &dir, const std::string &filename);

  std::vector<std::string> _scanDir;
  std::string _uploadURL;
};

YODA_NS_END

#endif //YODA_SIXSIX_CRASH_REPORTER_H
