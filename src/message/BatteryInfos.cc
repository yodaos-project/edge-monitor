#include "BatteryInfos.h"
#include "MessageDefine.h"
using namespace rokid;
/*
 * serialize this object as buffer
*/
int32_t BatteryInfos::serialize(void* buf, uint32_t bufsize) const {
  std::shared_ptr<Caps> caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_BATTERYINFOS));
  int32_t wRstBatTemp = caps->write((int32_t)batTemp);
  if (wRstBatTemp != CAPS_SUCCESS) return wRstBatTemp;
  int32_t wRstCpuTemp = caps->write((int32_t)cpuTemp);
  if (wRstCpuTemp != CAPS_SUCCESS) return wRstCpuTemp;
  int32_t wRstCurrentNow = caps->write((int32_t)currentNow);
  if (wRstCurrentNow != CAPS_SUCCESS) return wRstCurrentNow;
  int32_t wRstCapacity = caps->write((int32_t)capacity);
  if (wRstCapacity != CAPS_SUCCESS) return wRstCapacity;
  int32_t wRstVoltageBat = caps->write((int32_t)voltageBat);
  if (wRstVoltageBat != CAPS_SUCCESS) return wRstVoltageBat;
  int32_t wRstVoltageUsb = caps->write((int32_t)voltageUsb);
  if (wRstVoltageUsb != CAPS_SUCCESS) return wRstVoltageUsb;
  int32_t wRstStatus;
  assert(status);
  wRstStatus = caps->write(status->c_str());
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstOnline = caps->write((int32_t)online);
  if (wRstOnline != CAPS_SUCCESS) return wRstOnline;
  int32_t wRstPresent = caps->write((int32_t)present);
  if (wRstPresent != CAPS_SUCCESS) return wRstPresent;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return caps->serialize(buf, bufsize);
}
/*
 * deserialize this object as caps (with message type)
 */
int32_t BatteryInfos::serialize(std::shared_ptr<Caps> &caps) const {
  if (!caps)
    caps = Caps::new_instance();
  caps->write(static_cast<int32_t>(MessageType::TYPE_BATTERYINFOS));
  int32_t wRstBatTemp = caps->write((int32_t)batTemp);
  if (wRstBatTemp != CAPS_SUCCESS) return wRstBatTemp;
  int32_t wRstCpuTemp = caps->write((int32_t)cpuTemp);
  if (wRstCpuTemp != CAPS_SUCCESS) return wRstCpuTemp;
  int32_t wRstCurrentNow = caps->write((int32_t)currentNow);
  if (wRstCurrentNow != CAPS_SUCCESS) return wRstCurrentNow;
  int32_t wRstCapacity = caps->write((int32_t)capacity);
  if (wRstCapacity != CAPS_SUCCESS) return wRstCapacity;
  int32_t wRstVoltageBat = caps->write((int32_t)voltageBat);
  if (wRstVoltageBat != CAPS_SUCCESS) return wRstVoltageBat;
  int32_t wRstVoltageUsb = caps->write((int32_t)voltageUsb);
  if (wRstVoltageUsb != CAPS_SUCCESS) return wRstVoltageUsb;
  int32_t wRstStatus;
  assert(status);
  wRstStatus = caps->write(status->c_str());
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstOnline = caps->write((int32_t)online);
  if (wRstOnline != CAPS_SUCCESS) return wRstOnline;
  int32_t wRstPresent = caps->write((int32_t)present);
  if (wRstPresent != CAPS_SUCCESS) return wRstPresent;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from buffer
 */
int32_t BatteryInfos::deserialize(void* buf, uint32_t bufSize) {
  std::shared_ptr<Caps> caps;
  int32_t pRst = Caps::parse(buf, bufSize, caps);
  if(pRst != CAPS_SUCCESS) return pRst;
  int32_t rRstBatTemp = caps->read(batTemp);
  if (rRstBatTemp != CAPS_SUCCESS) return rRstBatTemp;
  int32_t rRstCpuTemp = caps->read(cpuTemp);
  if (rRstCpuTemp != CAPS_SUCCESS) return rRstCpuTemp;
  int32_t rRstCurrentNow = caps->read(currentNow);
  if (rRstCurrentNow != CAPS_SUCCESS) return rRstCurrentNow;
  int32_t rRstCapacity = caps->read(capacity);
  if (rRstCapacity != CAPS_SUCCESS) return rRstCapacity;
  int32_t rRstVoltageBat = caps->read(voltageBat);
  if (rRstVoltageBat != CAPS_SUCCESS) return rRstVoltageBat;
  int32_t rRstVoltageUsb = caps->read(voltageUsb);
  if (rRstVoltageUsb != CAPS_SUCCESS) return rRstVoltageUsb;
  if (!status) status = std::make_shared<std::string>();
  int32_t rRstStatus = caps->read_string(*status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  int32_t rRstOnline = caps->read(online);
  if (rRstOnline != CAPS_SUCCESS) return rRstOnline;
  int32_t rRstPresent = caps->read(present);
  if (rRstPresent != CAPS_SUCCESS) return rRstPresent;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (with message type)
 */
int32_t BatteryInfos::deserialize(std::shared_ptr<Caps> &caps) {
  int32_t rRstBatTemp = caps->read(batTemp);
  if (rRstBatTemp != CAPS_SUCCESS) return rRstBatTemp;
  int32_t rRstCpuTemp = caps->read(cpuTemp);
  if (rRstCpuTemp != CAPS_SUCCESS) return rRstCpuTemp;
  int32_t rRstCurrentNow = caps->read(currentNow);
  if (rRstCurrentNow != CAPS_SUCCESS) return rRstCurrentNow;
  int32_t rRstCapacity = caps->read(capacity);
  if (rRstCapacity != CAPS_SUCCESS) return rRstCapacity;
  int32_t rRstVoltageBat = caps->read(voltageBat);
  if (rRstVoltageBat != CAPS_SUCCESS) return rRstVoltageBat;
  int32_t rRstVoltageUsb = caps->read(voltageUsb);
  if (rRstVoltageUsb != CAPS_SUCCESS) return rRstVoltageUsb;
  if (!status) status = std::make_shared<std::string>();
  int32_t rRstStatus = caps->read_string(*status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  int32_t rRstOnline = caps->read(online);
  if (rRstOnline != CAPS_SUCCESS) return rRstOnline;
  int32_t rRstPresent = caps->read(present);
  if (rRstPresent != CAPS_SUCCESS) return rRstPresent;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * serialize this object as caps (without message type)
 */
int32_t BatteryInfos::serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
  caps = Caps::new_instance();
  int32_t wRstBatTemp = caps->write((int32_t)batTemp);
  if (wRstBatTemp != CAPS_SUCCESS) return wRstBatTemp;
  int32_t wRstCpuTemp = caps->write((int32_t)cpuTemp);
  if (wRstCpuTemp != CAPS_SUCCESS) return wRstCpuTemp;
  int32_t wRstCurrentNow = caps->write((int32_t)currentNow);
  if (wRstCurrentNow != CAPS_SUCCESS) return wRstCurrentNow;
  int32_t wRstCapacity = caps->write((int32_t)capacity);
  if (wRstCapacity != CAPS_SUCCESS) return wRstCapacity;
  int32_t wRstVoltageBat = caps->write((int32_t)voltageBat);
  if (wRstVoltageBat != CAPS_SUCCESS) return wRstVoltageBat;
  int32_t wRstVoltageUsb = caps->write((int32_t)voltageUsb);
  if (wRstVoltageUsb != CAPS_SUCCESS) return wRstVoltageUsb;
  int32_t wRstStatus;
  assert(status);
  wRstStatus = caps->write(status->c_str());
  if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
  int32_t wRstOnline = caps->write((int32_t)online);
  if (wRstOnline != CAPS_SUCCESS) return wRstOnline;
  int32_t wRstPresent = caps->write((int32_t)present);
  if (wRstPresent != CAPS_SUCCESS) return wRstPresent;
  int32_t wRstTimestamp = caps->write((int64_t)timestamp);
  if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
  return CAPS_SUCCESS;
}
/*
 * deserialize this object from caps (without message type)
 */
int32_t BatteryInfos::deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
  int32_t rRstBatTemp = caps->read(batTemp);
  if (rRstBatTemp != CAPS_SUCCESS) return rRstBatTemp;
  int32_t rRstCpuTemp = caps->read(cpuTemp);
  if (rRstCpuTemp != CAPS_SUCCESS) return rRstCpuTemp;
  int32_t rRstCurrentNow = caps->read(currentNow);
  if (rRstCurrentNow != CAPS_SUCCESS) return rRstCurrentNow;
  int32_t rRstCapacity = caps->read(capacity);
  if (rRstCapacity != CAPS_SUCCESS) return rRstCapacity;
  int32_t rRstVoltageBat = caps->read(voltageBat);
  if (rRstVoltageBat != CAPS_SUCCESS) return rRstVoltageBat;
  int32_t rRstVoltageUsb = caps->read(voltageUsb);
  if (rRstVoltageUsb != CAPS_SUCCESS) return rRstVoltageUsb;
  if (!status) status = std::make_shared<std::string>();
  int32_t rRstStatus = caps->read_string(*status);
  if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
  int32_t rRstOnline = caps->read(online);
  if (rRstOnline != CAPS_SUCCESS) return rRstOnline;
  int32_t rRstPresent = caps->read(present);
  if (rRstPresent != CAPS_SUCCESS) return rRstPresent;
  int32_t rRstTimestamp = caps->read(timestamp);
  if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
  return CAPS_SUCCESS;
}

