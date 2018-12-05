#ifndef _MEMINFOS_H
#define _MEMINFOS_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
#include "ProcMemInfo.h"
#include "SysMemInfo.h"
namespace rokid {
  /*
   * 采集到的Mem信息
   */
  class MemInfos {
  private:
    std::shared_ptr<std::vector<ProcMemInfo>> procMemInfo = nullptr;
    std::shared_ptr<SysMemInfo> sysMem = nullptr;
    int64_t timestamp = 0;
  public:
    inline static std::shared_ptr<MemInfos> create() {
      return std::make_shared<MemInfos>();
    }
    /*
    * getter 进程mem信息列表
    */
    inline const std::shared_ptr<std::vector<ProcMemInfo>> getProcMemInfo() const {
      return procMemInfo;
    }
    /*
    * getter 系统Mem状态
    */
    inline const std::shared_ptr<SysMemInfo> & getSysMem() const {
      return sysMem;
    }
    /*
    * getter 采集时间
    */
    inline int64_t getTimestamp() const {
      return timestamp;
    }
    /*
    * setter 进程mem信息列表
    */
    inline void setProcMemInfo(const std::shared_ptr<std::vector<ProcMemInfo>> &v) {
      this->procMemInfo = v;
    }
    /*
    * setter 系统Mem状态
    */
    inline void setSysMem(const std::shared_ptr<SysMemInfo> &v) {
      sysMem = v;
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
#endif // _MEMINFOS_H