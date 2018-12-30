//
// Created by ximin.chen@rokid.com on 2018/11/22.
//

#ifndef YODA_SIXSIX_BUSY_BOX_H
#define YODA_SIXSIX_BUSY_BOX_H

#include "common.h"
#include "executor_def.h"

// from https://github.com/mirror/busybox
namespace busybox {

std::shared_ptr<SystemCPUDetailInfo> getCPUTop(const std::string &dir);

std::shared_ptr<ProcessTopInfo> getProcessTop(const std::string &dir,
                                              uint32_t pid);

std::shared_ptr<ProcessSmapInfo> getProcessSmap(const std::string &dir,
                                                uint32_t pid);

std::shared_ptr<SystemMemoryInfo> getSystemMemory(const std::string &dir);

std::shared_ptr<SystemTopInfo> getSystemTop(const std::string &dir);

}

#endif //YODA_SIXSIX_BUSY_BOX_H
