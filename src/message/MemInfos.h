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
   * memory infos
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
    * getter processes memory info
    */
    inline const std::shared_ptr<std::vector<ProcMemInfo>> getProcMemInfo() const {
      return procMemInfo;
    }
    /*
    * getter system memory info
    */
    inline const std::shared_ptr<SysMemInfo> & getSysMem() const {
      return sysMem;
    }
    /*
    * getter collect time
    */
    inline int64_t getTimestamp() const {
      return timestamp;
    }
    /*
    * setter processes memory info
    */
    inline void setProcMemInfo(const std::shared_ptr<std::vector<ProcMemInfo>> &v) {
      this->procMemInfo = v;
    }
    /*
    * setter system memory info
    */
    inline void setSysMem(const std::shared_ptr<SysMemInfo> &v) {
      sysMem = v;
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
#endif // _MEMINFOS_H