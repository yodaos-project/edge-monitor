#include "MemInfos.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t MemInfos::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_MEMINFOS));
  if (!procMemInfo)
    caps->write((int32_t)0);
  else {
    caps->write((int32_t)procMemInfo->size());
    for(auto &v : *procMemInfo) {
      std::shared_ptr<Caps> c;
      int32_t sRst = v.serializeForCapsObj(c);
      if (sRst != CAPS_SUCCESS)
      return sRst;
      else {
        int32_t wRst = caps->write(c);
        if (wRst != CAPS_SUCCESS) return wRst;
      }
    }
  }
  std::shared_ptr<Caps> capsSysMem;
  assert(sysMem);
  int32_t sRstSysMem = sysMem->serializeForCapsObj(capsSysMem);
  if (sRstSysMem != CAPS_SUCCESS)
    return sRstSysMem;
  else {
      int32_t wRst = caps->write(capsSysMem);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t MemInfos::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_MEMINFOS));
  if (!procMemInfo)
    caps->write((int32_t)0);
  else {
    caps->write((int32_t)procMemInfo->size());
    for(auto &v : *procMemInfo) {
      std::shared_ptr<Caps> c;
      int32_t sRst = v.serializeForCapsObj(c);
      if (sRst != CAPS_SUCCESS)
      return sRst;
      else {
        int32_t wRst = caps->write(c);
        if (wRst != CAPS_SUCCESS) return wRst;
      }
    }
  }
  std::shared_ptr<Caps> capsSysMem;
  assert(sysMem);
  int32_t sRstSysMem = sysMem->serializeForCapsObj(capsSysMem);
  if (sRstSysMem != CAPS_SUCCESS)
    return sRstSysMem;
  else {
      int32_t wRst = caps->write(capsSysMem);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t MemInfos::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  int32_t arraySizeProcMemInfo = 0;
  int32_t rRstProcMemInfo = caps->read(arraySizeProcMemInfo);
  if (rRstProcMemInfo != CAPS_SUCCESS) return rRstProcMemInfo;
  procMemInfo->clear();
  for(int32_t i = 0; i < arraySizeProcMemInfo;++i) {
    std::shared_ptr<Caps> c;
    if (caps->read(c) == CAPS_SUCCESS && c) {
      procMemInfo->emplace_back();
      int32_t dRst = procMemInfo->back().deserializeForCapsObj(c);
      if (dRst != CAPS_SUCCESS) return dRst;
    }
  }
  std::shared_ptr<Caps> capsSysMem;
  int32_t rRstSysMem = caps->read(capsSysMem);
  if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
  if (!sysMem) sysMem = std::make_shared<SysMemInfo>();
  rRstSysMem = sysMem->deserializeForCapsObj(capsSysMem);
  if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t MemInfos::deserialize(std::shared_ptr<Caps> &caps) {
  int32_t arraySizeProcMemInfo = 0;
  int32_t rRstProcMemInfo = caps->read(arraySizeProcMemInfo);
  if (rRstProcMemInfo != CAPS_SUCCESS) return rRstProcMemInfo;
  procMemInfo->clear();
  for(int32_t i = 0; i < arraySizeProcMemInfo;++i) {
    std::shared_ptr<Caps> c;
    if (caps->read(c) == CAPS_SUCCESS && c) {
      procMemInfo->emplace_back();
      int32_t dRst = procMemInfo->back().deserializeForCapsObj(c);
      if (dRst != CAPS_SUCCESS) return dRst;
    }
  }
  std::shared_ptr<Caps> capsSysMem;
  int32_t rRstSysMem = caps->read(capsSysMem);
  if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
  if (!sysMem) sysMem = std::make_shared<SysMemInfo>();
  rRstSysMem = sysMem->deserializeForCapsObj(capsSysMem);
  if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t MemInfos::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  if (!procMemInfo)
    caps->write((int32_t)0);
  else {
    caps->write((int32_t)procMemInfo->size());
    for(auto &v : *procMemInfo) {
      std::shared_ptr<Caps> c;
      int32_t sRst = v.serializeForCapsObj(c);
      if (sRst != CAPS_SUCCESS)
      return sRst;
      else {
        int32_t wRst = caps->write(c);
        if (wRst != CAPS_SUCCESS) return wRst;
      }
    }
  }
  std::shared_ptr<Caps> capsSysMem;
  assert(sysMem);
  int32_t sRstSysMem = sysMem->serializeForCapsObj(capsSysMem);
  if (sRstSysMem != CAPS_SUCCESS)
    return sRstSysMem;
  else {
      int32_t wRst = caps->write(capsSysMem);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t MemInfos::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  int32_t arraySizeProcMemInfo = 0;
  int32_t rRstProcMemInfo = caps->read(arraySizeProcMemInfo);
  if (rRstProcMemInfo != CAPS_SUCCESS) return rRstProcMemInfo;
  procMemInfo->clear();
  for(int32_t i = 0; i < arraySizeProcMemInfo;++i) {
    std::shared_ptr<Caps> c;
    if (caps->read(c) == CAPS_SUCCESS && c) {
      procMemInfo->emplace_back();
      int32_t dRst = procMemInfo->back().deserializeForCapsObj(c);
      if (dRst != CAPS_SUCCESS) return dRst;
    }
  }
  std::shared_ptr<Caps> capsSysMem;
  int32_t rRstSysMem = caps->read(capsSysMem);
  if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
  if (!sysMem) sysMem = std::make_shared<SysMemInfo>();
  rRstSysMem = sysMem->deserializeForCapsObj(capsSysMem);
  if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}

