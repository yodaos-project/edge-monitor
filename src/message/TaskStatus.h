#ifndef _TASKSTATUS_H
#define _TASKSTATUS_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
namespace rokid {
  /*
   * 任务状态（设备->平台）
   */
  class TaskStatus {
  private:
    int32_t taskId = 0;
    int32_t status = 0;
    std::shared_ptr<std::string> message = nullptr;
    int32_t shellId = 0;
    int64_t timestamp = 0;
  public:
    inline static std::shared_ptr<TaskStatus> create() {
      return std::make_shared<TaskStatus>();
    }
    /*
    * getter 设备sn
    */
    inline int32_t getTaskId() const {
      return taskId;
    }
    /*
    * getter 任务状态：0-运行中, 1-执行失败, 2-正常结束, 3-准备中
    */
    inline int32_t getStatus() const {
      return status;
    }
    /*
    * getter 状态描述
    */
    inline const std::shared_ptr<std::string> getMessage() const {
      return message;
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
    inline void setTaskId(int32_t v) {
      taskId = v;
    }
    /*
    * setter 任务状态：0-运行中, 1-执行失败, 2-正常结束, 3-准备中
    */
    inline void setStatus(int32_t v) {
      status = v;
    }
    /*
    * setter 状态描述
    */
    inline void setMessage(const std::shared_ptr<std::string> &v) {
      message = v;
    }
    /*
    * setter 状态描述
    */
    inline void setMessage(const char* v) {
      if (!message) message = std::make_shared<std::string>();
      *message = v;
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
#endif // _TASKSTATUS_H