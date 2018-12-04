#ifndef _TASKCOMMAND_H
#define _TASKCOMMAND_H
#include <vector>
#include <string>
#include <memory>
#include "caps.h"
namespace rokid {
  /*
   * 任务下发(平台->设备)
   */
  class TaskCommand {
  private:
    int32_t taskId = 0;
    std::shared_ptr<std::string> taskType = nullptr;
    uint32_t shellId = 0;
    std::shared_ptr<std::string> shellType = nullptr;
    std::shared_ptr<std::string> shellContent = nullptr;
    int64_t timestamp = 0;
  public:
    inline static std::shared_ptr<TaskCommand> create() {
      return std::make_shared<TaskCommand>();
    }
    /*
    * getter 任务id
    */
    inline int32_t getTaskId() const {
      return taskId;
    }
    /*
    * getter 任务类型
    */
    inline const std::shared_ptr<std::string> getTaskType() const {
      return taskType;
    }
    /*
    * getter 脚本id
    */
    inline uint32_t getShellId() const {
      return shellId;
    }
    /*
    * getter 脚本类型
    */
    inline const std::shared_ptr<std::string> getShellType() const {
      return shellType;
    }
    /*
    * getter 脚本内容
    */
    inline const std::shared_ptr<std::string> getShellContent() const {
      return shellContent;
    }
    /*
    * getter 命令时间
    */
    inline int64_t getTimestamp() const {
      return timestamp;
    }
    /*
    * setter 任务id
    */
    inline void setTaskId(int32_t v) {
      taskId = v;
    }
    /*
    * setter 任务类型
    */
    inline void setTaskType(const std::shared_ptr<std::string> &v) {
      taskType = v;
    }
    /*
    * setter 任务类型
    */
    inline void setTaskType(const char* v) {
      if (!taskType)    taskType = std::make_shared<std::string>();  *taskType = v;
    }
    /*
    * setter 脚本id
    */
    inline void setShellId(uint32_t v) {
      shellId = v;
    }
    /*
    * setter 脚本类型
    */
    inline void setShellType(const std::shared_ptr<std::string> &v) {
      shellType = v;
    }
    /*
    * setter 脚本类型
    */
    inline void setShellType(const char* v) {
      if (!shellType)    shellType = std::make_shared<std::string>();  *shellType = v;
    }
    /*
    * setter 脚本内容
    */
    inline void setShellContent(const std::shared_ptr<std::string> &v) {
      shellContent = v;
    }
    /*
    * setter 脚本内容
    */
    inline void setShellContent(const char* v) {
      if (!shellContent)    shellContent = std::make_shared<std::string>();  *shellContent = v;
    }
    /*
    * setter 命令时间
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
#endif // _TASKCOMMAND_H