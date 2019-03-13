#include "SysCPUInfo.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t SysCPUInfo::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_SYSCPUINFO));
  std::shared_ptr<Caps> capsTotal;
  assert(total);
  int32_t sRstTotal = total->serializeForCapsObj(capsTotal);
  if (sRstTotal != CAPS_SUCCESS)
    return sRstTotal;
  else {
      int32_t wRst = caps->write(capsTotal);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  if (!cores)
    caps->write((int32_t)0);
  else {
    caps->write((int32_t)cores->size());
    for(auto &v : *cores) {
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
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t SysCPUInfo::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_SYSCPUINFO));
  std::shared_ptr<Caps> capsTotal;
  assert(total);
  int32_t sRstTotal = total->serializeForCapsObj(capsTotal);
  if (sRstTotal != CAPS_SUCCESS)
    return sRstTotal;
  else {
      int32_t wRst = caps->write(capsTotal);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  if (!cores)
    caps->write((int32_t)0);
  else {
    caps->write((int32_t)cores->size());
    for(auto &v : *cores) {
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
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t SysCPUInfo::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  std::shared_ptr<Caps> capsTotal;
  int32_t rRstTotal = caps->read(capsTotal);
  if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
  if (!total) total = std::make_shared<SysCPUCoreInfo>();
  rRstTotal = total->deserializeForCapsObj(capsTotal);
  if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
  int32_t arraySizeCores = 0;
  int32_t rRstCores = caps->read(arraySizeCores);
  if (rRstCores != CAPS_SUCCESS) return rRstCores;
  if (!cores)
    cores = std::make_shared<std::vector<SysCPUCoreInfo>>();
  else
    cores->clear();
  for(int32_t i = 0; i < arraySizeCores;++i) {
    std::shared_ptr<Caps> c;
    if (caps->read(c) == CAPS_SUCCESS && c) {
      cores->emplace_back();
      int32_t dRst = cores->back().deserializeForCapsObj(c);
      if (dRst != CAPS_SUCCESS) return dRst;
    }
  }
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t SysCPUInfo::deserialize(std::shared_ptr<Caps> &caps) {
  std::shared_ptr<Caps> capsTotal;
  int32_t rRstTotal = caps->read(capsTotal);
  if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
  if (!total) total = std::make_shared<SysCPUCoreInfo>();
  rRstTotal = total->deserializeForCapsObj(capsTotal);
  if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
  int32_t arraySizeCores = 0;
  int32_t rRstCores = caps->read(arraySizeCores);
  if (rRstCores != CAPS_SUCCESS) return rRstCores;
  if (!cores)
    cores = std::make_shared<std::vector<SysCPUCoreInfo>>();
  else
    cores->clear();
  for(int32_t i = 0; i < arraySizeCores;++i) {
    std::shared_ptr<Caps> c;
    if (caps->read(c) == CAPS_SUCCESS && c) {
      cores->emplace_back();
      int32_t dRst = cores->back().deserializeForCapsObj(c);
      if (dRst != CAPS_SUCCESS) return dRst;
    }
  }
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t SysCPUInfo::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  std::shared_ptr<Caps> capsTotal;
  assert(total);
  int32_t sRstTotal = total->serializeForCapsObj(capsTotal);
  if (sRstTotal != CAPS_SUCCESS)
    return sRstTotal;
  else {
      int32_t wRst = caps->write(capsTotal);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  if (!cores)
    caps->write((int32_t)0);
  else {
    caps->write((int32_t)cores->size());
    for(auto &v : *cores) {
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
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t SysCPUInfo::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  std::shared_ptr<Caps> capsTotal;
  int32_t rRstTotal = caps->read(capsTotal);
  if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
  if (!total) total = std::make_shared<SysCPUCoreInfo>();
  rRstTotal = total->deserializeForCapsObj(capsTotal);
  if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
  int32_t arraySizeCores = 0;
  int32_t rRstCores = caps->read(arraySizeCores);
  if (rRstCores != CAPS_SUCCESS) return rRstCores;
  if (!cores)
    cores = std::make_shared<std::vector<SysCPUCoreInfo>>();
  else
    cores->clear();
  for(int32_t i = 0; i < arraySizeCores;++i) {
    std::shared_ptr<Caps> c;
    if (caps->read(c) == CAPS_SUCCESS && c) {
      cores->emplace_back();
      int32_t dRst = cores->back().deserializeForCapsObj(c);
      if (dRst != CAPS_SUCCESS) return dRst;
    }
  }
  return CAPS_SUCCESS;
}

