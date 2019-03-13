#ifndef _CPUINFOS_H
#define _CPUINFOS_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
#include "ProcCPUInfo.h"
#include "SysCPUInfo.h"
namespace rokid {
  /*
   * cpu info
   */
  class CPUInfos {
  private:
    std::shared_ptr<std::vector<ProcCPUInfo>> procCpuList = nullptr;
    std::shared_ptr<SysCPUInfo> sysCpu = nullptr;
    int64_t timestamp = 0;
  public:
    inline static std::shared_ptr<CPUInfos> create() {
      return std::make_shared<CPUInfos>();
    }
    /*
    * getter processes cpu infos
    */
    inline const std::shared_ptr<std::vector<ProcCPUInfo>> getProcCpuList() const {
      return procCpuList;
    }
    /*
    * getter system cpu infos
    */
    inline const std::shared_ptr<SysCPUInfo> & getSysCpu() const {
      return sysCpu;
    }
    /*
    * getter collect time
    */
    inline int64_t getTimestamp() const {
      return timestamp;
    }
    /*
    * setter processes cpu infos
    */
    inline void setProcCpuList(const std::shared_ptr<std::vector<ProcCPUInfo>> &v) {
      this->procCpuList = v;
    }
    /*
    * setter system cpu infos
    */
    inline void setSysCpu(const std::shared_ptr<SysCPUInfo> &v) {
      sysCpu = v;
    }
    /*
    * setter collect time
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
#endif // _CPUINFOS_H