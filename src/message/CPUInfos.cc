#include "CPUInfos.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t CPUInfos::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_CPUINFOS));
  if (!procCpuList)
    caps->write((int32_t)0);
  else {
    caps->write((int32_t)procCpuList->size());
    for(auto &v : *procCpuList) {
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
  std::shared_ptr<Caps> capsSysCpu;
  assert(sysCpu);
  int32_t sRstSysCpu = sysCpu->serializeForCapsObj(capsSysCpu);
  if (sRstSysCpu != CAPS_SUCCESS)
    return sRstSysCpu;
  else {
      int32_t wRst = caps->write(capsSysCpu);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t CPUInfos::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_CPUINFOS));
  if (!procCpuList)
    caps->write((int32_t)0);
  else {
    caps->write((int32_t)procCpuList->size());
    for(auto &v : *procCpuList) {
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
  std::shared_ptr<Caps> capsSysCpu;
  assert(sysCpu);
  int32_t sRstSysCpu = sysCpu->serializeForCapsObj(capsSysCpu);
  if (sRstSysCpu != CAPS_SUCCESS)
    return sRstSysCpu;
  else {
      int32_t wRst = caps->write(capsSysCpu);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t CPUInfos::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  int32_t arraySizeProcCpuList = 0;
  int32_t rRstProcCpuList = caps->read(arraySizeProcCpuList);
  if (rRstProcCpuList != CAPS_SUCCESS) return rRstProcCpuList;
  procCpuList->clear();
  for(int32_t i = 0; i < arraySizeProcCpuList;++i) {
    std::shared_ptr<Caps> c;
    if (caps->read(c) == CAPS_SUCCESS && c) {
      procCpuList->emplace_back();
      int32_t dRst = procCpuList->back().deserializeForCapsObj(c);
      if (dRst != CAPS_SUCCESS) return dRst;
    }
  }
  std::shared_ptr<Caps> capsSysCpu;
  int32_t rRstSysCpu = caps->read(capsSysCpu);
  if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
  if (!sysCpu) sysCpu = std::make_shared<SysCPUInfo>();
  rRstSysCpu = sysCpu->deserializeForCapsObj(capsSysCpu);
  if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t CPUInfos::deserialize(std::shared_ptr<Caps> &caps) {
  int32_t arraySizeProcCpuList = 0;
  int32_t rRstProcCpuList = caps->read(arraySizeProcCpuList);
  if (rRstProcCpuList != CAPS_SUCCESS) return rRstProcCpuList;
  procCpuList->clear();
  for(int32_t i = 0; i < arraySizeProcCpuList;++i) {
    std::shared_ptr<Caps> c;
    if (caps->read(c) == CAPS_SUCCESS && c) {
      procCpuList->emplace_back();
      int32_t dRst = procCpuList->back().deserializeForCapsObj(c);
      if (dRst != CAPS_SUCCESS) return dRst;
    }
  }
  std::shared_ptr<Caps> capsSysCpu;
  int32_t rRstSysCpu = caps->read(capsSysCpu);
  if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
  if (!sysCpu) sysCpu = std::make_shared<SysCPUInfo>();
  rRstSysCpu = sysCpu->deserializeForCapsObj(capsSysCpu);
  if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t CPUInfos::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  if (!procCpuList)
    caps->write((int32_t)0);
  else {
    caps->write((int32_t)procCpuList->size());
    for(auto &v : *procCpuList) {
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
  std::shared_ptr<Caps> capsSysCpu;
  assert(sysCpu);
  int32_t sRstSysCpu = sysCpu->serializeForCapsObj(capsSysCpu);
  if (sRstSysCpu != CAPS_SUCCESS)
    return sRstSysCpu;
  else {
      int32_t wRst = caps->write(capsSysCpu);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t CPUInfos::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  int32_t arraySizeProcCpuList = 0;
  int32_t rRstProcCpuList = caps->read(arraySizeProcCpuList);
  if (rRstProcCpuList != CAPS_SUCCESS) return rRstProcCpuList;
  procCpuList->clear();
  for(int32_t i = 0; i < arraySizeProcCpuList;++i) {
    std::shared_ptr<Caps> c;
    if (caps->read(c) == CAPS_SUCCESS && c) {
      procCpuList->emplace_back();
      int32_t dRst = procCpuList->back().deserializeForCapsObj(c);
      if (dRst != CAPS_SUCCESS) return dRst;
    }
  }
  std::shared_ptr<Caps> capsSysCpu;
  int32_t rRstSysCpu = caps->read(capsSysCpu);
  if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
  if (!sysCpu) sysCpu = std::make_shared<SysCPUInfo>();
  rRstSysCpu = sysCpu->deserializeForCapsObj(capsSysCpu);
  if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}

