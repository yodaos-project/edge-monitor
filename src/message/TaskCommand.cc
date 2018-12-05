#include "TaskCommand.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t TaskCommand::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_TASKCOMMAND));
  int32_t wRstTaskId = caps->write((int32_t)taskId);
  if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
  int32_t wRstTaskType;
  assert(taskType);
  wRstTaskType = caps->write(taskType->c_str());
  if (wRstTaskType != CAPS_SUCCESS) return wRstTaskType;
  int32_t wRstShellId = caps->write((uint32_t)shellId);
  if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
  int32_t wRstShellType;
  assert(shellType);
  wRstShellType = caps->write(shellType->c_str());
  if (wRstShellType != CAPS_SUCCESS) return wRstShellType;
  int32_t wRstShellContent;
  assert(shellContent);
  wRstShellContent = caps->write(shellContent->c_str());
  if (wRstShellContent != CAPS_SUCCESS) return wRstShellContent;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t TaskCommand::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_TASKCOMMAND));
  int32_t wRstTaskId = caps->write((int32_t)taskId);
  if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
  int32_t wRstTaskType;
  assert(taskType);
  wRstTaskType = caps->write(taskType->c_str());
  if (wRstTaskType != CAPS_SUCCESS) return wRstTaskType;
  int32_t wRstShellId = caps->write((uint32_t)shellId);
  if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
  int32_t wRstShellType;
  assert(shellType);
  wRstShellType = caps->write(shellType->c_str());
  if (wRstShellType != CAPS_SUCCESS) return wRstShellType;
  int32_t wRstShellContent;
  assert(shellContent);
  wRstShellContent = caps->write(shellContent->c_str());
  if (wRstShellContent != CAPS_SUCCESS) return wRstShellContent;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t TaskCommand::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  int32_t rRstTaskId = caps->read(taskId);
  if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
  if (!taskType) taskType = std::make_shared<std::string>();
  int32_t rRstTaskType = caps->read_string(*taskType);
  if (rRstTaskType != CAPS_SUCCESS) return rRstTaskType;
  int32_t rRstShellId = caps->read(shellId);
  if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
  if (!shellType) shellType = std::make_shared<std::string>();
  int32_t rRstShellType = caps->read_string(*shellType);
  if (rRstShellType != CAPS_SUCCESS) return rRstShellType;
  if (!shellContent) shellContent = std::make_shared<std::string>();
  int32_t rRstShellContent = caps->read_string(*shellContent);
  if (rRstShellContent != CAPS_SUCCESS) return rRstShellContent;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t TaskCommand::deserialize(std::shared_ptr<Caps> &caps) {
  int32_t rRstTaskId = caps->read(taskId);
  if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
  if (!taskType) taskType = std::make_shared<std::string>();
  int32_t rRstTaskType = caps->read_string(*taskType);
  if (rRstTaskType != CAPS_SUCCESS) return rRstTaskType;
  int32_t rRstShellId = caps->read(shellId);
  if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
  if (!shellType) shellType = std::make_shared<std::string>();
  int32_t rRstShellType = caps->read_string(*shellType);
  if (rRstShellType != CAPS_SUCCESS) return rRstShellType;
  if (!shellContent) shellContent = std::make_shared<std::string>();
  int32_t rRstShellContent = caps->read_string(*shellContent);
  if (rRstShellContent != CAPS_SUCCESS) return rRstShellContent;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t TaskCommand::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  int32_t wRstTaskId = caps->write((int32_t)taskId);
  if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
  int32_t wRstTaskType;
  assert(taskType);
  wRstTaskType = caps->write(taskType->c_str());
  if (wRstTaskType != CAPS_SUCCESS) return wRstTaskType;
  int32_t wRstShellId = caps->write((uint32_t)shellId);
  if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
  int32_t wRstShellType;
  assert(shellType);
  wRstShellType = caps->write(shellType->c_str());
  if (wRstShellType != CAPS_SUCCESS) return wRstShellType;
  int32_t wRstShellContent;
  assert(shellContent);
  wRstShellContent = caps->write(shellContent->c_str());
  if (wRstShellContent != CAPS_SUCCESS) return wRstShellContent;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t TaskCommand::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  int32_t rRstTaskId = caps->read(taskId);
  if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
  if (!taskType) taskType = std::make_shared<std::string>();
  int32_t rRstTaskType = caps->read_string(*taskType);
  if (rRstTaskType != CAPS_SUCCESS) return rRstTaskType;
  int32_t rRstShellId = caps->read(shellId);
  if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
  if (!shellType) shellType = std::make_shared<std::string>();
  int32_t rRstShellType = caps->read_string(*shellType);
  if (rRstShellType != CAPS_SUCCESS) return rRstShellType;
  if (!shellContent) shellContent = std::make_shared<std::string>();
  int32_t rRstShellContent = caps->read_string(*shellContent);
  if (rRstShellContent != CAPS_SUCCESS) return rRstShellContent;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}

