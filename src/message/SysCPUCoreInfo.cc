#include "SysCPUCoreInfo.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t SysCPUCoreInfo::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_SYSCPUCOREINFO));
  int32_t wRstUsrUsage = caps->write((float)usrUsage);
  if (wRstUsrUsage != CAPS_SUCCESS) return wRstUsrUsage;
  int32_t wRstSysUsage = caps->write((float)sysUsage);
  if (wRstSysUsage != CAPS_SUCCESS) return wRstSysUsage;
  int32_t wRstIdleUsage = caps->write((float)idleUsage);
  if (wRstIdleUsage != CAPS_SUCCESS) return wRstIdleUsage;
  int32_t wRstIoUsage = caps->write((float)ioUsage);
  if (wRstIoUsage != CAPS_SUCCESS) return wRstIoUsage;
  int32_t wRstBusyUsage = caps->write((float)busyUsage);
  if (wRstBusyUsage != CAPS_SUCCESS) return wRstBusyUsage;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t SysCPUCoreInfo::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_SYSCPUCOREINFO));
  int32_t wRstUsrUsage = caps->write((float)usrUsage);
  if (wRstUsrUsage != CAPS_SUCCESS) return wRstUsrUsage;
  int32_t wRstSysUsage = caps->write((float)sysUsage);
  if (wRstSysUsage != CAPS_SUCCESS) return wRstSysUsage;
  int32_t wRstIdleUsage = caps->write((float)idleUsage);
  if (wRstIdleUsage != CAPS_SUCCESS) return wRstIdleUsage;
  int32_t wRstIoUsage = caps->write((float)ioUsage);
  if (wRstIoUsage != CAPS_SUCCESS) return wRstIoUsage;
  int32_t wRstBusyUsage = caps->write((float)busyUsage);
  if (wRstBusyUsage != CAPS_SUCCESS) return wRstBusyUsage;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t SysCPUCoreInfo::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  int32_t rRstUsrUsage = caps->read(usrUsage);
  if (rRstUsrUsage != CAPS_SUCCESS) return rRstUsrUsage;
  int32_t rRstSysUsage = caps->read(sysUsage);
  if (rRstSysUsage != CAPS_SUCCESS) return rRstSysUsage;
  int32_t rRstIdleUsage = caps->read(idleUsage);
  if (rRstIdleUsage != CAPS_SUCCESS) return rRstIdleUsage;
  int32_t rRstIoUsage = caps->read(ioUsage);
  if (rRstIoUsage != CAPS_SUCCESS) return rRstIoUsage;
  int32_t rRstBusyUsage = caps->read(busyUsage);
  if (rRstBusyUsage != CAPS_SUCCESS) return rRstBusyUsage;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t SysCPUCoreInfo::deserialize(std::shared_ptr<Caps> &caps) {
  int32_t rRstUsrUsage = caps->read(usrUsage);
  if (rRstUsrUsage != CAPS_SUCCESS) return rRstUsrUsage;
  int32_t rRstSysUsage = caps->read(sysUsage);
  if (rRstSysUsage != CAPS_SUCCESS) return rRstSysUsage;
  int32_t rRstIdleUsage = caps->read(idleUsage);
  if (rRstIdleUsage != CAPS_SUCCESS) return rRstIdleUsage;
  int32_t rRstIoUsage = caps->read(ioUsage);
  if (rRstIoUsage != CAPS_SUCCESS) return rRstIoUsage;
  int32_t rRstBusyUsage = caps->read(busyUsage);
  if (rRstBusyUsage != CAPS_SUCCESS) return rRstBusyUsage;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t SysCPUCoreInfo::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  int32_t wRstUsrUsage = caps->write((float)usrUsage);
  if (wRstUsrUsage != CAPS_SUCCESS) return wRstUsrUsage;
  int32_t wRstSysUsage = caps->write((float)sysUsage);
  if (wRstSysUsage != CAPS_SUCCESS) return wRstSysUsage;
  int32_t wRstIdleUsage = caps->write((float)idleUsage);
  if (wRstIdleUsage != CAPS_SUCCESS) return wRstIdleUsage;
  int32_t wRstIoUsage = caps->write((float)ioUsage);
  if (wRstIoUsage != CAPS_SUCCESS) return wRstIoUsage;
  int32_t wRstBusyUsage = caps->write((float)busyUsage);
  if (wRstBusyUsage != CAPS_SUCCESS) return wRstBusyUsage;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t SysCPUCoreInfo::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  int32_t rRstUsrUsage = caps->read(usrUsage);
  if (rRstUsrUsage != CAPS_SUCCESS) return rRstUsrUsage;
  int32_t rRstSysUsage = caps->read(sysUsage);
  if (rRstSysUsage != CAPS_SUCCESS) return rRstSysUsage;
  int32_t rRstIdleUsage = caps->read(idleUsage);
  if (rRstIdleUsage != CAPS_SUCCESS) return rRstIdleUsage;
  int32_t rRstIoUsage = caps->read(ioUsage);
  if (rRstIoUsage != CAPS_SUCCESS) return rRstIoUsage;
  int32_t rRstBusyUsage = caps->read(busyUsage);
  if (rRstBusyUsage != CAPS_SUCCESS) return rRstBusyUsage;
  return CAPS_SUCCESS;
}

