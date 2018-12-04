#ifndef _MESSAGECOMMON_H
#define _MESSAGECOMMON_H
#include "caps.h"
#include "MessageDefine.h"
#include "ProcCPUInfo.h"
#include "ProcMemInfo.h"
#include "SysCPUCoreInfo.h"
#include "SysCPUInfo.h"
#include "SysMemInfo.h"
#include "CPUInfos.h"
#include "MemInfos.h"
#include "CollectData.h"
#include "CrashLog.h"
#include "Log.h"
#include "TaskCommand.h"
#include "TaskStatus.h"
#include "DeviceStatus.h"
#include <vector>
#include <string>

/*
* 性能平台消息定义
*/
namespace rokid{
  /*
  * type define for shared_ptr
  */
  typedef std::shared_ptr<ProcCPUInfo> ProcCPUInfoPtr;
  typedef std::shared_ptr<ProcMemInfo> ProcMemInfoPtr;
  typedef std::shared_ptr<SysCPUCoreInfo> SysCPUCoreInfoPtr;
  typedef std::shared_ptr<SysCPUInfo> SysCPUInfoPtr;
  typedef std::shared_ptr<SysMemInfo> SysMemInfoPtr;
  typedef std::shared_ptr<CPUInfos> CPUInfosPtr;
  typedef std::shared_ptr<MemInfos> MemInfosPtr;
  typedef std::shared_ptr<CollectData> CollectDataPtr;
  typedef std::shared_ptr<CrashLog> CrashLogPtr;
  typedef std::shared_ptr<Log> LogPtr;
  typedef std::shared_ptr<TaskCommand> TaskCommandPtr;
  typedef std::shared_ptr<TaskStatus> TaskStatusPtr;
  typedef std::shared_ptr<DeviceStatus> DeviceStatusPtr;
  /*
   * you should call this function when you got message package,
   * [in] buff: the packgae buffer
   * [in] buffer_len: then buffer length
   * [out] caps: the caps object contain message, but without message type
   * return message type if success, else return MessageType::TYPE_UNKNOWN
   */
  MessageType get_msg_type(const unsigned char * buff, int32_t buff_len, std::shared_ptr<Caps> &caps);
  MessageType get_msg_type(std::shared_ptr<Caps> &caps);
}
#endif //_MESSAGECOMMON_H
