#ifndef _SYSCPUCOREINFO_H
#define _SYSCPUCOREINFO_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
namespace rokid {
  /*
   * 系统CPU核心信息
   */
  class SysCPUCoreInfo {
  private:
    float usrUsage = 0;
    float sysUsage = 0;
    float idleUsage = 0;
    float ioUsage = 0;
    float busyUsage = 0;
  public:
    inline static std::shared_ptr<SysCPUCoreInfo> create() {
      return std::make_shared<SysCPUCoreInfo>();
    }
    /*
    * getter 用户态cpu占用百分比
    */
    inline float getUsrUsage() const {
      return usrUsage;
    }
    /*
    * getter 内核态cpu占用百分比
    */
    inline float getSysUsage() const {
      return sysUsage;
    }
    /*
    * getter 空闲cpu占用百分比
    */
    inline float getIdleUsage() const {
      return idleUsage;
    }
    /*
    * getter io等待百分比
    */
    inline float getIoUsage() const {
      return ioUsage;
    }
    /*
    * getter cpu使用率
    */
    inline float getBusyUsage() const {
      return busyUsage;
    }
    /*
    * setter 用户态cpu占用百分比
    */
    inline void setUsrUsage(float v) {
      usrUsage = v;
    }
    /*
    * setter 内核态cpu占用百分比
    */
    inline void setSysUsage(float v) {
      sysUsage = v;
    }
    /*
    * setter 空闲cpu占用百分比
    */
    inline void setIdleUsage(float v) {
      idleUsage = v;
    }
    /*
    * setter io等待百分比
    */
    inline void setIoUsage(float v) {
      ioUsage = v;
    }
    /*
    * setter cpu使用率
    */
    inline void setBusyUsage(float v) {
      busyUsage = v;
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
#endif // _SYSCPUCOREINFO_H