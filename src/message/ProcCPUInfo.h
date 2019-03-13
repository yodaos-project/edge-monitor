#ifndef _PROCCPUINFO_H
#define _PROCCPUINFO_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
namespace rokid {
  /*
   * process cpu usage
   */
  class ProcCPUInfo {
  private:
    uint32_t pid = 0;
    std::shared_ptr<std::string> status = std::make_shared<std::string>("sss");
    std::shared_ptr<std::string> fullName = nullptr;
    int64_t utime = 0;
    int64_t stime = 0;
    int64_t ticks = 0;
    int32_t nice = 0;
    float cpuUsage = 0;
  public:
    inline static std::shared_ptr<ProcCPUInfo> create() {
      return std::make_shared<ProcCPUInfo>();
    }
    /*
    * getter process id
    */
    inline uint32_t getPid() const {
      return pid;
    }
    /*
    * getter process status
    */
    inline const std::shared_ptr<std::string> getStatus() const {
      return status;
    }
    /*
    * getter process command line
    */
    inline const std::shared_ptr<std::string> getFullName() const {
      return fullName;
    }
    /*
    * getter cpu jiffy in user mode
    */
    inline int64_t getUtime() const {
      return utime;
    }
    /*
    * getter cpu jiffy in kernel mode
    */
    inline int64_t getStime() const {
      return stime;
    }
    /*
    * getter jiffy summary
    */
    inline int64_t getTicks() const {
      return ticks;
    }
    /*
    * getter nice value
    */
    inline int32_t getNice() const {
      return nice;
    }
    /*
    * getter cpu usage percent
    */
    inline float getCpuUsage() const {
      return cpuUsage;
    }
    /*
    * setter process id
    */
    inline void setPid(uint32_t v) {
      pid = v;
    }
    /*
    * setter process status
    */
    inline void setStatus(const std::shared_ptr<std::string> &v) {
      status = v;
    }
    /*
    * setter process status
    */
    inline void setStatus(const char* v) {
      if (!status)    status = std::make_shared<std::string>();  *status = v;
    }
    /*
    * setter process command line
    */
    inline void setFullName(const std::shared_ptr<std::string> &v) {
      fullName = v;
    }
    /*
    * setter process command line
    */
    inline void setFullName(const char* v) {
      if (!fullName)    fullName = std::make_shared<std::string>();  *fullName = v;
    }
    /*
    * setter cpu jiffy in user mode
    */
    inline void setUtime(int64_t v) {
      utime = v;
    }
    /*
    * setter cpu jiffy in kernel mode
    */
    inline void setStime(int64_t v) {
      stime = v;
    }
    /*
    * setter jiffy summary
    */
    inline void setTicks(int64_t v) {
      ticks = v;
    }
    /*
    * setter nice value
    */
    inline void setNice(int32_t v) {
      nice = v;
    }
    /*
    * setter cpu usage percent
    */
    inline void setCpuUsage(float v) {
      cpuUsage = v;
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
#endif // _PROCCPUINFO_H