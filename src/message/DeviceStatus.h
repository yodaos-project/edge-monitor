#ifndef _DEVICESTATUS_H
#define _DEVICESTATUS_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
namespace rokid {
  /*
   * 设备状态（设备->平台）
   */
  class DeviceStatus {
  private:
    std::shared_ptr<std::string> sn = nullptr;
    std::shared_ptr<std::string> version = nullptr;
    std::shared_ptr<std::string> vspVersion = nullptr;
    std::shared_ptr<std::string> turenVersion = nullptr;
    int32_t status = 0;
    int32_t shellId = 0;
    int64_t timestamp = 0;
  public:
    inline static std::shared_ptr<DeviceStatus> create() {
      return std::make_shared<DeviceStatus>();
    }
    /*
    * getter 设备sn
    */
    inline const std::shared_ptr<std::string> getSn() const {
      return sn;
    }
    /*
    * getter 设备版本
    */
    inline const std::shared_ptr<std::string> getVersion() const {
      return version;
    }
    /*
    * getter VSP 版本
    */
    inline const std::shared_ptr<std::string> getVspVersion() const {
      return vspVersion;
    }
    /*
    * getter 土人版本
    */
    inline const std::shared_ptr<std::string> getTurenVersion() const {
      return turenVersion;
    }
    /*
    * getter 设备状态：0-idle，1-exe
    */
    inline int32_t getStatus() const {
      return status;
    }
    /*
    * getter 脚本id
    */
    inline int32_t getShellId() const {
      return shellId;
    }
    /*
    * getter 采集时间
    */
    inline int64_t getTimestamp() const {
      return timestamp;
    }
    /*
    * setter 设备sn
    */
    inline void setSn(const std::shared_ptr<std::string> &v) {
      sn = v;
    }
    /*
    * setter 设备sn
    */
    inline void setSn(const char* v) {
      if (!sn)    sn = std::make_shared<std::string>();  *sn = v;
    }
    /*
    * setter 设备版本
    */
    inline void setVersion(const std::shared_ptr<std::string> &v) {
      version = v;
    }
    /*
    * setter 设备版本
    */
    inline void setVersion(const char* v) {
      if (!version)    version = std::make_shared<std::string>();  *version = v;
    }
    /*
    * setter VSP 版本
    */
    inline void setVspVersion(const std::shared_ptr<std::string> &v) {
      vspVersion = v;
    }
    /*
    * setter VSP 版本
    */
    inline void setVspVersion(const char* v) {
      if (!vspVersion)    vspVersion = std::make_shared<std::string>();  *vspVersion = v;
    }
    /*
    * setter 土人版本
    */
    inline void setTurenVersion(const std::shared_ptr<std::string> &v) {
      turenVersion = v;
    }
    /*
    * setter 土人版本
    */
    inline void setTurenVersion(const char* v) {
      if (!turenVersion)    turenVersion = std::make_shared<std::string>();  *turenVersion = v;
    }
    /*
    * setter 设备状态：0-idle，1-exe
    */
    inline void setStatus(int32_t v) {
      status = v;
    }
    /*
    * setter 脚本id
    */
    inline void setShellId(int32_t v) {
      shellId = v;
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
#endif // _DEVICESTATUS_H