#include "CollectData.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t CollectData::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_COLLECTDATA));
  std::shared_ptr<Caps> capsCpu;
  assert(cpu);
  int32_t sRstCpu = cpu->serializeForCapsObj(capsCpu);
  if (sRstCpu != CAPS_SUCCESS)
    return sRstCpu;
  else {
      int32_t wRst = caps->write(capsCpu);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  std::shared_ptr<Caps> capsMem;
  assert(mem);
  int32_t sRstMem = mem->serializeForCapsObj(capsMem);
  if (sRstMem != CAPS_SUCCESS)
    return sRstMem;
  else {
      int32_t wRst = caps->write(capsMem);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t CollectData::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_COLLECTDATA));
  std::shared_ptr<Caps> capsCpu;
  assert(cpu);
  int32_t sRstCpu = cpu->serializeForCapsObj(capsCpu);
  if (sRstCpu != CAPS_SUCCESS)
    return sRstCpu;
  else {
      int32_t wRst = caps->write(capsCpu);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  std::shared_ptr<Caps> capsMem;
  assert(mem);
  int32_t sRstMem = mem->serializeForCapsObj(capsMem);
  if (sRstMem != CAPS_SUCCESS)
    return sRstMem;
  else {
      int32_t wRst = caps->write(capsMem);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t CollectData::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  std::shared_ptr<Caps> capsCpu;
  int32_t rRstCpu = caps->read(capsCpu);
  if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
  if (!cpu) cpu = std::make_shared<CPUInfos>();
  rRstCpu = cpu->deserializeForCapsObj(capsCpu);
  if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
  std::shared_ptr<Caps> capsMem;
  int32_t rRstMem = caps->read(capsMem);
  if (rRstMem != CAPS_SUCCESS) return rRstMem;
  if (!mem) mem = std::make_shared<MemInfos>();
  rRstMem = mem->deserializeForCapsObj(capsMem);
  if (rRstMem != CAPS_SUCCESS) return rRstMem;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t CollectData::deserialize(std::shared_ptr<Caps> &caps) {
  std::shared_ptr<Caps> capsCpu;
  int32_t rRstCpu = caps->read(capsCpu);
  if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
  if (!cpu) cpu = std::make_shared<CPUInfos>();
  rRstCpu = cpu->deserializeForCapsObj(capsCpu);
  if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
  std::shared_ptr<Caps> capsMem;
  int32_t rRstMem = caps->read(capsMem);
  if (rRstMem != CAPS_SUCCESS) return rRstMem;
  if (!mem) mem = std::make_shared<MemInfos>();
  rRstMem = mem->deserializeForCapsObj(capsMem);
  if (rRstMem != CAPS_SUCCESS) return rRstMem;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t CollectData::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  std::shared_ptr<Caps> capsCpu;
  assert(cpu);
  int32_t sRstCpu = cpu->serializeForCapsObj(capsCpu);
  if (sRstCpu != CAPS_SUCCESS)
    return sRstCpu;
  else {
      int32_t wRst = caps->write(capsCpu);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  std::shared_ptr<Caps> capsMem;
  assert(mem);
  int32_t sRstMem = mem->serializeForCapsObj(capsMem);
  if (sRstMem != CAPS_SUCCESS)
    return sRstMem;
  else {
      int32_t wRst = caps->write(capsMem);
      if (wRst != CAPS_SUCCESS) return wRst;
  }
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t CollectData::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  std::shared_ptr<Caps> capsCpu;
  int32_t rRstCpu = caps->read(capsCpu);
  if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
  if (!cpu) cpu = std::make_shared<CPUInfos>();
  rRstCpu = cpu->deserializeForCapsObj(capsCpu);
  if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
  std::shared_ptr<Caps> capsMem;
  int32_t rRstMem = caps->read(capsMem);
  if (rRstMem != CAPS_SUCCESS) return rRstMem;
  if (!mem) mem = std::make_shared<MemInfos>();
  rRstMem = mem->deserializeForCapsObj(capsMem);
  if (rRstMem != CAPS_SUCCESS) return rRstMem;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}

