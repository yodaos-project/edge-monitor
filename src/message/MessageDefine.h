#ifndef _MESSAGEDEFINE_H
#define _MESSAGEDEFINE_H
#include <memory>
#include <assert.h>
/*
* 性能平台消息定义
*/
namespace rokid{
  /*
  * enum of message type
  */
  enum class MessageType: int32_t {
    TYPE_PROCCPUINFO = 1111,
    TYPE_PROCMEMINFO,
    TYPE_SYSCPUCOREINFO,
    TYPE_SYSCPUINFO,
    TYPE_SYSMEMINFO,
    TYPE_CPUINFOS,
    TYPE_MEMINFOS,
    TYPE_COLLECTDATA,
    TYPE_CRASHLOG,
    TYPE_LOG,
    TYPE_TASKCOMMAND,
    TYPE_TASKSTATUS,
    TYPE_DEVICESTATUS,
    TYPE_BATTERYINFOS,
    TYPE_UNKNOWN
  };
}
#endif //_MESSAGEDEFINE_H