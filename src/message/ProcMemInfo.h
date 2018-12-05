#ifndef _PROCMEMINFO_H
#define _PROCMEMINFO_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
namespace rokid {
  /*
   * 单个进程内存信息
   */
  class ProcMemInfo {
  private:
    uint32_t pid = 0;
    std::shared_ptr<std::string> fullName = nullptr;
    int64_t sharedClean = 0;
    int64_t sharedDirty = 0;
    int64_t privateClean = 0;
    int64_t privateDirty = 0;
    int64_t pss = 0;
    int64_t rss = 0;
  public:
    inline static std::shared_ptr<ProcMemInfo> create() {
      return std::make_shared<ProcMemInfo>();
    }
    /*
    * getter 进程id
    */
    inline uint32_t getPid() const {
      return pid;
    }
    /*
    * getter 进程名
    */
    inline const std::shared_ptr<std::string> getFullName() const {
      return fullName;
    }
    /*
    * getter shared_clean
    */
    inline int64_t getSharedClean() const {
      return sharedClean;
    }
    /*
    * getter shared_dirty
    */
    inline int64_t getSharedDirty() const {
      return sharedDirty;
    }
    /*
    * getter private_clean
    */
    inline int64_t getPrivateClean() const {
      return privateClean;
    }
    /*
    * getter private_dirty
    */
    inline int64_t getPrivateDirty() const {
      return privateDirty;
    }
    /*
    * getter pss
    */
    inline int64_t getPss() const {
      return pss;
    }
    /*
    * getter rss
    */
    inline int64_t getRss() const {
      return rss;
    }
    /*
    * setter 进程id
    */
    inline void setPid(uint32_t v) {
      pid = v;
    }
    /*
    * setter 进程名
    */
    inline void setFullName(const std::shared_ptr<std::string> &v) {
      fullName = v;
    }
    /*
    * setter 进程名
    */
    inline void setFullName(const char* v) {
      if (!fullName)    fullName = std::make_shared<std::string>();  *fullName = v;
    }
    /*
    * setter shared_clean
    */
    inline void setSharedClean(int64_t v) {
      sharedClean = v;
    }
    /*
    * setter shared_dirty
    */
    inline void setSharedDirty(int64_t v) {
      sharedDirty = v;
    }
    /*
    * setter private_clean
    */
    inline void setPrivateClean(int64_t v) {
      privateClean = v;
    }
    /*
    * setter private_dirty
    */
    inline void setPrivateDirty(int64_t v) {
      privateDirty = v;
    }
    /*
    * setter pss
    */
    inline void setPss(int64_t v) {
      pss = v;
    }
    /*
    * setter rss
    */
    inline void setRss(int64_t v) {
      rss = v;
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
#endif // _PROCMEMINFO_H