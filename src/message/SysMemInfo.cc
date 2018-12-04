#include "SysMemInfo.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t SysMemInfo::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_SYSMEMINFO));
  int32_t wRstTotal = caps->write((int64_t)total);
  if (wRstTotal != CAPS_SUCCESS) return wRstTotal;
  int32_t wRstFree = caps->write((int64_t)free);
  if (wRstFree != CAPS_SUCCESS) return wRstFree;
  int32_t wRstBuffers = caps->write((int64_t)buffers);
  if (wRstBuffers != CAPS_SUCCESS) return wRstBuffers;
  int32_t wRstCached = caps->write((int64_t)cached);
  if (wRstCached != CAPS_SUCCESS) return wRstCached;
  int32_t wRstAvailable = caps->write((int64_t)available);
  if (wRstAvailable != CAPS_SUCCESS) return wRstAvailable;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t SysMemInfo::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_SYSMEMINFO));
  int32_t wRstTotal = caps->write((int64_t)total);
  if (wRstTotal != CAPS_SUCCESS) return wRstTotal;
  int32_t wRstFree = caps->write((int64_t)free);
  if (wRstFree != CAPS_SUCCESS) return wRstFree;
  int32_t wRstBuffers = caps->write((int64_t)buffers);
  if (wRstBuffers != CAPS_SUCCESS) return wRstBuffers;
  int32_t wRstCached = caps->write((int64_t)cached);
  if (wRstCached != CAPS_SUCCESS) return wRstCached;
  int32_t wRstAvailable = caps->write((int64_t)available);
  if (wRstAvailable != CAPS_SUCCESS) return wRstAvailable;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t SysMemInfo::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  int32_t rRstTotal = caps->read(total);
  if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
  int32_t rRstFree = caps->read(free);
  if (rRstFree != CAPS_SUCCESS) return rRstFree;
  int32_t rRstBuffers = caps->read(buffers);
  if (rRstBuffers != CAPS_SUCCESS) return rRstBuffers;
  int32_t rRstCached = caps->read(cached);
  if (rRstCached != CAPS_SUCCESS) return rRstCached;
  int32_t rRstAvailable = caps->read(available);
  if (rRstAvailable != CAPS_SUCCESS) return rRstAvailable;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t SysMemInfo::deserialize(std::shared_ptr<Caps> &caps) {
  int32_t rRstTotal = caps->read(total);
  if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
  int32_t rRstFree = caps->read(free);
  if (rRstFree != CAPS_SUCCESS) return rRstFree;
  int32_t rRstBuffers = caps->read(buffers);
  if (rRstBuffers != CAPS_SUCCESS) return rRstBuffers;
  int32_t rRstCached = caps->read(cached);
  if (rRstCached != CAPS_SUCCESS) return rRstCached;
  int32_t rRstAvailable = caps->read(available);
  if (rRstAvailable != CAPS_SUCCESS) return rRstAvailable;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t SysMemInfo::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  int32_t wRstTotal = caps->write((int64_t)total);
  if (wRstTotal != CAPS_SUCCESS) return wRstTotal;
  int32_t wRstFree = caps->write((int64_t)free);
  if (wRstFree != CAPS_SUCCESS) return wRstFree;
  int32_t wRstBuffers = caps->write((int64_t)buffers);
  if (wRstBuffers != CAPS_SUCCESS) return wRstBuffers;
  int32_t wRstCached = caps->write((int64_t)cached);
  if (wRstCached != CAPS_SUCCESS) return wRstCached;
  int32_t wRstAvailable = caps->write((int64_t)available);
  if (wRstAvailable != CAPS_SUCCESS) return wRstAvailable;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t SysMemInfo::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  int32_t rRstTotal = caps->read(total);
  if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
  int32_t rRstFree = caps->read(free);
  if (rRstFree != CAPS_SUCCESS) return rRstFree;
  int32_t rRstBuffers = caps->read(buffers);
  if (rRstBuffers != CAPS_SUCCESS) return rRstBuffers;
  int32_t rRstCached = caps->read(cached);
  if (rRstCached != CAPS_SUCCESS) return rRstCached;
  int32_t rRstAvailable = caps->read(available);
  if (rRstAvailable != CAPS_SUCCESS) return rRstAvailable;
  return CAPS_SUCCESS;
}

