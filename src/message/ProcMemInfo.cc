#include "ProcMemInfo.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t ProcMemInfo::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_PROCMEMINFO));
  int32_t wRstPid = caps->write((uint32_t)pid);
  if (wRstPid != CAPS_SUCCESS) return wRstPid;
  int32_t wRstFullName;
  assert(fullName);
  wRstFullName = caps->write(fullName->c_str());
  if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
  int32_t wRstSharedClean = caps->write((int64_t)sharedClean);
  if (wRstSharedClean != CAPS_SUCCESS) return wRstSharedClean;
  int32_t wRstSharedDirty = caps->write((int64_t)sharedDirty);
  if (wRstSharedDirty != CAPS_SUCCESS) return wRstSharedDirty;
  int32_t wRstPrivateClean = caps->write((int64_t)privateClean);
  if (wRstPrivateClean != CAPS_SUCCESS) return wRstPrivateClean;
  int32_t wRstPrivateDirty = caps->write((int64_t)privateDirty);
  if (wRstPrivateDirty != CAPS_SUCCESS) return wRstPrivateDirty;
  int32_t wRstPss = caps->write((int64_t)pss);
  if (wRstPss != CAPS_SUCCESS) return wRstPss;
  int32_t wRstRss = caps->write((int64_t)rss);
  if (wRstRss != CAPS_SUCCESS) return wRstRss;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t ProcMemInfo::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_PROCMEMINFO));
  int32_t wRstPid = caps->write((uint32_t)pid);
  if (wRstPid != CAPS_SUCCESS) return wRstPid;
  int32_t wRstFullName;
  assert(fullName);
  wRstFullName = caps->write(fullName->c_str());
  if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
  int32_t wRstSharedClean = caps->write((int64_t)sharedClean);
  if (wRstSharedClean != CAPS_SUCCESS) return wRstSharedClean;
  int32_t wRstSharedDirty = caps->write((int64_t)sharedDirty);
  if (wRstSharedDirty != CAPS_SUCCESS) return wRstSharedDirty;
  int32_t wRstPrivateClean = caps->write((int64_t)privateClean);
  if (wRstPrivateClean != CAPS_SUCCESS) return wRstPrivateClean;
  int32_t wRstPrivateDirty = caps->write((int64_t)privateDirty);
  if (wRstPrivateDirty != CAPS_SUCCESS) return wRstPrivateDirty;
  int32_t wRstPss = caps->write((int64_t)pss);
  if (wRstPss != CAPS_SUCCESS) return wRstPss;
  int32_t wRstRss = caps->write((int64_t)rss);
  if (wRstRss != CAPS_SUCCESS) return wRstRss;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t ProcMemInfo::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  int32_t rRstPid = caps->read(pid);
  if (rRstPid != CAPS_SUCCESS) return rRstPid;
  if (!fullName) fullName = std::make_shared<std::string>();
  int32_t rRstFullName = caps->read_string(*fullName);
  if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
  int32_t rRstSharedClean = caps->read(sharedClean);
  if (rRstSharedClean != CAPS_SUCCESS) return rRstSharedClean;
  int32_t rRstSharedDirty = caps->read(sharedDirty);
  if (rRstSharedDirty != CAPS_SUCCESS) return rRstSharedDirty;
  int32_t rRstPrivateClean = caps->read(privateClean);
  if (rRstPrivateClean != CAPS_SUCCESS) return rRstPrivateClean;
  int32_t rRstPrivateDirty = caps->read(privateDirty);
  if (rRstPrivateDirty != CAPS_SUCCESS) return rRstPrivateDirty;
  int32_t rRstPss = caps->read(pss);
  if (rRstPss != CAPS_SUCCESS) return rRstPss;
  int32_t rRstRss = caps->read(rss);
  if (rRstRss != CAPS_SUCCESS) return rRstRss;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t ProcMemInfo::deserialize(std::shared_ptr<Caps> &caps) {
  int32_t rRstPid = caps->read(pid);
  if (rRstPid != CAPS_SUCCESS) return rRstPid;
  if (!fullName) fullName = std::make_shared<std::string>();
  int32_t rRstFullName = caps->read_string(*fullName);
  if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
  int32_t rRstSharedClean = caps->read(sharedClean);
  if (rRstSharedClean != CAPS_SUCCESS) return rRstSharedClean;
  int32_t rRstSharedDirty = caps->read(sharedDirty);
  if (rRstSharedDirty != CAPS_SUCCESS) return rRstSharedDirty;
  int32_t rRstPrivateClean = caps->read(privateClean);
  if (rRstPrivateClean != CAPS_SUCCESS) return rRstPrivateClean;
  int32_t rRstPrivateDirty = caps->read(privateDirty);
  if (rRstPrivateDirty != CAPS_SUCCESS) return rRstPrivateDirty;
  int32_t rRstPss = caps->read(pss);
  if (rRstPss != CAPS_SUCCESS) return rRstPss;
  int32_t rRstRss = caps->read(rss);
  if (rRstRss != CAPS_SUCCESS) return rRstRss;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t ProcMemInfo::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  int32_t wRstPid = caps->write((uint32_t)pid);
  if (wRstPid != CAPS_SUCCESS) return wRstPid;
  int32_t wRstFullName;
  assert(fullName);
  wRstFullName = caps->write(fullName->c_str());
  if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
  int32_t wRstSharedClean = caps->write((int64_t)sharedClean);
  if (wRstSharedClean != CAPS_SUCCESS) return wRstSharedClean;
  int32_t wRstSharedDirty = caps->write((int64_t)sharedDirty);
  if (wRstSharedDirty != CAPS_SUCCESS) return wRstSharedDirty;
  int32_t wRstPrivateClean = caps->write((int64_t)privateClean);
  if (wRstPrivateClean != CAPS_SUCCESS) return wRstPrivateClean;
  int32_t wRstPrivateDirty = caps->write((int64_t)privateDirty);
  if (wRstPrivateDirty != CAPS_SUCCESS) return wRstPrivateDirty;
  int32_t wRstPss = caps->write((int64_t)pss);
  if (wRstPss != CAPS_SUCCESS) return wRstPss;
  int32_t wRstRss = caps->write((int64_t)rss);
  if (wRstRss != CAPS_SUCCESS) return wRstRss;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t ProcMemInfo::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  int32_t rRstPid = caps->read(pid);
  if (rRstPid != CAPS_SUCCESS) return rRstPid;
  if (!fullName) fullName = std::make_shared<std::string>();
  int32_t rRstFullName = caps->read_string(*fullName);
  if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
  int32_t rRstSharedClean = caps->read(sharedClean);
  if (rRstSharedClean != CAPS_SUCCESS) return rRstSharedClean;
  int32_t rRstSharedDirty = caps->read(sharedDirty);
  if (rRstSharedDirty != CAPS_SUCCESS) return rRstSharedDirty;
  int32_t rRstPrivateClean = caps->read(privateClean);
  if (rRstPrivateClean != CAPS_SUCCESS) return rRstPrivateClean;
  int32_t rRstPrivateDirty = caps->read(privateDirty);
  if (rRstPrivateDirty != CAPS_SUCCESS) return rRstPrivateDirty;
  int32_t rRstPss = caps->read(pss);
  if (rRstPss != CAPS_SUCCESS) return rRstPss;
  int32_t rRstRss = caps->read(rss);
  if (rRstRss != CAPS_SUCCESS) return rRstRss;
  return CAPS_SUCCESS;
}

