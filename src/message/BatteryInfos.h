#ifndef _BATTERYINFOS_H
#define _BATTERYINFOS_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
namespace rokid {
  /*
   * 设备电池监控信息（设备->平台）
   */
  class BatteryInfos {
  private:
    int32_t batTemp = 0;
    int32_t cpuTemp = 0;
    int32_t currentNow = 0;
    int32_t capacity = 0;
    int32_t voltageBat = 0;
    int32_t voltageUsb = 0;
    std::shared_ptr<std::string> status = nullptr;
    int32_t online = 0;
    int32_t present = 0;
    int64_t timestamp = 0;
  public:
    inline static std::shared_ptr<BatteryInfos> create() {
      return std::make_shared<BatteryInfos>();
    }
    /*
    * getter 电池温度
    */
    inline int32_t getBatTemp() const {
      return batTemp;
    }
    /*
    * getter cpu温度
    */
    inline int32_t getCpuTemp() const {
      return cpuTemp;
    }
    /*
    * getter 充电电流(uA)
    */
    inline int32_t getCurrentNow() const {
      return currentNow;
    }
    /*
    * getter 电池电量(%)
    */
    inline int32_t getCapacity() const {
      return capacity;
    }
    /*
    * getter 电池电压(V)
    */
    inline int32_t getVoltageBat() const {
      return voltageBat;
    }
    /*
    * getter USB电压(V)
    */
    inline int32_t getVoltageUsb() const {
      return voltageUsb;
    }
    /*
    * getter 充电状态
    */
    inline const std::shared_ptr<std::string> getStatus() const {
      return status;
    }
    /*
    * getter online
    */
    inline int32_t getOnline() const {
      return online;
    }
    /*
    * getter present
    */
    inline int32_t getPresent() const {
      return present;
    }
    /*
    * getter 采集时间
    */
    inline int64_t getTimestamp() const {
      return timestamp;
    }
    /*
    * setter 电池温度
    */
    inline void setBatTemp(int32_t v) {
      batTemp = v;
    }
    /*
    * setter cpu温度
    */
    inline void setCpuTemp(int32_t v) {
      cpuTemp = v;
    }
    /*
    * setter 充电电流(uA)
    */
    inline void setCurrentNow(int32_t v) {
      currentNow = v;
    }
    /*
    * setter 电池电量(%)
    */
    inline void setCapacity(int32_t v) {
      capacity = v;
    }
    /*
    * setter 电池电压(V)
    */
    inline void setVoltageBat(int32_t v) {
      voltageBat = v;
    }
    /*
    * setter USB电压(V)
    */
    inline void setVoltageUsb(int32_t v) {
      voltageUsb = v;
    }
    /*
    * setter 充电状态
    */
    inline void setStatus(const std::shared_ptr<std::string> &v) {
      status = v;
    }
    /*
    * setter 充电状态
    */
    inline void setStatus(const char* v) {
      if (!status) status = std::make_shared<std::string>();
      *status = v;
    }
    /*
    * setter online
    */
    inline void setOnline(int32_t v) {
      online = v;
    }
    /*
    * setter present
    */
    inline void setPresent(int32_t v) {
      present = v;
    }
    /*
    * setter 采集时间
    */
    inline void setTimestamp(int64_t v) {
      timestamp = v;
    }
    /*
     * serialize this object as buffer
    */
    int32_t serialize(void* buf, uint32_t bufsize) const;
    /*
     * deserialize this object as caps (with message type)
     */
    int32_t serialize(std::shared_ptr<Caps> &caps) const;
    /*
     * deserialize this object from buffer
     */
    int32_t deserialize(void* buf, uint32_t bufSize);
    /*
     * deserialize this object from caps (with message type)
     */
    int32_t deserialize(std::shared_ptr<Caps> &caps);
    /*
     * serialize this object as caps (without message type)
     */
    int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const;
    /*
     * deserialize this object from caps (without message type)
     */
    int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps);
  };

}
#endif // _BATTERYINFOS_H