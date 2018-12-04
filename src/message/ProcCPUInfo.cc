#include "ProcCPUInfo.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t ProcCPUInfo::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_PROCCPUINFO));
  int32_t wRstPid = caps->write((uint32_t)pid);
  if (wRstPid != CAPS_SUCCESS) return wRstPid;
  int32_t wRstStatus;
  assert(status);
  wRstStatus = caps->write(status->c_str());
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstFullName;
  assert(fullName);
  wRstFullName = caps->write(fullName->c_str());
  if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
  int32_t wRstUtime = caps->write((int64_t)utime);
  if (wRstUtime != CAPS_SUCCESS) return wRstUtime;
  int32_t wRstStime = caps->write((int64_t)stime);
  if (wRstStime != CAPS_SUCCESS) return wRstStime;
  int32_t wRstTicks = caps->write((int64_t)ticks);
  if (wRstTicks != CAPS_SUCCESS) return wRstTicks;
  int32_t wRstNice = caps->write((int32_t)nice);
  if (wRstNice != CAPS_SUCCESS) return wRstNice;
  int32_t wRstCpuUsage = caps->write((float)cpuUsage);
  if (wRstCpuUsage != CAPS_SUCCESS) return wRstCpuUsage;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t ProcCPUInfo::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_PROCCPUINFO));
  int32_t wRstPid = caps->write((uint32_t)pid);
  if (wRstPid != CAPS_SUCCESS) return wRstPid;
  int32_t wRstStatus;
  assert(status);
  wRstStatus = caps->write(status->c_str());
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstFullName;
  assert(fullName);
  wRstFullName = caps->write(fullName->c_str());
  if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
  int32_t wRstUtime = caps->write((int64_t)utime);
  if (wRstUtime != CAPS_SUCCESS) return wRstUtime;
  int32_t wRstStime = caps->write((int64_t)stime);
  if (wRstStime != CAPS_SUCCESS) return wRstStime;
  int32_t wRstTicks = caps->write((int64_t)ticks);
  if (wRstTicks != CAPS_SUCCESS) return wRstTicks;
  int32_t wRstNice = caps->write((int32_t)nice);
  if (wRstNice != CAPS_SUCCESS) return wRstNice;
  int32_t wRstCpuUsage = caps->write((float)cpuUsage);
  if (wRstCpuUsage != CAPS_SUCCESS) return wRstCpuUsage;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t ProcCPUInfo::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  int32_t rRstPid = caps->read(pid);
  if (rRstPid != CAPS_SUCCESS) return rRstPid;
  if (!status) status = std::make_shared<std::string>();
  int32_t rRstStatus = caps->read_string(*status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  if (!fullName) fullName = std::make_shared<std::string>();
  int32_t rRstFullName = caps->read_string(*fullName);
  if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
  int32_t rRstUtime = caps->read(utime);
  if (rRstUtime != CAPS_SUCCESS) return rRstUtime;
  int32_t rRstStime = caps->read(stime);
  if (rRstStime != CAPS_SUCCESS) return rRstStime;
  int32_t rRstTicks = caps->read(ticks);
  if (rRstTicks != CAPS_SUCCESS) return rRstTicks;
  int32_t rRstNice = caps->read(nice);
  if (rRstNice != CAPS_SUCCESS) return rRstNice;
  int32_t rRstCpuUsage = caps->read(cpuUsage);
  if (rRstCpuUsage != CAPS_SUCCESS) return rRstCpuUsage;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t ProcCPUInfo::deserialize(std::shared_ptr<Caps> &caps) {
  int32_t rRstPid = caps->read(pid);
  if (rRstPid != CAPS_SUCCESS) return rRstPid;
  if (!status) status = std::make_shared<std::string>();
  int32_t rRstStatus = caps->read_string(*status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  if (!fullName) fullName = std::make_shared<std::string>();
  int32_t rRstFullName = caps->read_string(*fullName);
  if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
  int32_t rRstUtime = caps->read(utime);
  if (rRstUtime != CAPS_SUCCESS) return rRstUtime;
  int32_t rRstStime = caps->read(stime);
  if (rRstStime != CAPS_SUCCESS) return rRstStime;
  int32_t rRstTicks = caps->read(ticks);
  if (rRstTicks != CAPS_SUCCESS) return rRstTicks;
  int32_t rRstNice = caps->read(nice);
  if (rRstNice != CAPS_SUCCESS) return rRstNice;
  int32_t rRstCpuUsage = caps->read(cpuUsage);
  if (rRstCpuUsage != CAPS_SUCCESS) return rRstCpuUsage;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t ProcCPUInfo::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  int32_t wRstPid = caps->write((uint32_t)pid);
  if (wRstPid != CAPS_SUCCESS) return wRstPid;
  int32_t wRstStatus;
  assert(status);
  wRstStatus = caps->write(status->c_str());
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstFullName;
  assert(fullName);
  wRstFullName = caps->write(fullName->c_str());
  if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
  int32_t wRstUtime = caps->write((int64_t)utime);
  if (wRstUtime != CAPS_SUCCESS) return wRstUtime;
  int32_t wRstStime = caps->write((int64_t)stime);
  if (wRstStime != CAPS_SUCCESS) return wRstStime;
  int32_t wRstTicks = caps->write((int64_t)ticks);
  if (wRstTicks != CAPS_SUCCESS) return wRstTicks;
  int32_t wRstNice = caps->write((int32_t)nice);
  if (wRstNice != CAPS_SUCCESS) return wRstNice;
  int32_t wRstCpuUsage = caps->write((float)cpuUsage);
  if (wRstCpuUsage != CAPS_SUCCESS) return wRstCpuUsage;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t ProcCPUInfo::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  int32_t rRstPid = caps->read(pid);
  if (rRstPid != CAPS_SUCCESS) return rRstPid;
  if (!status) status = std::make_shared<std::string>();
  int32_t rRstStatus = caps->read_string(*status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  if (!fullName) fullName = std::make_shared<std::string>();
  int32_t rRstFullName = caps->read_string(*fullName);
  if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
  int32_t rRstUtime = caps->read(utime);
  if (rRstUtime != CAPS_SUCCESS) return rRstUtime;
  int32_t rRstStime = caps->read(stime);
  if (rRstStime != CAPS_SUCCESS) return rRstStime;
  int32_t rRstTicks = caps->read(ticks);
  if (rRstTicks != CAPS_SUCCESS) return rRstTicks;
  int32_t rRstNice = caps->read(nice);
  if (rRstNice != CAPS_SUCCESS) return rRstNice;
  int32_t rRstCpuUsage = caps->read(cpuUsage);
  if (rRstCpuUsage != CAPS_SUCCESS) return rRstCpuUsage;
  return CAPS_SUCCESS;
}

