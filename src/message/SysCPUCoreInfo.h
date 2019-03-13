#ifndef _SYSCPUCOREINFO_H
#define _SYSCPUCOREINFO_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
namespace rokid {
  /*
   * cpu core info
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
    * getter jiffy in user mode
    */
    inline float getUsrUsage() const {
      return usrUsage;
    }
    /*
    * getter jiffy in kernel mode
    */
    inline float getSysUsage() const {
      return sysUsage;
    }
    /*
    * getter idle percent
    */
    inline float getIdleUsage() const {
      return idleUsage;
    }
    /*
    * getter iowait percent
    */
    inline float getIoUsage() const {
      return ioUsage;
    }
    /*
    * getter busy percent
    */
    inline float getBusyUsage() const {
      return busyUsage;
    }
    /*
    * setter jiffy in user mode
    */
    inline void setUsrUsage(float v) {
      usrUsage = v;
    }
    /*
    * setter jiffy in kernel mode
    */
    inline void setSysUsage(float v) {
      sysUsage = v;
    }
    /*
    * setter idle percent
    */
    inline void setIdleUsage(float v) {
      idleUsage = v;
    }
    /*
    * setter iowait percent
    */
    inline void setIoUsage(float v) {
      ioUsage = v;
    }
    /*
    * setter busy percent
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