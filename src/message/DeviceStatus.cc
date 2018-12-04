#include "DeviceStatus.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t DeviceStatus::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_DEVICESTATUS));
  int32_t wRstSn;
  assert(sn);
  wRstSn = caps->write(sn->c_str());
  if (wRstSn != CAPS_SUCCESS) return wRstSn;
  int32_t wRstVersion;
  assert(version);
  wRstVersion = caps->write(version->c_str());
  if (wRstVersion != CAPS_SUCCESS) return wRstVersion;
  int32_t wRstStatus = caps->write((int32_t)status);
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstShellId = caps->write((int32_t)shellId);
  if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t DeviceStatus::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_DEVICESTATUS));
  int32_t wRstSn;
  assert(sn);
  wRstSn = caps->write(sn->c_str());
  if (wRstSn != CAPS_SUCCESS) return wRstSn;
  int32_t wRstVersion;
  assert(version);
  wRstVersion = caps->write(version->c_str());
  if (wRstVersion != CAPS_SUCCESS) return wRstVersion;
  int32_t wRstStatus = caps->write((int32_t)status);
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstShellId = caps->write((int32_t)shellId);
  if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t DeviceStatus::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  if (!sn) sn = std::make_shared<std::string>();
  int32_t rRstSn = caps->read_string(*sn);
  if (rRstSn != CAPS_SUCCESS) return rRstSn;
  if (!version) version = std::make_shared<std::string>();
  int32_t rRstVersion = caps->read_string(*version);
  if (rRstVersion != CAPS_SUCCESS) return rRstVersion;
  int32_t rRstStatus = caps->read(status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  int32_t rRstShellId = caps->read(shellId);
  if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t DeviceStatus::deserialize(std::shared_ptr<Caps> &caps) {
  if (!sn) sn = std::make_shared<std::string>();
  int32_t rRstSn = caps->read_string(*sn);
  if (rRstSn != CAPS_SUCCESS) return rRstSn;
  if (!version) version = std::make_shared<std::string>();
  int32_t rRstVersion = caps->read_string(*version);
  if (rRstVersion != CAPS_SUCCESS) return rRstVersion;
  int32_t rRstStatus = caps->read(status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  int32_t rRstShellId = caps->read(shellId);
  if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t DeviceStatus::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  int32_t wRstSn;
  assert(sn);
  wRstSn = caps->write(sn->c_str());
  if (wRstSn != CAPS_SUCCESS) return wRstSn;
  int32_t wRstVersion;
  assert(version);
  wRstVersion = caps->write(version->c_str());
  if (wRstVersion != CAPS_SUCCESS) return wRstVersion;
  int32_t wRstStatus = caps->write((int32_t)status);
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstShellId = caps->write((int32_t)shellId);
  if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t DeviceStatus::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  if (!sn) sn = std::make_shared<std::string>();
  int32_t rRstSn = caps->read_string(*sn);
  if (rRstSn != CAPS_SUCCESS) return rRstSn;
  if (!version) version = std::make_shared<std::string>();
  int32_t rRstVersion = caps->read_string(*version);
  if (rRstVersion != CAPS_SUCCESS) return rRstVersion;
  int32_t rRstStatus = caps->read(status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  int32_t rRstShellId = caps->read(shellId);
  if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}

