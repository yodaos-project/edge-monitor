#ifndef _LOG_H
#define _LOG_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
namespace rokid {
  /*
   * 日志信息
   */
  class Log {
  private:
    std::shared_ptr<std::string> content = nullptr;
  public:
    inline static std::shared_ptr<Log> create() {
      return std::make_shared<Log>();
    }
    /*
    * getter 日志内容
    */
    inline const std::shared_ptr<std::string> getContent() const {
      return content;
    }
    /*
    * setter 日志内容
    */
    inline void setContent(const std::shared_ptr<std::string> &v) {
      content = v;
    }
    /*
    * setter 日志内容
    */
    inline void setContent(const char* v) {
      if (!content) content = std::make_shared<std::string>();
      *content = v;
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
#endif // _LOG_H