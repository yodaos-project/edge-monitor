#include "TaskStatus.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t TaskStatus::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_TASKSTATUS));
  int32_t wRstTaskId = caps->write((int32_t)taskId);
  if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
  int32_t wRstStatus = caps->write((int32_t)status);
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstMessage;
  assert(message);
  wRstMessage = caps->write(message->c_str());
  if (wRstMessage != CAPS_SUCCESS) return wRstMessage;
  int32_t wRstShellId = caps->write((int32_t)shellId);
  if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t TaskStatus::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_TASKSTATUS));
  int32_t wRstTaskId = caps->write((int32_t)taskId);
  if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
  int32_t wRstStatus = caps->write((int32_t)status);
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstMessage;
  assert(message);
  wRstMessage = caps->write(message->c_str());
  if (wRstMessage != CAPS_SUCCESS) return wRstMessage;
  int32_t wRstShellId = caps->write((int32_t)shellId);
  if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t TaskStatus::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  int32_t rRstTaskId = caps->read(taskId);
  if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
  int32_t rRstStatus = caps->read(status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  if (!message) message = std::make_shared<std::string>();
  int32_t rRstMessage = caps->read_string(*message);
  if (rRstMessage != CAPS_SUCCESS) return rRstMessage;
  int32_t rRstShellId = caps->read(shellId);
  if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t TaskStatus::deserialize(std::shared_ptr<Caps> &caps) {
  int32_t rRstTaskId = caps->read(taskId);
  if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
  int32_t rRstStatus = caps->read(status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  if (!message) message = std::make_shared<std::string>();
  int32_t rRstMessage = caps->read_string(*message);
  if (rRstMessage != CAPS_SUCCESS) return rRstMessage;
  int32_t rRstShellId = caps->read(shellId);
  if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t TaskStatus::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  int32_t wRstTaskId = caps->write((int32_t)taskId);
  if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
  int32_t wRstStatus = caps->write((int32_t)status);
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstMessage;
  assert(message);
  wRstMessage = caps->write(message->c_str());
  if (wRstMessage != CAPS_SUCCESS) return wRstMessage;
  int32_t wRstShellId = caps->write((int32_t)shellId);
  if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t TaskStatus::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  int32_t rRstTaskId = caps->read(taskId);
  if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
  int32_t rRstStatus = caps->read(status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  if (!message) message = std::make_shared<std::string>();
  int32_t rRstMessage = caps->read_string(*message);
  if (rRstMessage != CAPS_SUCCESS) return rRstMessage;
  int32_t rRstShellId = caps->read(shellId);
  if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}

