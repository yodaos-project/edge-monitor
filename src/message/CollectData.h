#ifndef _COLLECTDATA_H
#define _COLLECTDATA_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
#include "CPUInfos.h"
#include "MemInfos.h"
namespace rokid {
  /*
   * 采集到的信息
   */
  class CollectData {
  private:
    std::shared_ptr<CPUInfos> cpu = nullptr;
    std::shared_ptr<MemInfos> mem = nullptr;
    int64_t timestamp = 0;
  public:
    inline static std::shared_ptr<CollectData> create() {
      return std::make_shared<CollectData>();
    }
    /*
    * getter CPU信息，包括系统CPU和进程CPU
    */
    inline const std::shared_ptr<CPUInfos> & getCpu() const {
      return cpu;
    }
    /*
    * getter 内存信息，包括系统内存和进程内存
    */
    inline const std::shared_ptr<MemInfos> & getMem() const {
      return mem;
    }
    /*
    * getter 采集时间
    */
    inline int64_t getTimestamp() const {
      return timestamp;
    }
    /*
    * setter CPU信息，包括系统CPU和进程CPU
    */
    inline void setCpu(const std::shared_ptr<CPUInfos> &v) {
      cpu = v;
    }
    /*
    * setter 内存信息，包括系统内存和进程内存
    */
    inline void setMem(const std::shared_ptr<MemInfos> &v) {
      mem = v;
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
#endif // _COLLECTDATA_H