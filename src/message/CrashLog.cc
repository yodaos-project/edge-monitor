#include "CrashLog.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t CrashLog::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_CRASHLOG));
  int32_t wRstPid = caps->write((int32_t)pid);
  if (wRstPid != CAPS_SUCCESS) return wRstPid;
  int32_t wRstName;
  assert(pName);
  wRstName = caps->write(pName->c_str());
  if (wRstName != CAPS_SUCCESS) return wRstName;
  int32_t wRstArgs;
  assert(args);
  wRstArgs = caps->write(args->c_str());
  if (wRstArgs != CAPS_SUCCESS) return wRstArgs;
  int32_t wRstRunTime = caps->write((uint64_t)runTime);
  if (wRstRunTime != CAPS_SUCCESS) return wRstRunTime;
  int32_t wRstTs = caps->write((uint64_t)ts);
  if (wRstTs != CAPS_SUCCESS) return wRstTs;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t CrashLog::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_CRASHLOG));
  int32_t wRstPid = caps->write((int32_t)pid);
  if (wRstPid != CAPS_SUCCESS) return wRstPid;
  int32_t wRstName;
  assert(pName);
  wRstName = caps->write(pName->c_str());
  if (wRstName != CAPS_SUCCESS) return wRstName;
  int32_t wRstArgs;
  assert(args);
  wRstArgs = caps->write(args->c_str());
  if (wRstArgs != CAPS_SUCCESS) return wRstArgs;
  int32_t wRstRunTime = caps->write((uint64_t)runTime);
  if (wRstRunTime != CAPS_SUCCESS) return wRstRunTime;
  int32_t wRstTs = caps->write((uint64_t)ts);
  if (wRstTs != CAPS_SUCCESS) return wRstTs;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t CrashLog::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  int32_t rRstPid = caps->read(pid);
  if (rRstPid != CAPS_SUCCESS) return rRstPid;
  if (!pName) pName = std::make_shared<std::string>();
  int32_t rRstName = caps->read_string(*pName);
  if (rRstName != CAPS_SUCCESS) return rRstName;
  if (!args) args = std::make_shared<std::string>();
  int32_t rRstArgs = caps->read_string(*args);
  if (rRstArgs != CAPS_SUCCESS) return rRstArgs;
  int32_t rRstRunTime = caps->read(runTime);
  if (rRstRunTime != CAPS_SUCCESS) return rRstRunTime;
  int32_t rRstTs = caps->read(ts);
  if (rRstTs != CAPS_SUCCESS) return rRstTs;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t CrashLog::deserialize(std::shared_ptr<Caps> &caps) {
  int32_t rRstPid = caps->read(pid);
  if (rRstPid != CAPS_SUCCESS) return rRstPid;
  if (!pName) pName = std::make_shared<std::string>();
  int32_t rRstName = caps->read_string(*pName);
  if (rRstName != CAPS_SUCCESS) return rRstName;
  if (!args) args = std::make_shared<std::string>();
  int32_t rRstArgs = caps->read_string(*args);
  if (rRstArgs != CAPS_SUCCESS) return rRstArgs;
  int32_t rRstRunTime = caps->read(runTime);
  if (rRstRunTime != CAPS_SUCCESS) return rRstRunTime;
  int32_t rRstTs = caps->read(ts);
  if (rRstTs != CAPS_SUCCESS) return rRstTs;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t CrashLog::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  int32_t wRstPid = caps->write((int32_t)pid);
  if (wRstPid != CAPS_SUCCESS) return wRstPid;
  int32_t wRstName;
  assert(pName);
  wRstName = caps->write(pName->c_str());
  if (wRstName != CAPS_SUCCESS) return wRstName;
  int32_t wRstArgs;
  assert(args);
  wRstArgs = caps->write(args->c_str());
  if (wRstArgs != CAPS_SUCCESS) return wRstArgs;
  int32_t wRstRunTime = caps->write((uint64_t)runTime);
  if (wRstRunTime != CAPS_SUCCESS) return wRstRunTime;
  int32_t wRstTs = caps->write((uint64_t)ts);
  if (wRstTs != CAPS_SUCCESS) return wRstTs;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t CrashLog::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  int32_t rRstPid = caps->read(pid);
  if (rRstPid != CAPS_SUCCESS) return rRstPid;
  if (!pName) pName = std::make_shared<std::string>();
  int32_t rRstName = caps->read_string(*pName);
  if (rRstName != CAPS_SUCCESS) return rRstName;
  if (!args) args = std::make_shared<std::string>();
  int32_t rRstArgs = caps->read_string(*args);
  if (rRstArgs != CAPS_SUCCESS) return rRstArgs;
  int32_t rRstRunTime = caps->read(runTime);
  if (rRstRunTime != CAPS_SUCCESS) return rRstRunTime;
  int32_t rRstTs = caps->read(ts);
  if (rRstTs != CAPS_SUCCESS) return rRstTs;
  return CAPS_SUCCESS;
}

