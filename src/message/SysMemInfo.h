#ifndef _SYSMEMINFO_H
#define _SYSMEMINFO_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
namespace rokid {
  /*
   * 系统内存信息
   */
  class SysMemInfo {
  private:
    int64_t total = 0;
    int64_t free = 0;
    int64_t buffers = 0;
    int64_t cached = 0;
    int64_t available = 0;
  public:
    inline static std::shared_ptr<SysMemInfo> create() {
      return std::make_shared<SysMemInfo>();
    }
    /*
    * getter 总内存
    */
    inline int64_t getTotal() const {
      return total;
    }
    /*
    * getter free内存
    */
    inline int64_t getFree() const {
      return free;
    }
    /*
    * getter buffers内存
    */
    inline int64_t getBuffers() const {
      return buffers;
    }
    /*
    * getter cached
    */
    inline int64_t getCached() const {
      return cached;
    }
    /*
    * getter available
    */
    inline int64_t getAvailable() const {
      return available;
    }
    /*
    * setter 总内存
    */
    inline void setTotal(int64_t v) {
      total = v;
    }
    /*
    * setter free内存
    */
    inline void setFree(int64_t v) {
      free = v;
    }
    /*
    * setter buffers内存
    */
    inline void setBuffers(int64_t v) {
      buffers = v;
    }
    /*
    * setter cached
    */
    inline void setCached(int64_t v) {
      cached = v;
    }
    /*
    * setter available
    */
    inline void setAvailable(int64_t v) {
      available = v;
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
#endif // _SYSMEMINFO_H