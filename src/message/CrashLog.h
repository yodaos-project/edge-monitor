#ifndef _CRASHLOG_H
#define _CRASHLOG_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
namespace rokid {
  /*
   * 进程crash日志信息
   */
  class CrashLog {
  private:
    int32_t pid = 0;
    std::shared_ptr<std::string> pName = nullptr;
    std::shared_ptr<std::string> args = nullptr;
    uint64_t runTime = 0;
    uint64_t ts = 0;
  public:
    inline static std::shared_ptr<CrashLog> create() {
      return std::make_shared<CrashLog>();
    }
    /*
    * getter 进程id
    */
    inline int32_t getPid() const {
      return pid;
    }
    /*
    * getter 进程名
    */
    inline const std::shared_ptr<std::string> getName() const {
      return pName;
    }
    /*
    * getter 进程运行参数
    */
    inline const std::shared_ptr<std::string> getArgs() const {
      return args;
    }
    /*
    * getter 运行时长，单位s
    */
    inline uint64_t getRunTime() const {
      return runTime;
    }
    /*
    * getter crash信息采集时间戳
    */
    inline uint64_t getTs() const {
      return ts;
    }
    /*
    * setter 进程id
    */
    inline void setPid(int32_t v) {
      pid = v;
    }
    /*
    * setter 进程名
    */
    inline void setName(const std::shared_ptr<std::string> &v) {
      pName = v;
    }
    /*
    * setter 进程名
    */
    inline void setName(const char* v) {
      if (!pName)    pName = std::make_shared<std::string>();  *pName = v;
    }
    /*
    * setter 进程运行参数
    */
    inline void setArgs(const std::shared_ptr<std::string> &v) {
      args = v;
    }
    /*
    * setter 进程运行参数
    */
    inline void setArgs(const char* v) {
      if (!args)    args = std::make_shared<std::string>();  *args = v;
    }
    /*
    * setter 运行时长，单位s
    */
    inline void setRunTime(uint64_t v) {
      runTime = v;
    }
    /*
    * setter crash信息采集时间戳
    */
    inline void setTs(uint64_t v) {
      ts = v;
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
#endif // _CRASHLOG_H