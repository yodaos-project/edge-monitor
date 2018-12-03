#ifndef _CAPSMESSAGE_H
#define _CAPSMESSAGE_H
#include "caps.h"
#include <vector>
#include <string>
namespace rokid{

    //enum of message type
    enum class MessageType: int32_t {TYPE_PROCCPUINFO = 1111, TYPE_PROCMEMINFO, TYPE_SYSCPUCOREINFO, TYPE_SYSCPUINFO, TYPE_SYSMEMINFO, TYPE_CPUINFOS, TYPE_MEMINFOS, TYPE_COLLECTDATA, TYPE_CRASHLOG, TYPE_LOG, TYPE_TASKCOMMAND, TYPE_TASKSTATUS, TYPE_DEVICESTATUS, TYPE_UNKNOWN};
    class ProcCPUInfo {
    private:
        uint32_t pid;
        std::shared_ptr<std::string> status;
        std::shared_ptr<std::string> fullName;
        int64_t utime;
        int64_t stime;
        int64_t ticks;
        int32_t nice;
        float cpuUsage;
    public:
        static std::shared_ptr<ProcCPUInfo> create() {
          return std::make_shared<ProcCPUInfo>();
        }
        //getter 进程id
        uint32_t getPid() const { return pid; }
        //getter 进程状态
        const std::shared_ptr<std::string> getStatus() const { return status; }
        //getter 进程名
        const std::shared_ptr<std::string> getFullName() const { return fullName; }
        //getter 用户态cpu tick计数
        int64_t getUtime() const { return utime; }
        //getter 内核态cpu tick计数
        int64_t getStime() const { return stime; }
        //getter 总tick计数
        int64_t getTicks() const { return ticks; }
        //getter nice
        int32_t getNice() const { return nice; }
        //getter cpu占用百分比
        float getCpuUsage() const { return cpuUsage; }
        //setter 进程id
        void setPid(uint32_t v) { pid = v; }
        //setter 进程状态
        void setStatus(const std::shared_ptr<std::string> &v) { status = v; }
        //setter 进程状态
        void setStatus(const char* v) { if (!status) status = std::make_shared<std::string>(); *status = v; }
        //setter 进程名
        void setFullName(const std::shared_ptr<std::string> &v) { fullName = v; }
        //setter 进程名
        void setFullName(const char* v) { if (!fullName) fullName = std::make_shared<std::string>(); *fullName = v; }
        //setter 用户态cpu tick计数
        void setUtime(int64_t v) { utime = v; }
        //setter 内核态cpu tick计数
        void setStime(int64_t v) { stime = v; }
        //setter 总tick计数
        void setTicks(int64_t v) { ticks = v; }
        //setter nice
        void setNice(int32_t v) { nice = v; }
        //setter cpu占用百分比
        void setCpuUsage(float v) { cpuUsage = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_PROCCPUINFO));
          int32_t wRstPid = caps->write((uint32_t)pid);
          if (wRstPid != CAPS_SUCCESS) return wRstPid;
          int32_t wRstStatus;
          assert(status);
          wRstStatus = caps->write(status->c_str());
          if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
          int32_t wRstFullName;
          assert(fullName);
          wRstFullName = caps->write(fullName->c_str());
          if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
          int32_t wRstUtime = caps->write((int64_t)utime);
          if (wRstUtime != CAPS_SUCCESS) return wRstUtime;
          int32_t wRstStime = caps->write((int64_t)stime);
          if (wRstStime != CAPS_SUCCESS) return wRstStime;
          int32_t wRstTicks = caps->write((int64_t)ticks);
          if (wRstTicks != CAPS_SUCCESS) return wRstTicks;
          int32_t wRstNice = caps->write((int32_t)nice);
          if (wRstNice != CAPS_SUCCESS) return wRstNice;
          int32_t wRstCpuUsage = caps->write((float)cpuUsage);
          if (wRstCpuUsage != CAPS_SUCCESS) return wRstCpuUsage;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_PROCCPUINFO));
          int32_t wRstPid = caps->write((uint32_t)pid);
          if (wRstPid != CAPS_SUCCESS) return wRstPid;
          int32_t wRstStatus;
          assert(status);
          wRstStatus = caps->write(status->c_str());
          if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
          int32_t wRstFullName;
          assert(fullName);
          wRstFullName = caps->write(fullName->c_str());
          if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
          int32_t wRstUtime = caps->write((int64_t)utime);
          if (wRstUtime != CAPS_SUCCESS) return wRstUtime;
          int32_t wRstStime = caps->write((int64_t)stime);
          if (wRstStime != CAPS_SUCCESS) return wRstStime;
          int32_t wRstTicks = caps->write((int64_t)ticks);
          if (wRstTicks != CAPS_SUCCESS) return wRstTicks;
          int32_t wRstNice = caps->write((int32_t)nice);
          if (wRstNice != CAPS_SUCCESS) return wRstNice;
          int32_t wRstCpuUsage = caps->write((float)cpuUsage);
          if (wRstCpuUsage != CAPS_SUCCESS) return wRstCpuUsage;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          int32_t rRstPid = caps->read(pid);
          if (rRstPid != CAPS_SUCCESS) return rRstPid;
          if (!status) status = std::make_shared<std::string>();
          int32_t rRstStatus = caps->read_string(*status);
          if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
          if (!fullName) fullName = std::make_shared<std::string>();
          int32_t rRstFullName = caps->read_string(*fullName);
          if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
          int32_t rRstUtime = caps->read(utime);
          if (rRstUtime != CAPS_SUCCESS) return rRstUtime;
          int32_t rRstStime = caps->read(stime);
          if (rRstStime != CAPS_SUCCESS) return rRstStime;
          int32_t rRstTicks = caps->read(ticks);
          if (rRstTicks != CAPS_SUCCESS) return rRstTicks;
          int32_t rRstNice = caps->read(nice);
          if (rRstNice != CAPS_SUCCESS) return rRstNice;
          int32_t rRstCpuUsage = caps->read(cpuUsage);
          if (rRstCpuUsage != CAPS_SUCCESS) return rRstCpuUsage;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          int32_t rRstPid = caps->read(pid);
          if (rRstPid != CAPS_SUCCESS) return rRstPid;
          if (!status) status = std::make_shared<std::string>();
          int32_t rRstStatus = caps->read_string(*status);
          if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
          if (!fullName) fullName = std::make_shared<std::string>();
          int32_t rRstFullName = caps->read_string(*fullName);
          if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
          int32_t rRstUtime = caps->read(utime);
          if (rRstUtime != CAPS_SUCCESS) return rRstUtime;
          int32_t rRstStime = caps->read(stime);
          if (rRstStime != CAPS_SUCCESS) return rRstStime;
          int32_t rRstTicks = caps->read(ticks);
          if (rRstTicks != CAPS_SUCCESS) return rRstTicks;
          int32_t rRstNice = caps->read(nice);
          if (rRstNice != CAPS_SUCCESS) return rRstNice;
          int32_t rRstCpuUsage = caps->read(cpuUsage);
          if (rRstCpuUsage != CAPS_SUCCESS) return rRstCpuUsage;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          int32_t wRstPid = caps->write((uint32_t)pid);
          if (wRstPid != CAPS_SUCCESS) return wRstPid;
          int32_t wRstStatus;
          assert(status);
          wRstStatus = caps->write(status->c_str());
          if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
          int32_t wRstFullName;
          assert(fullName);
          wRstFullName = caps->write(fullName->c_str());
          if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
          int32_t wRstUtime = caps->write((int64_t)utime);
          if (wRstUtime != CAPS_SUCCESS) return wRstUtime;
          int32_t wRstStime = caps->write((int64_t)stime);
          if (wRstStime != CAPS_SUCCESS) return wRstStime;
          int32_t wRstTicks = caps->write((int64_t)ticks);
          if (wRstTicks != CAPS_SUCCESS) return wRstTicks;
          int32_t wRstNice = caps->write((int32_t)nice);
          if (wRstNice != CAPS_SUCCESS) return wRstNice;
          int32_t wRstCpuUsage = caps->write((float)cpuUsage);
          if (wRstCpuUsage != CAPS_SUCCESS) return wRstCpuUsage;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          int32_t rRstPid = caps->read(pid);
          if (rRstPid != CAPS_SUCCESS) return rRstPid;
          if (!status) status = std::make_shared<std::string>();
          int32_t rRstStatus = caps->read_string(*status);
          if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
          if (!fullName) fullName = std::make_shared<std::string>();
          int32_t rRstFullName = caps->read_string(*fullName);
          if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
          int32_t rRstUtime = caps->read(utime);
          if (rRstUtime != CAPS_SUCCESS) return rRstUtime;
          int32_t rRstStime = caps->read(stime);
          if (rRstStime != CAPS_SUCCESS) return rRstStime;
          int32_t rRstTicks = caps->read(ticks);
          if (rRstTicks != CAPS_SUCCESS) return rRstTicks;
          int32_t rRstNice = caps->read(nice);
          if (rRstNice != CAPS_SUCCESS) return rRstNice;
          int32_t rRstCpuUsage = caps->read(cpuUsage);
          if (rRstCpuUsage != CAPS_SUCCESS) return rRstCpuUsage;
          return CAPS_SUCCESS;
        }
    };

    class ProcMemInfo {
    private:
        uint32_t pid;
        std::shared_ptr<std::string> fullName;
        int64_t sharedClean;
        int64_t sharedDirty;
        int64_t privateClean;
        int64_t privateDirty;
        int64_t pss;
        int64_t rss;
    public:
        static std::shared_ptr<ProcMemInfo> create() {
          return std::make_shared<ProcMemInfo>();
        }
        //getter 进程id
        uint32_t getPid() const { return pid; }
        //getter 进程名
        const std::shared_ptr<std::string> getFullName() const { return fullName; }
        //getter shared_clean
        int64_t getSharedClean() const { return sharedClean; }
        //getter shared_dirty
        int64_t getSharedDirty() const { return sharedDirty; }
        //getter private_clean
        int64_t getPrivateClean() const { return privateClean; }
        //getter private_dirty
        int64_t getPrivateDirty() const { return privateDirty; }
        //getter pss
        int64_t getPss() const { return pss; }
        //getter rss
        int64_t getRss() const { return rss; }
        //setter 进程id
        void setPid(uint32_t v) { pid = v; }
        //setter 进程名
        void setFullName(const std::shared_ptr<std::string> &v) { fullName = v; }
        //setter 进程名
        void setFullName(const char* v) { if (!fullName) fullName = std::make_shared<std::string>(); *fullName = v; }
        //setter shared_clean
        void setSharedClean(int64_t v) { sharedClean = v; }
        //setter shared_dirty
        void setSharedDirty(int64_t v) { sharedDirty = v; }
        //setter private_clean
        void setPrivateClean(int64_t v) { privateClean = v; }
        //setter private_dirty
        void setPrivateDirty(int64_t v) { privateDirty = v; }
        //setter pss
        void setPss(int64_t v) { pss = v; }
        //setter rss
        void setRss(int64_t v) { rss = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_PROCMEMINFO));
          int32_t wRstPid = caps->write((uint32_t)pid);
          if (wRstPid != CAPS_SUCCESS) return wRstPid;
          int32_t wRstFullName;
          assert(fullName);
          wRstFullName = caps->write(fullName->c_str());
          if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
          int32_t wRstSharedClean = caps->write((int64_t)sharedClean);
          if (wRstSharedClean != CAPS_SUCCESS) return wRstSharedClean;
          int32_t wRstSharedDirty = caps->write((int64_t)sharedDirty);
          if (wRstSharedDirty != CAPS_SUCCESS) return wRstSharedDirty;
          int32_t wRstPrivateClean = caps->write((int64_t)privateClean);
          if (wRstPrivateClean != CAPS_SUCCESS) return wRstPrivateClean;
          int32_t wRstPrivateDirty = caps->write((int64_t)privateDirty);
          if (wRstPrivateDirty != CAPS_SUCCESS) return wRstPrivateDirty;
          int32_t wRstPss = caps->write((int64_t)pss);
          if (wRstPss != CAPS_SUCCESS) return wRstPss;
          int32_t wRstRss = caps->write((int64_t)rss);
          if (wRstRss != CAPS_SUCCESS) return wRstRss;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_PROCMEMINFO));
          int32_t wRstPid = caps->write((uint32_t)pid);
          if (wRstPid != CAPS_SUCCESS) return wRstPid;
          int32_t wRstFullName;
          assert(fullName);
          wRstFullName = caps->write(fullName->c_str());
          if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
          int32_t wRstSharedClean = caps->write((int64_t)sharedClean);
          if (wRstSharedClean != CAPS_SUCCESS) return wRstSharedClean;
          int32_t wRstSharedDirty = caps->write((int64_t)sharedDirty);
          if (wRstSharedDirty != CAPS_SUCCESS) return wRstSharedDirty;
          int32_t wRstPrivateClean = caps->write((int64_t)privateClean);
          if (wRstPrivateClean != CAPS_SUCCESS) return wRstPrivateClean;
          int32_t wRstPrivateDirty = caps->write((int64_t)privateDirty);
          if (wRstPrivateDirty != CAPS_SUCCESS) return wRstPrivateDirty;
          int32_t wRstPss = caps->write((int64_t)pss);
          if (wRstPss != CAPS_SUCCESS) return wRstPss;
          int32_t wRstRss = caps->write((int64_t)rss);
          if (wRstRss != CAPS_SUCCESS) return wRstRss;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          int32_t rRstPid = caps->read(pid);
          if (rRstPid != CAPS_SUCCESS) return rRstPid;
          if (!fullName) fullName = std::make_shared<std::string>();
          int32_t rRstFullName = caps->read_string(*fullName);
          if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
          int32_t rRstSharedClean = caps->read(sharedClean);
          if (rRstSharedClean != CAPS_SUCCESS) return rRstSharedClean;
          int32_t rRstSharedDirty = caps->read(sharedDirty);
          if (rRstSharedDirty != CAPS_SUCCESS) return rRstSharedDirty;
          int32_t rRstPrivateClean = caps->read(privateClean);
          if (rRstPrivateClean != CAPS_SUCCESS) return rRstPrivateClean;
          int32_t rRstPrivateDirty = caps->read(privateDirty);
          if (rRstPrivateDirty != CAPS_SUCCESS) return rRstPrivateDirty;
          int32_t rRstPss = caps->read(pss);
          if (rRstPss != CAPS_SUCCESS) return rRstPss;
          int32_t rRstRss = caps->read(rss);
          if (rRstRss != CAPS_SUCCESS) return rRstRss;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          int32_t rRstPid = caps->read(pid);
          if (rRstPid != CAPS_SUCCESS) return rRstPid;
          if (!fullName) fullName = std::make_shared<std::string>();
          int32_t rRstFullName = caps->read_string(*fullName);
          if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
          int32_t rRstSharedClean = caps->read(sharedClean);
          if (rRstSharedClean != CAPS_SUCCESS) return rRstSharedClean;
          int32_t rRstSharedDirty = caps->read(sharedDirty);
          if (rRstSharedDirty != CAPS_SUCCESS) return rRstSharedDirty;
          int32_t rRstPrivateClean = caps->read(privateClean);
          if (rRstPrivateClean != CAPS_SUCCESS) return rRstPrivateClean;
          int32_t rRstPrivateDirty = caps->read(privateDirty);
          if (rRstPrivateDirty != CAPS_SUCCESS) return rRstPrivateDirty;
          int32_t rRstPss = caps->read(pss);
          if (rRstPss != CAPS_SUCCESS) return rRstPss;
          int32_t rRstRss = caps->read(rss);
          if (rRstRss != CAPS_SUCCESS) return rRstRss;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          int32_t wRstPid = caps->write((uint32_t)pid);
          if (wRstPid != CAPS_SUCCESS) return wRstPid;
          int32_t wRstFullName;
          assert(fullName);
          wRstFullName = caps->write(fullName->c_str());
          if (wRstFullName != CAPS_SUCCESS) return wRstFullName;
          int32_t wRstSharedClean = caps->write((int64_t)sharedClean);
          if (wRstSharedClean != CAPS_SUCCESS) return wRstSharedClean;
          int32_t wRstSharedDirty = caps->write((int64_t)sharedDirty);
          if (wRstSharedDirty != CAPS_SUCCESS) return wRstSharedDirty;
          int32_t wRstPrivateClean = caps->write((int64_t)privateClean);
          if (wRstPrivateClean != CAPS_SUCCESS) return wRstPrivateClean;
          int32_t wRstPrivateDirty = caps->write((int64_t)privateDirty);
          if (wRstPrivateDirty != CAPS_SUCCESS) return wRstPrivateDirty;
          int32_t wRstPss = caps->write((int64_t)pss);
          if (wRstPss != CAPS_SUCCESS) return wRstPss;
          int32_t wRstRss = caps->write((int64_t)rss);
          if (wRstRss != CAPS_SUCCESS) return wRstRss;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          int32_t rRstPid = caps->read(pid);
          if (rRstPid != CAPS_SUCCESS) return rRstPid;
          if (!fullName) fullName = std::make_shared<std::string>();
          int32_t rRstFullName = caps->read_string(*fullName);
          if (rRstFullName != CAPS_SUCCESS) return rRstFullName;
          int32_t rRstSharedClean = caps->read(sharedClean);
          if (rRstSharedClean != CAPS_SUCCESS) return rRstSharedClean;
          int32_t rRstSharedDirty = caps->read(sharedDirty);
          if (rRstSharedDirty != CAPS_SUCCESS) return rRstSharedDirty;
          int32_t rRstPrivateClean = caps->read(privateClean);
          if (rRstPrivateClean != CAPS_SUCCESS) return rRstPrivateClean;
          int32_t rRstPrivateDirty = caps->read(privateDirty);
          if (rRstPrivateDirty != CAPS_SUCCESS) return rRstPrivateDirty;
          int32_t rRstPss = caps->read(pss);
          if (rRstPss != CAPS_SUCCESS) return rRstPss;
          int32_t rRstRss = caps->read(rss);
          if (rRstRss != CAPS_SUCCESS) return rRstRss;
          return CAPS_SUCCESS;
        }
    };

    class SysCPUCoreInfo {
    private:
        float usrUsage;
        float sysUsage;
        float idleUsage;
        float ioUsage;
        float busyUsage;
    public:
        static std::shared_ptr<SysCPUCoreInfo> create() {
          return std::make_shared<SysCPUCoreInfo>();
        }
        //getter 用户态cpu占用百分比
        float getUsrUsage() const { return usrUsage; }
        //getter 内核态cpu占用百分比
        float getSysUsage() const { return sysUsage; }
        //getter 空闲cpu占用百分比
        float getIdleUsage() const { return idleUsage; }
        //getter io等待百分比
        float getIoUsage() const { return ioUsage; }
        //getter cpu使用率
        float getBusyUsage() const { return busyUsage; }
        //setter 用户态cpu占用百分比
        void setUsrUsage(float v) { usrUsage = v; }
        //setter 内核态cpu占用百分比
        void setSysUsage(float v) { sysUsage = v; }
        //setter 空闲cpu占用百分比
        void setIdleUsage(float v) { idleUsage = v; }
        //setter io等待百分比
        void setIoUsage(float v) { ioUsage = v; }
        //setter cpu使用率
        void setBusyUsage(float v) { busyUsage = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_SYSCPUCOREINFO));
          int32_t wRstUsrUsage = caps->write((float)usrUsage);
          if (wRstUsrUsage != CAPS_SUCCESS) return wRstUsrUsage;
          int32_t wRstSysUsage = caps->write((float)sysUsage);
          if (wRstSysUsage != CAPS_SUCCESS) return wRstSysUsage;
          int32_t wRstIdleUsage = caps->write((float)idleUsage);
          if (wRstIdleUsage != CAPS_SUCCESS) return wRstIdleUsage;
          int32_t wRstIoUsage = caps->write((float)ioUsage);
          if (wRstIoUsage != CAPS_SUCCESS) return wRstIoUsage;
          int32_t wRstBusyUsage = caps->write((float)busyUsage);
          if (wRstBusyUsage != CAPS_SUCCESS) return wRstBusyUsage;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_SYSCPUCOREINFO));
          int32_t wRstUsrUsage = caps->write((float)usrUsage);
          if (wRstUsrUsage != CAPS_SUCCESS) return wRstUsrUsage;
          int32_t wRstSysUsage = caps->write((float)sysUsage);
          if (wRstSysUsage != CAPS_SUCCESS) return wRstSysUsage;
          int32_t wRstIdleUsage = caps->write((float)idleUsage);
          if (wRstIdleUsage != CAPS_SUCCESS) return wRstIdleUsage;
          int32_t wRstIoUsage = caps->write((float)ioUsage);
          if (wRstIoUsage != CAPS_SUCCESS) return wRstIoUsage;
          int32_t wRstBusyUsage = caps->write((float)busyUsage);
          if (wRstBusyUsage != CAPS_SUCCESS) return wRstBusyUsage;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          int32_t rRstUsrUsage = caps->read(usrUsage);
          if (rRstUsrUsage != CAPS_SUCCESS) return rRstUsrUsage;
          int32_t rRstSysUsage = caps->read(sysUsage);
          if (rRstSysUsage != CAPS_SUCCESS) return rRstSysUsage;
          int32_t rRstIdleUsage = caps->read(idleUsage);
          if (rRstIdleUsage != CAPS_SUCCESS) return rRstIdleUsage;
          int32_t rRstIoUsage = caps->read(ioUsage);
          if (rRstIoUsage != CAPS_SUCCESS) return rRstIoUsage;
          int32_t rRstBusyUsage = caps->read(busyUsage);
          if (rRstBusyUsage != CAPS_SUCCESS) return rRstBusyUsage;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          int32_t rRstUsrUsage = caps->read(usrUsage);
          if (rRstUsrUsage != CAPS_SUCCESS) return rRstUsrUsage;
          int32_t rRstSysUsage = caps->read(sysUsage);
          if (rRstSysUsage != CAPS_SUCCESS) return rRstSysUsage;
          int32_t rRstIdleUsage = caps->read(idleUsage);
          if (rRstIdleUsage != CAPS_SUCCESS) return rRstIdleUsage;
          int32_t rRstIoUsage = caps->read(ioUsage);
          if (rRstIoUsage != CAPS_SUCCESS) return rRstIoUsage;
          int32_t rRstBusyUsage = caps->read(busyUsage);
          if (rRstBusyUsage != CAPS_SUCCESS) return rRstBusyUsage;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          int32_t wRstUsrUsage = caps->write((float)usrUsage);
          if (wRstUsrUsage != CAPS_SUCCESS) return wRstUsrUsage;
          int32_t wRstSysUsage = caps->write((float)sysUsage);
          if (wRstSysUsage != CAPS_SUCCESS) return wRstSysUsage;
          int32_t wRstIdleUsage = caps->write((float)idleUsage);
          if (wRstIdleUsage != CAPS_SUCCESS) return wRstIdleUsage;
          int32_t wRstIoUsage = caps->write((float)ioUsage);
          if (wRstIoUsage != CAPS_SUCCESS) return wRstIoUsage;
          int32_t wRstBusyUsage = caps->write((float)busyUsage);
          if (wRstBusyUsage != CAPS_SUCCESS) return wRstBusyUsage;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          int32_t rRstUsrUsage = caps->read(usrUsage);
          if (rRstUsrUsage != CAPS_SUCCESS) return rRstUsrUsage;
          int32_t rRstSysUsage = caps->read(sysUsage);
          if (rRstSysUsage != CAPS_SUCCESS) return rRstSysUsage;
          int32_t rRstIdleUsage = caps->read(idleUsage);
          if (rRstIdleUsage != CAPS_SUCCESS) return rRstIdleUsage;
          int32_t rRstIoUsage = caps->read(ioUsage);
          if (rRstIoUsage != CAPS_SUCCESS) return rRstIoUsage;
          int32_t rRstBusyUsage = caps->read(busyUsage);
          if (rRstBusyUsage != CAPS_SUCCESS) return rRstBusyUsage;
          return CAPS_SUCCESS;
        }
    };

    class SysCPUInfo {
    private:
        std::shared_ptr<SysCPUCoreInfo> total;
        std::shared_ptr<std::vector<SysCPUCoreInfo>> cores;
    public:
        static std::shared_ptr<SysCPUInfo> create() {
          return std::make_shared<SysCPUInfo>();
        }
        //getter 总cpu信息
        const std::shared_ptr<SysCPUCoreInfo> & getTotal() const { return total; }
        //getter CPU核心信息
        const std::shared_ptr<std::vector<SysCPUCoreInfo>> getCores() const { return cores; }
        //setter 总cpu信息
        void setTotal(const std::shared_ptr<SysCPUCoreInfo> &v) { total = v; }
        //setter CPU核心信息
        void setCores(const std::shared_ptr<std::vector<SysCPUCoreInfo>> &v) { this->cores = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_SYSCPUINFO));
          std::shared_ptr<Caps> capsTotal;
          assert(total);
          int32_t sRstTotal = total->serializeForCapsObj(capsTotal);
          if (sRstTotal != CAPS_SUCCESS)
            return sRstTotal;
          else {
            int32_t wRst = caps->write(capsTotal);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          if (!cores)
            caps->write((int32_t)0);
          else {
            caps->write((int32_t)cores->size());
            for(auto &v : *cores) {
              std::shared_ptr<Caps> c;
              int32_t sRst = v.serializeForCapsObj(c);
              if (sRst != CAPS_SUCCESS)
                return sRst;
              else {
                int32_t wRst = caps->write(c);
                if (wRst != CAPS_SUCCESS) return wRst;
              }
            }
          }
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_SYSCPUINFO));
          std::shared_ptr<Caps> capsTotal;
          assert(total);
          int32_t sRstTotal = total->serializeForCapsObj(capsTotal);
          if (sRstTotal != CAPS_SUCCESS)
            return sRstTotal;
          else {
            int32_t wRst = caps->write(capsTotal);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          if (!cores)
            caps->write((int32_t)0);
          else {
            caps->write((int32_t)cores->size());
            for(auto &v : *cores) {
              std::shared_ptr<Caps> c;
              int32_t sRst = v.serializeForCapsObj(c);
              if (sRst != CAPS_SUCCESS)
                return sRst;
              else {
                int32_t wRst = caps->write(c);
                if (wRst != CAPS_SUCCESS) return wRst;
              }
            }
          }
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          std::shared_ptr<Caps> capsTotal;
          int32_t rRstTotal = caps->read(capsTotal);
          if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
          if (!total) total = std::make_shared<SysCPUCoreInfo>();
          rRstTotal = total->deserializeForCapsObj(capsTotal);
          if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
          int32_t arraySizeCores = 0;
          int32_t rRstCores = caps->read(arraySizeCores);
          if (rRstCores != CAPS_SUCCESS) return rRstCores;
          cores->clear();
          for(int32_t i = 0; i < arraySizeCores;++i) {
            std::shared_ptr<Caps> c;
            if (caps->read(c) == CAPS_SUCCESS && c) {
              cores->emplace_back();
              int32_t dRst = cores->back().deserializeForCapsObj(c);
              if (dRst != CAPS_SUCCESS) return dRst;
            }
          }
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          std::shared_ptr<Caps> capsTotal;
          int32_t rRstTotal = caps->read(capsTotal);
          if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
          if (!total) total = std::make_shared<SysCPUCoreInfo>();
          rRstTotal = total->deserializeForCapsObj(capsTotal);
          if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
          int32_t arraySizeCores = 0;
          int32_t rRstCores = caps->read(arraySizeCores);
          if (rRstCores != CAPS_SUCCESS) return rRstCores;
          cores->clear();
          for(int32_t i = 0; i < arraySizeCores;++i) {
            std::shared_ptr<Caps> c;
            if (caps->read(c) == CAPS_SUCCESS && c) {
              cores->emplace_back();
              int32_t dRst = cores->back().deserializeForCapsObj(c);
              if (dRst != CAPS_SUCCESS) return dRst;
            }
          }
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          std::shared_ptr<Caps> capsTotal;
          assert(total);
          int32_t sRstTotal = total->serializeForCapsObj(capsTotal);
          if (sRstTotal != CAPS_SUCCESS)
            return sRstTotal;
          else {
            int32_t wRst = caps->write(capsTotal);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          if (!cores)
            caps->write((int32_t)0);
          else {
            caps->write((int32_t)cores->size());
            for(auto &v : *cores) {
              std::shared_ptr<Caps> c;
              int32_t sRst = v.serializeForCapsObj(c);
              if (sRst != CAPS_SUCCESS)
                return sRst;
              else {
                int32_t wRst = caps->write(c);
                if (wRst != CAPS_SUCCESS) return wRst;
              }
            }
          }
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          std::shared_ptr<Caps> capsTotal;
          int32_t rRstTotal = caps->read(capsTotal);
          if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
          if (!total) total = std::make_shared<SysCPUCoreInfo>();
          rRstTotal = total->deserializeForCapsObj(capsTotal);
          if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
          int32_t arraySizeCores = 0;
          int32_t rRstCores = caps->read(arraySizeCores);
          if (rRstCores != CAPS_SUCCESS) return rRstCores;
          cores->clear();
          for(int32_t i = 0; i < arraySizeCores;++i) {
            std::shared_ptr<Caps> c;
            if (caps->read(c) == CAPS_SUCCESS && c) {
              cores->emplace_back();
              int32_t dRst = cores->back().deserializeForCapsObj(c);
              if (dRst != CAPS_SUCCESS) return dRst;
            }
          }
          return CAPS_SUCCESS;
        }
    };

    class SysMemInfo {
    private:
        int64_t total;
        int64_t free;
        int64_t buffers;
        int64_t cached;
        int64_t available;
    public:
        static std::shared_ptr<SysMemInfo> create() {
          return std::make_shared<SysMemInfo>();
        }
        //getter 总内存
        int64_t getTotal() const { return total; }
        //getter free内存
        int64_t getFree() const { return free; }
        //getter buffers内存
        int64_t getBuffers() const { return buffers; }
        //getter cached
        int64_t getCached() const { return cached; }
        //getter available
        int64_t getAvailable() const { return available; }
        //setter 总内存
        void setTotal(int64_t v) { total = v; }
        //setter free内存
        void setFree(int64_t v) { free = v; }
        //setter buffers内存
        void setBuffers(int64_t v) { buffers = v; }
        //setter cached
        void setCached(int64_t v) { cached = v; }
        //setter available
        void setAvailable(int64_t v) { available = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_SYSMEMINFO));
          int32_t wRstTotal = caps->write((int64_t)total);
          if (wRstTotal != CAPS_SUCCESS) return wRstTotal;
          int32_t wRstFree = caps->write((int64_t)free);
          if (wRstFree != CAPS_SUCCESS) return wRstFree;
          int32_t wRstBuffers = caps->write((int64_t)buffers);
          if (wRstBuffers != CAPS_SUCCESS) return wRstBuffers;
          int32_t wRstCached = caps->write((int64_t)cached);
          if (wRstCached != CAPS_SUCCESS) return wRstCached;
          int32_t wRstAvailable = caps->write((int64_t)available);
          if (wRstAvailable != CAPS_SUCCESS) return wRstAvailable;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_SYSMEMINFO));
          int32_t wRstTotal = caps->write((int64_t)total);
          if (wRstTotal != CAPS_SUCCESS) return wRstTotal;
          int32_t wRstFree = caps->write((int64_t)free);
          if (wRstFree != CAPS_SUCCESS) return wRstFree;
          int32_t wRstBuffers = caps->write((int64_t)buffers);
          if (wRstBuffers != CAPS_SUCCESS) return wRstBuffers;
          int32_t wRstCached = caps->write((int64_t)cached);
          if (wRstCached != CAPS_SUCCESS) return wRstCached;
          int32_t wRstAvailable = caps->write((int64_t)available);
          if (wRstAvailable != CAPS_SUCCESS) return wRstAvailable;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          int32_t rRstTotal = caps->read(total);
          if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
          int32_t rRstFree = caps->read(free);
          if (rRstFree != CAPS_SUCCESS) return rRstFree;
          int32_t rRstBuffers = caps->read(buffers);
          if (rRstBuffers != CAPS_SUCCESS) return rRstBuffers;
          int32_t rRstCached = caps->read(cached);
          if (rRstCached != CAPS_SUCCESS) return rRstCached;
          int32_t rRstAvailable = caps->read(available);
          if (rRstAvailable != CAPS_SUCCESS) return rRstAvailable;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          int32_t rRstTotal = caps->read(total);
          if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
          int32_t rRstFree = caps->read(free);
          if (rRstFree != CAPS_SUCCESS) return rRstFree;
          int32_t rRstBuffers = caps->read(buffers);
          if (rRstBuffers != CAPS_SUCCESS) return rRstBuffers;
          int32_t rRstCached = caps->read(cached);
          if (rRstCached != CAPS_SUCCESS) return rRstCached;
          int32_t rRstAvailable = caps->read(available);
          if (rRstAvailable != CAPS_SUCCESS) return rRstAvailable;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          int32_t wRstTotal = caps->write((int64_t)total);
          if (wRstTotal != CAPS_SUCCESS) return wRstTotal;
          int32_t wRstFree = caps->write((int64_t)free);
          if (wRstFree != CAPS_SUCCESS) return wRstFree;
          int32_t wRstBuffers = caps->write((int64_t)buffers);
          if (wRstBuffers != CAPS_SUCCESS) return wRstBuffers;
          int32_t wRstCached = caps->write((int64_t)cached);
          if (wRstCached != CAPS_SUCCESS) return wRstCached;
          int32_t wRstAvailable = caps->write((int64_t)available);
          if (wRstAvailable != CAPS_SUCCESS) return wRstAvailable;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          int32_t rRstTotal = caps->read(total);
          if (rRstTotal != CAPS_SUCCESS) return rRstTotal;
          int32_t rRstFree = caps->read(free);
          if (rRstFree != CAPS_SUCCESS) return rRstFree;
          int32_t rRstBuffers = caps->read(buffers);
          if (rRstBuffers != CAPS_SUCCESS) return rRstBuffers;
          int32_t rRstCached = caps->read(cached);
          if (rRstCached != CAPS_SUCCESS) return rRstCached;
          int32_t rRstAvailable = caps->read(available);
          if (rRstAvailable != CAPS_SUCCESS) return rRstAvailable;
          return CAPS_SUCCESS;
        }
    };

    class CPUInfos {
    private:
        std::shared_ptr<std::vector<ProcCPUInfo>> procCpuList;
        std::shared_ptr<SysCPUInfo> sysCpu;
        int64_t timestamp;
    public:
        static std::shared_ptr<CPUInfos> create() {
          return std::make_shared<CPUInfos>();
        }
        //getter 进程cpu信息列表
        const std::shared_ptr<std::vector<ProcCPUInfo>> getProcCpuList() const { return procCpuList; }
        //getter 系统CPU状态
        const std::shared_ptr<SysCPUInfo> & getSysCpu() const { return sysCpu; }
        //getter 采集时间
        int64_t getTimestamp() const { return timestamp; }
        //setter 进程cpu信息列表
        void setProcCpuList(const std::shared_ptr<std::vector<ProcCPUInfo>> &v) { this->procCpuList = v; }
        //setter 系统CPU状态
        void setSysCpu(const std::shared_ptr<SysCPUInfo> &v) { sysCpu = v; }
        //setter 采集时间
        void setTimestamp(int64_t v) { timestamp = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_CPUINFOS));
          if (!procCpuList)
            caps->write((int32_t)0);
          else {
            caps->write((int32_t)procCpuList->size());
            for(auto &v : *procCpuList) {
              std::shared_ptr<Caps> c;
              int32_t sRst = v.serializeForCapsObj(c);
              if (sRst != CAPS_SUCCESS)
                return sRst;
              else {
                int32_t wRst = caps->write(c);
                if (wRst != CAPS_SUCCESS) return wRst;
              }
            }
          }
          std::shared_ptr<Caps> capsSysCpu;
          assert(sysCpu);
          int32_t sRstSysCpu = sysCpu->serializeForCapsObj(capsSysCpu);
          if (sRstSysCpu != CAPS_SUCCESS)
            return sRstSysCpu;
          else {
            int32_t wRst = caps->write(capsSysCpu);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_CPUINFOS));
          if (!procCpuList)
            caps->write((int32_t)0);
          else {
            caps->write((int32_t)procCpuList->size());
            for(auto &v : *procCpuList) {
              std::shared_ptr<Caps> c;
              int32_t sRst = v.serializeForCapsObj(c);
              if (sRst != CAPS_SUCCESS)
                return sRst;
              else {
                int32_t wRst = caps->write(c);
                if (wRst != CAPS_SUCCESS) return wRst;
              }
            }
          }
          std::shared_ptr<Caps> capsSysCpu;
          assert(sysCpu);
          int32_t sRstSysCpu = sysCpu->serializeForCapsObj(capsSysCpu);
          if (sRstSysCpu != CAPS_SUCCESS)
            return sRstSysCpu;
          else {
            int32_t wRst = caps->write(capsSysCpu);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          int32_t arraySizeProcCpuList = 0;
          int32_t rRstProcCpuList = caps->read(arraySizeProcCpuList);
          if (rRstProcCpuList != CAPS_SUCCESS) return rRstProcCpuList;
          procCpuList->clear();
          for(int32_t i = 0; i < arraySizeProcCpuList;++i) {
            std::shared_ptr<Caps> c;
            if (caps->read(c) == CAPS_SUCCESS && c) {
              procCpuList->emplace_back();
              int32_t dRst = procCpuList->back().deserializeForCapsObj(c);
              if (dRst != CAPS_SUCCESS) return dRst;
            }
          }
          std::shared_ptr<Caps> capsSysCpu;
          int32_t rRstSysCpu = caps->read(capsSysCpu);
          if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
          if (!sysCpu) sysCpu = std::make_shared<SysCPUInfo>();
          rRstSysCpu = sysCpu->deserializeForCapsObj(capsSysCpu);
          if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          int32_t arraySizeProcCpuList = 0;
          int32_t rRstProcCpuList = caps->read(arraySizeProcCpuList);
          if (rRstProcCpuList != CAPS_SUCCESS) return rRstProcCpuList;
          procCpuList->clear();
          for(int32_t i = 0; i < arraySizeProcCpuList;++i) {
            std::shared_ptr<Caps> c;
            if (caps->read(c) == CAPS_SUCCESS && c) {
              procCpuList->emplace_back();
              int32_t dRst = procCpuList->back().deserializeForCapsObj(c);
              if (dRst != CAPS_SUCCESS) return dRst;
            }
          }
          std::shared_ptr<Caps> capsSysCpu;
          int32_t rRstSysCpu = caps->read(capsSysCpu);
          if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
          if (!sysCpu) sysCpu = std::make_shared<SysCPUInfo>();
          rRstSysCpu = sysCpu->deserializeForCapsObj(capsSysCpu);
          if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          if (!procCpuList)
            caps->write((int32_t)0);
          else {
            caps->write((int32_t)procCpuList->size());
            for(auto &v : *procCpuList) {
              std::shared_ptr<Caps> c;
              int32_t sRst = v.serializeForCapsObj(c);
              if (sRst != CAPS_SUCCESS)
                return sRst;
              else {
                int32_t wRst = caps->write(c);
                if (wRst != CAPS_SUCCESS) return wRst;
              }
            }
          }
          std::shared_ptr<Caps> capsSysCpu;
          assert(sysCpu);
          int32_t sRstSysCpu = sysCpu->serializeForCapsObj(capsSysCpu);
          if (sRstSysCpu != CAPS_SUCCESS)
            return sRstSysCpu;
          else {
            int32_t wRst = caps->write(capsSysCpu);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          int32_t arraySizeProcCpuList = 0;
          int32_t rRstProcCpuList = caps->read(arraySizeProcCpuList);
          if (rRstProcCpuList != CAPS_SUCCESS) return rRstProcCpuList;
          procCpuList->clear();
          for(int32_t i = 0; i < arraySizeProcCpuList;++i) {
            std::shared_ptr<Caps> c;
            if (caps->read(c) == CAPS_SUCCESS && c) {
              procCpuList->emplace_back();
              int32_t dRst = procCpuList->back().deserializeForCapsObj(c);
              if (dRst != CAPS_SUCCESS) return dRst;
            }
          }
          std::shared_ptr<Caps> capsSysCpu;
          int32_t rRstSysCpu = caps->read(capsSysCpu);
          if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
          if (!sysCpu) sysCpu = std::make_shared<SysCPUInfo>();
          rRstSysCpu = sysCpu->deserializeForCapsObj(capsSysCpu);
          if (rRstSysCpu != CAPS_SUCCESS) return rRstSysCpu;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
    };

    class MemInfos {
    private:
        std::shared_ptr<std::vector<ProcMemInfo>> procMemInfo;
        std::shared_ptr<SysMemInfo> sysMem;
        int64_t timestamp;
    public:
        static std::shared_ptr<MemInfos> create() {
          return std::make_shared<MemInfos>();
        }
        //getter 进程mem信息列表
        const std::shared_ptr<std::vector<ProcMemInfo>> getProcMemInfo() const { return procMemInfo; }
        //getter 系统Mem状态
        const std::shared_ptr<SysMemInfo> & getSysMem() const { return sysMem; }
        //getter 采集时间
        int64_t getTimestamp() const { return timestamp; }
        //setter 进程mem信息列表
        void setProcMemInfo(const std::shared_ptr<std::vector<ProcMemInfo>> &v) { this->procMemInfo = v; }
        //setter 系统Mem状态
        void setSysMem(const std::shared_ptr<SysMemInfo> &v) { sysMem = v; }
        //setter 采集时间
        void setTimestamp(int64_t v) { timestamp = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_MEMINFOS));
          if (!procMemInfo)
            caps->write((int32_t)0);
          else {
            caps->write((int32_t)procMemInfo->size());
            for(auto &v : *procMemInfo) {
              std::shared_ptr<Caps> c;
              int32_t sRst = v.serializeForCapsObj(c);
              if (sRst != CAPS_SUCCESS)
                return sRst;
              else {
                int32_t wRst = caps->write(c);
                if (wRst != CAPS_SUCCESS) return wRst;
              }
            }
          }
          std::shared_ptr<Caps> capsSysMem;
          assert(sysMem);
          int32_t sRstSysMem = sysMem->serializeForCapsObj(capsSysMem);
          if (sRstSysMem != CAPS_SUCCESS)
            return sRstSysMem;
          else {
            int32_t wRst = caps->write(capsSysMem);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_MEMINFOS));
          if (!procMemInfo)
            caps->write((int32_t)0);
          else {
            caps->write((int32_t)procMemInfo->size());
            for(auto &v : *procMemInfo) {
              std::shared_ptr<Caps> c;
              int32_t sRst = v.serializeForCapsObj(c);
              if (sRst != CAPS_SUCCESS)
                return sRst;
              else {
                int32_t wRst = caps->write(c);
                if (wRst != CAPS_SUCCESS) return wRst;
              }
            }
          }
          std::shared_ptr<Caps> capsSysMem;
          assert(sysMem);
          int32_t sRstSysMem = sysMem->serializeForCapsObj(capsSysMem);
          if (sRstSysMem != CAPS_SUCCESS)
            return sRstSysMem;
          else {
            int32_t wRst = caps->write(capsSysMem);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          int32_t arraySizeProcMemInfo = 0;
          int32_t rRstProcMemInfo = caps->read(arraySizeProcMemInfo);
          if (rRstProcMemInfo != CAPS_SUCCESS) return rRstProcMemInfo;
          procMemInfo->clear();
          for(int32_t i = 0; i < arraySizeProcMemInfo;++i) {
            std::shared_ptr<Caps> c;
            if (caps->read(c) == CAPS_SUCCESS && c) {
              procMemInfo->emplace_back();
              int32_t dRst = procMemInfo->back().deserializeForCapsObj(c);
              if (dRst != CAPS_SUCCESS) return dRst;
            }
          }
          std::shared_ptr<Caps> capsSysMem;
          int32_t rRstSysMem = caps->read(capsSysMem);
          if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
          if (!sysMem) sysMem = std::make_shared<SysMemInfo>();
          rRstSysMem = sysMem->deserializeForCapsObj(capsSysMem);
          if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          int32_t arraySizeProcMemInfo = 0;
          int32_t rRstProcMemInfo = caps->read(arraySizeProcMemInfo);
          if (rRstProcMemInfo != CAPS_SUCCESS) return rRstProcMemInfo;
          procMemInfo->clear();
          for(int32_t i = 0; i < arraySizeProcMemInfo;++i) {
            std::shared_ptr<Caps> c;
            if (caps->read(c) == CAPS_SUCCESS && c) {
              procMemInfo->emplace_back();
              int32_t dRst = procMemInfo->back().deserializeForCapsObj(c);
              if (dRst != CAPS_SUCCESS) return dRst;
            }
          }
          std::shared_ptr<Caps> capsSysMem;
          int32_t rRstSysMem = caps->read(capsSysMem);
          if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
          if (!sysMem) sysMem = std::make_shared<SysMemInfo>();
          rRstSysMem = sysMem->deserializeForCapsObj(capsSysMem);
          if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          if (!procMemInfo)
            caps->write((int32_t)0);
          else {
            caps->write((int32_t)procMemInfo->size());
            for(auto &v : *procMemInfo) {
              std::shared_ptr<Caps> c;
              int32_t sRst = v.serializeForCapsObj(c);
              if (sRst != CAPS_SUCCESS)
                return sRst;
              else {
                int32_t wRst = caps->write(c);
                if (wRst != CAPS_SUCCESS) return wRst;
              }
            }
          }
          std::shared_ptr<Caps> capsSysMem;
          assert(sysMem);
          int32_t sRstSysMem = sysMem->serializeForCapsObj(capsSysMem);
          if (sRstSysMem != CAPS_SUCCESS)
            return sRstSysMem;
          else {
            int32_t wRst = caps->write(capsSysMem);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          int32_t arraySizeProcMemInfo = 0;
          int32_t rRstProcMemInfo = caps->read(arraySizeProcMemInfo);
          if (rRstProcMemInfo != CAPS_SUCCESS) return rRstProcMemInfo;
          procMemInfo->clear();
          for(int32_t i = 0; i < arraySizeProcMemInfo;++i) {
            std::shared_ptr<Caps> c;
            if (caps->read(c) == CAPS_SUCCESS && c) {
              procMemInfo->emplace_back();
              int32_t dRst = procMemInfo->back().deserializeForCapsObj(c);
              if (dRst != CAPS_SUCCESS) return dRst;
            }
          }
          std::shared_ptr<Caps> capsSysMem;
          int32_t rRstSysMem = caps->read(capsSysMem);
          if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
          if (!sysMem) sysMem = std::make_shared<SysMemInfo>();
          rRstSysMem = sysMem->deserializeForCapsObj(capsSysMem);
          if (rRstSysMem != CAPS_SUCCESS) return rRstSysMem;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
    };

    class CollectData {
    private:
        std::shared_ptr<CPUInfos> cpu;
        std::shared_ptr<MemInfos> mem;
        int64_t timestamp;
    public:
        static std::shared_ptr<CollectData> create() {
          return std::make_shared<CollectData>();
        }
        //getter CPU信息，包括系统CPU和进程CPU
        const std::shared_ptr<CPUInfos> & getCpu() const { return cpu; }
        //getter 内存信息，包括系统内存和进程内存
        const std::shared_ptr<MemInfos> & getMem() const { return mem; }
        //getter 采集时间
        int64_t getTimestamp() const { return timestamp; }
        //setter CPU信息，包括系统CPU和进程CPU
        void setCpu(const std::shared_ptr<CPUInfos> &v) { cpu = v; }
        //setter 内存信息，包括系统内存和进程内存
        void setMem(const std::shared_ptr<MemInfos> &v) { mem = v; }
        //setter 采集时间
        void setTimestamp(int64_t v) { timestamp = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_COLLECTDATA));
          std::shared_ptr<Caps> capsCpu;
          assert(cpu);
          int32_t sRstCpu = cpu->serializeForCapsObj(capsCpu);
          if (sRstCpu != CAPS_SUCCESS)
            return sRstCpu;
          else {
            int32_t wRst = caps->write(capsCpu);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          std::shared_ptr<Caps> capsMem;
          assert(mem);
          int32_t sRstMem = mem->serializeForCapsObj(capsMem);
          if (sRstMem != CAPS_SUCCESS)
            return sRstMem;
          else {
            int32_t wRst = caps->write(capsMem);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_COLLECTDATA));
          std::shared_ptr<Caps> capsCpu;
          assert(cpu);
          int32_t sRstCpu = cpu->serializeForCapsObj(capsCpu);
          if (sRstCpu != CAPS_SUCCESS)
            return sRstCpu;
          else {
            int32_t wRst = caps->write(capsCpu);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          std::shared_ptr<Caps> capsMem;
          assert(mem);
          int32_t sRstMem = mem->serializeForCapsObj(capsMem);
          if (sRstMem != CAPS_SUCCESS)
            return sRstMem;
          else {
            int32_t wRst = caps->write(capsMem);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          std::shared_ptr<Caps> capsCpu;
          int32_t rRstCpu = caps->read(capsCpu);
          if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
          if (!cpu) cpu = std::make_shared<CPUInfos>();
          rRstCpu = cpu->deserializeForCapsObj(capsCpu);
          if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
          std::shared_ptr<Caps> capsMem;
          int32_t rRstMem = caps->read(capsMem);
          if (rRstMem != CAPS_SUCCESS) return rRstMem;
          if (!mem) mem = std::make_shared<MemInfos>();
          rRstMem = mem->deserializeForCapsObj(capsMem);
          if (rRstMem != CAPS_SUCCESS) return rRstMem;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          std::shared_ptr<Caps> capsCpu;
          int32_t rRstCpu = caps->read(capsCpu);
          if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
          if (!cpu) cpu = std::make_shared<CPUInfos>();
          rRstCpu = cpu->deserializeForCapsObj(capsCpu);
          if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
          std::shared_ptr<Caps> capsMem;
          int32_t rRstMem = caps->read(capsMem);
          if (rRstMem != CAPS_SUCCESS) return rRstMem;
          if (!mem) mem = std::make_shared<MemInfos>();
          rRstMem = mem->deserializeForCapsObj(capsMem);
          if (rRstMem != CAPS_SUCCESS) return rRstMem;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          std::shared_ptr<Caps> capsCpu;
          assert(cpu);
          int32_t sRstCpu = cpu->serializeForCapsObj(capsCpu);
          if (sRstCpu != CAPS_SUCCESS)
            return sRstCpu;
          else {
            int32_t wRst = caps->write(capsCpu);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          std::shared_ptr<Caps> capsMem;
          assert(mem);
          int32_t sRstMem = mem->serializeForCapsObj(capsMem);
          if (sRstMem != CAPS_SUCCESS)
            return sRstMem;
          else {
            int32_t wRst = caps->write(capsMem);
            if (wRst != CAPS_SUCCESS) return wRst;
          }
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          std::shared_ptr<Caps> capsCpu;
          int32_t rRstCpu = caps->read(capsCpu);
          if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
          if (!cpu) cpu = std::make_shared<CPUInfos>();
          rRstCpu = cpu->deserializeForCapsObj(capsCpu);
          if (rRstCpu != CAPS_SUCCESS) return rRstCpu;
          std::shared_ptr<Caps> capsMem;
          int32_t rRstMem = caps->read(capsMem);
          if (rRstMem != CAPS_SUCCESS) return rRstMem;
          if (!mem) mem = std::make_shared<MemInfos>();
          rRstMem = mem->deserializeForCapsObj(capsMem);
          if (rRstMem != CAPS_SUCCESS) return rRstMem;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
    };

    class CrashLog {
    private:
        int32_t pid;
        std::shared_ptr<std::string> pName;
        std::shared_ptr<std::string> args;
        uint64_t runTime;
        uint64_t ts;
    public:
        static std::shared_ptr<CrashLog> create() {
          return std::make_shared<CrashLog>();
        }
        //getter 进程id
        int32_t getPid() const { return pid; }
        //getter 进程名
        const std::shared_ptr<std::string> getName() const { return pName; }
        //getter 进程运行参数
        const std::shared_ptr<std::string> getArgs() const { return args; }
        //getter 运行时长，单位s
        uint64_t getRunTime() const { return runTime; }
        //getter crash信息采集时间戳
        uint64_t getTs() const { return ts; }
        //setter 进程id
        void setPid(int32_t v) { pid = v; }
        //setter 进程名
        void setName(const std::shared_ptr<std::string> &v) { pName = v; }
        //setter 进程名
        void setName(const char* v) { if (!pName) pName = std::make_shared<std::string>(); *pName = v; }
        //setter 进程运行参数
        void setArgs(const std::shared_ptr<std::string> &v) { args = v; }
        //setter 进程运行参数
        void setArgs(const char* v) { if (!args) args = std::make_shared<std::string>(); *args = v; }
        //setter 运行时长，单位s
        void setRunTime(uint64_t v) { runTime = v; }
        //setter crash信息采集时间戳
        void setTs(uint64_t v) { ts = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_CRASHLOG));
          int32_t wRstPid = caps->write((int32_t)pid);
          if (wRstPid != CAPS_SUCCESS) return wRstPid;
          int32_t wRstName;
          assert(pName);
          wRstName = caps->write(pName->c_str());
          if (wRstName != CAPS_SUCCESS) return wRstName;
          int32_t wRstArgs;
          assert(args);
          wRstArgs = caps->write(args->c_str());
          if (wRstArgs != CAPS_SUCCESS) return wRstArgs;
          int32_t wRstRunTime = caps->write((uint64_t)runTime);
          if (wRstRunTime != CAPS_SUCCESS) return wRstRunTime;
          int32_t wRstTs = caps->write((uint64_t)ts);
          if (wRstTs != CAPS_SUCCESS) return wRstTs;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_CRASHLOG));
          int32_t wRstPid = caps->write((int32_t)pid);
          if (wRstPid != CAPS_SUCCESS) return wRstPid;
          int32_t wRstName;
          assert(pName);
          wRstName = caps->write(pName->c_str());
          if (wRstName != CAPS_SUCCESS) return wRstName;
          int32_t wRstArgs;
          assert(args);
          wRstArgs = caps->write(args->c_str());
          if (wRstArgs != CAPS_SUCCESS) return wRstArgs;
          int32_t wRstRunTime = caps->write((uint64_t)runTime);
          if (wRstRunTime != CAPS_SUCCESS) return wRstRunTime;
          int32_t wRstTs = caps->write((uint64_t)ts);
          if (wRstTs != CAPS_SUCCESS) return wRstTs;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          int32_t rRstPid = caps->read(pid);
          if (rRstPid != CAPS_SUCCESS) return rRstPid;
          if (!pName) pName = std::make_shared<std::string>();
          int32_t rRstName = caps->read_string(*pName);
          if (rRstName != CAPS_SUCCESS) return rRstName;
          if (!args) args = std::make_shared<std::string>();
          int32_t rRstArgs = caps->read_string(*args);
          if (rRstArgs != CAPS_SUCCESS) return rRstArgs;
          int32_t rRstRunTime = caps->read(runTime);
          if (rRstRunTime != CAPS_SUCCESS) return rRstRunTime;
          int32_t rRstTs = caps->read(ts);
          if (rRstTs != CAPS_SUCCESS) return rRstTs;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          int32_t rRstPid = caps->read(pid);
          if (rRstPid != CAPS_SUCCESS) return rRstPid;
          if (!pName) pName = std::make_shared<std::string>();
          int32_t rRstName = caps->read_string(*pName);
          if (rRstName != CAPS_SUCCESS) return rRstName;
          if (!args) args = std::make_shared<std::string>();
          int32_t rRstArgs = caps->read_string(*args);
          if (rRstArgs != CAPS_SUCCESS) return rRstArgs;
          int32_t rRstRunTime = caps->read(runTime);
          if (rRstRunTime != CAPS_SUCCESS) return rRstRunTime;
          int32_t rRstTs = caps->read(ts);
          if (rRstTs != CAPS_SUCCESS) return rRstTs;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          int32_t wRstPid = caps->write((int32_t)pid);
          if (wRstPid != CAPS_SUCCESS) return wRstPid;
          int32_t wRstName;
          assert(pName);
          wRstName = caps->write(pName->c_str());
          if (wRstName != CAPS_SUCCESS) return wRstName;
          int32_t wRstArgs;
          assert(args);
          wRstArgs = caps->write(args->c_str());
          if (wRstArgs != CAPS_SUCCESS) return wRstArgs;
          int32_t wRstRunTime = caps->write((uint64_t)runTime);
          if (wRstRunTime != CAPS_SUCCESS) return wRstRunTime;
          int32_t wRstTs = caps->write((uint64_t)ts);
          if (wRstTs != CAPS_SUCCESS) return wRstTs;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          int32_t rRstPid = caps->read(pid);
          if (rRstPid != CAPS_SUCCESS) return rRstPid;
          if (!pName) pName = std::make_shared<std::string>();
          int32_t rRstName = caps->read_string(*pName);
          if (rRstName != CAPS_SUCCESS) return rRstName;
          if (!args) args = std::make_shared<std::string>();
          int32_t rRstArgs = caps->read_string(*args);
          if (rRstArgs != CAPS_SUCCESS) return rRstArgs;
          int32_t rRstRunTime = caps->read(runTime);
          if (rRstRunTime != CAPS_SUCCESS) return rRstRunTime;
          int32_t rRstTs = caps->read(ts);
          if (rRstTs != CAPS_SUCCESS) return rRstTs;
          return CAPS_SUCCESS;
        }
    };

    class Log {
    private:
        std::shared_ptr<std::string> content;
    public:
        static std::shared_ptr<Log> create() {
          return std::make_shared<Log>();
        }
        //getter 日志内容
        const std::shared_ptr<std::string> getContent() const { return content; }
        //setter 日志内容
        void setContent(const std::shared_ptr<std::string> &v) { content = v; }
        //setter 日志内容
        void setContent(const char* v) { if (!content) content = std::make_shared<std::string>(); *content = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_LOG));
          int32_t wRstContent;
          assert(content);
          wRstContent = caps->write(content->c_str());
          if (wRstContent != CAPS_SUCCESS) return wRstContent;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_LOG));
          int32_t wRstContent;
          assert(content);
          wRstContent = caps->write(content->c_str());
          if (wRstContent != CAPS_SUCCESS) return wRstContent;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          if (!content) content = std::make_shared<std::string>();
          int32_t rRstContent = caps->read_string(*content);
          if (rRstContent != CAPS_SUCCESS) return rRstContent;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          if (!content) content = std::make_shared<std::string>();
          int32_t rRstContent = caps->read_string(*content);
          if (rRstContent != CAPS_SUCCESS) return rRstContent;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          int32_t wRstContent;
          assert(content);
          wRstContent = caps->write(content->c_str());
          if (wRstContent != CAPS_SUCCESS) return wRstContent;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          if (!content) content = std::make_shared<std::string>();
          int32_t rRstContent = caps->read_string(*content);
          if (rRstContent != CAPS_SUCCESS) return rRstContent;
          return CAPS_SUCCESS;
        }
    };

    class TaskCommand {
    private:
        int32_t taskId;
        std::shared_ptr<std::string> taskType;
        uint32_t shellId;
        std::shared_ptr<std::string> shellType;
        std::shared_ptr<std::string> shellContent;
        int64_t timestamp;
    public:
        static std::shared_ptr<TaskCommand> create() {
          return std::make_shared<TaskCommand>();
        }
        //getter 任务id
        int32_t getTaskId() const { return taskId; }
        //getter 任务类型
        const std::shared_ptr<std::string> getTaskType() const { return taskType; }
        //getter 脚本id
        uint32_t getShellId() const { return shellId; }
        //getter 脚本类型
        const std::shared_ptr<std::string> getShellType() const { return shellType; }
        //getter 脚本内容
        const std::shared_ptr<std::string> getShellContent() const { return shellContent; }
        //getter 命令时间
        int64_t getTimestamp() const { return timestamp; }
        //setter 任务id
        void setTaskId(int32_t v) { taskId = v; }
        //setter 任务类型
        void setTaskType(const std::shared_ptr<std::string> &v) { taskType = v; }
        //setter 任务类型
        void setTaskType(const char* v) { if (!taskType) taskType = std::make_shared<std::string>(); *taskType = v; }
        //setter 脚本id
        void setShellId(uint32_t v) { shellId = v; }
        //setter 脚本类型
        void setShellType(const std::shared_ptr<std::string> &v) { shellType = v; }
        //setter 脚本类型
        void setShellType(const char* v) { if (!shellType) shellType = std::make_shared<std::string>(); *shellType = v; }
        //setter 脚本内容
        void setShellContent(const std::shared_ptr<std::string> &v) { shellContent = v; }
        //setter 脚本内容
        void setShellContent(const char* v) { if (!shellContent) shellContent = std::make_shared<std::string>(); *shellContent = v; }
        //setter 命令时间
        void setTimestamp(int64_t v) { timestamp = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_TASKCOMMAND));
          int32_t wRstTaskId = caps->write((int32_t)taskId);
          if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
          int32_t wRstTaskType;
          assert(taskType);
          wRstTaskType = caps->write(taskType->c_str());
          if (wRstTaskType != CAPS_SUCCESS) return wRstTaskType;
          int32_t wRstShellId = caps->write((uint32_t)shellId);
          if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
          int32_t wRstShellType;
          assert(shellType);
          wRstShellType = caps->write(shellType->c_str());
          if (wRstShellType != CAPS_SUCCESS) return wRstShellType;
          int32_t wRstShellContent;
          assert(shellContent);
          wRstShellContent = caps->write(shellContent->c_str());
          if (wRstShellContent != CAPS_SUCCESS) return wRstShellContent;
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_TASKCOMMAND));
          int32_t wRstTaskId = caps->write((int32_t)taskId);
          if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
          int32_t wRstTaskType;
          assert(taskType);
          wRstTaskType = caps->write(taskType->c_str());
          if (wRstTaskType != CAPS_SUCCESS) return wRstTaskType;
          int32_t wRstShellId = caps->write((uint32_t)shellId);
          if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
          int32_t wRstShellType;
          assert(shellType);
          wRstShellType = caps->write(shellType->c_str());
          if (wRstShellType != CAPS_SUCCESS) return wRstShellType;
          int32_t wRstShellContent;
          assert(shellContent);
          wRstShellContent = caps->write(shellContent->c_str());
          if (wRstShellContent != CAPS_SUCCESS) return wRstShellContent;
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          int32_t rRstTaskId = caps->read(taskId);
          if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
          if (!taskType) taskType = std::make_shared<std::string>();
          int32_t rRstTaskType = caps->read_string(*taskType);
          if (rRstTaskType != CAPS_SUCCESS) return rRstTaskType;
          int32_t rRstShellId = caps->read(shellId);
          if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
          if (!shellType) shellType = std::make_shared<std::string>();
          int32_t rRstShellType = caps->read_string(*shellType);
          if (rRstShellType != CAPS_SUCCESS) return rRstShellType;
          if (!shellContent) shellContent = std::make_shared<std::string>();
          int32_t rRstShellContent = caps->read_string(*shellContent);
          if (rRstShellContent != CAPS_SUCCESS) return rRstShellContent;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          int32_t rRstTaskId = caps->read(taskId);
          if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
          if (!taskType) taskType = std::make_shared<std::string>();
          int32_t rRstTaskType = caps->read_string(*taskType);
          if (rRstTaskType != CAPS_SUCCESS) return rRstTaskType;
          int32_t rRstShellId = caps->read(shellId);
          if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
          if (!shellType) shellType = std::make_shared<std::string>();
          int32_t rRstShellType = caps->read_string(*shellType);
          if (rRstShellType != CAPS_SUCCESS) return rRstShellType;
          if (!shellContent) shellContent = std::make_shared<std::string>();
          int32_t rRstShellContent = caps->read_string(*shellContent);
          if (rRstShellContent != CAPS_SUCCESS) return rRstShellContent;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          int32_t wRstTaskId = caps->write((int32_t)taskId);
          if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
          int32_t wRstTaskType;
          assert(taskType);
          wRstTaskType = caps->write(taskType->c_str());
          if (wRstTaskType != CAPS_SUCCESS) return wRstTaskType;
          int32_t wRstShellId = caps->write((uint32_t)shellId);
          if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
          int32_t wRstShellType;
          assert(shellType);
          wRstShellType = caps->write(shellType->c_str());
          if (wRstShellType != CAPS_SUCCESS) return wRstShellType;
          int32_t wRstShellContent;
          assert(shellContent);
          wRstShellContent = caps->write(shellContent->c_str());
          if (wRstShellContent != CAPS_SUCCESS) return wRstShellContent;
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          int32_t rRstTaskId = caps->read(taskId);
          if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
          if (!taskType) taskType = std::make_shared<std::string>();
          int32_t rRstTaskType = caps->read_string(*taskType);
          if (rRstTaskType != CAPS_SUCCESS) return rRstTaskType;
          int32_t rRstShellId = caps->read(shellId);
          if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
          if (!shellType) shellType = std::make_shared<std::string>();
          int32_t rRstShellType = caps->read_string(*shellType);
          if (rRstShellType != CAPS_SUCCESS) return rRstShellType;
          if (!shellContent) shellContent = std::make_shared<std::string>();
          int32_t rRstShellContent = caps->read_string(*shellContent);
          if (rRstShellContent != CAPS_SUCCESS) return rRstShellContent;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
    };

    class TaskStatus {
    private:
        int32_t taskId;
        int32_t status;
        std::shared_ptr<std::string> message;
        int32_t shellId;
        int64_t timestamp;
    public:
        static std::shared_ptr<TaskStatus> create() {
          return std::make_shared<TaskStatus>();
        }
        //getter 设备sn
        int32_t getTaskId() const { return taskId; }
        //getter 任务状态：0-运行中, 1-执行失败, 2-正常结束, 3-准备中
        int32_t getStatus() const { return status; }
        //getter 状态描述
        const std::shared_ptr<std::string> getMessage() const { return message; }
        //getter 脚本id
        int32_t getShellId() const { return shellId; }
        //getter 采集时间
        int64_t getTimestamp() const { return timestamp; }
        //setter 设备sn
        void setTaskId(int32_t v) { taskId = v; }
        //setter 任务状态：0-运行中, 1-执行失败, 2-正常结束, 3-准备中
        void setStatus(int32_t v) { status = v; }
        //setter 状态描述
        void setMessage(const std::shared_ptr<std::string> &v) { message = v; }
        //setter 状态描述
        void setMessage(const char* v) { if (!message) message = std::make_shared<std::string>(); *message = v; }
        //setter 脚本id
        void setShellId(int32_t v) { shellId = v; }
        //setter 采集时间
        void setTimestamp(int64_t v) { timestamp = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_TASKSTATUS));
          int32_t wRstTaskId = caps->write((int32_t)taskId);
          if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
          int32_t wRstStatus = caps->write((int32_t)status);
          if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
          int32_t wRstMessage;
          assert(message);
          wRstMessage = caps->write(message->c_str());
          if (wRstMessage != CAPS_SUCCESS) return wRstMessage;
          int32_t wRstShellId = caps->write((int32_t)shellId);
          if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_TASKSTATUS));
          int32_t wRstTaskId = caps->write((int32_t)taskId);
          if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
          int32_t wRstStatus = caps->write((int32_t)status);
          if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
          int32_t wRstMessage;
          assert(message);
          wRstMessage = caps->write(message->c_str());
          if (wRstMessage != CAPS_SUCCESS) return wRstMessage;
          int32_t wRstShellId = caps->write((int32_t)shellId);
          if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          int32_t rRstTaskId = caps->read(taskId);
          if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
          int32_t rRstStatus = caps->read(status);
          if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
          if (!message) message = std::make_shared<std::string>();
          int32_t rRstMessage = caps->read_string(*message);
          if (rRstMessage != CAPS_SUCCESS) return rRstMessage;
          int32_t rRstShellId = caps->read(shellId);
          if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          int32_t rRstTaskId = caps->read(taskId);
          if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
          int32_t rRstStatus = caps->read(status);
          if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
          if (!message) message = std::make_shared<std::string>();
          int32_t rRstMessage = caps->read_string(*message);
          if (rRstMessage != CAPS_SUCCESS) return rRstMessage;
          int32_t rRstShellId = caps->read(shellId);
          if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          int32_t wRstTaskId = caps->write((int32_t)taskId);
          if (wRstTaskId != CAPS_SUCCESS) return wRstTaskId;
          int32_t wRstStatus = caps->write((int32_t)status);
          if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
          int32_t wRstMessage;
          assert(message);
          wRstMessage = caps->write(message->c_str());
          if (wRstMessage != CAPS_SUCCESS) return wRstMessage;
          int32_t wRstShellId = caps->write((int32_t)shellId);
          if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          int32_t rRstTaskId = caps->read(taskId);
          if (rRstTaskId != CAPS_SUCCESS) return rRstTaskId;
          int32_t rRstStatus = caps->read(status);
          if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
          if (!message) message = std::make_shared<std::string>();
          int32_t rRstMessage = caps->read_string(*message);
          if (rRstMessage != CAPS_SUCCESS) return rRstMessage;
          int32_t rRstShellId = caps->read(shellId);
          if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
    };

    class DeviceStatus {
    private:
        std::shared_ptr<std::string> sn;
        std::shared_ptr<std::string> version;
        int32_t status;
        int32_t shellId;
        int64_t timestamp;
    public:
        static std::shared_ptr<DeviceStatus> create() {
          return std::make_shared<DeviceStatus>();
        }
        //getter 设备sn
        const std::shared_ptr<std::string> getSn() const { return sn; }
        //getter 设备版本
        const std::shared_ptr<std::string> getVersion() const { return version; }
        //getter 设备状态：0-idle，1-exe
        int32_t getStatus() const { return status; }
        //getter 脚本id
        int32_t getShellId() const { return shellId; }
        //getter 采集时间
        int64_t getTimestamp() const { return timestamp; }
        //setter 设备sn
        void setSn(const std::shared_ptr<std::string> &v) { sn = v; }
        //setter 设备sn
        void setSn(const char* v) { if (!sn) sn = std::make_shared<std::string>(); *sn = v; }
        //setter 设备版本
        void setVersion(const std::shared_ptr<std::string> &v) { version = v; }
        //setter 设备版本
        void setVersion(const char* v) { if (!version) version = std::make_shared<std::string>(); *version = v; }
        //setter 设备状态：0-idle，1-exe
        void setStatus(int32_t v) { status = v; }
        //setter 脚本id
        void setShellId(int32_t v) { shellId = v; }
        //setter 采集时间
        void setTimestamp(int64_t v) { timestamp = v; }
        /*
         * serialize this object as buffer
        */
        int32_t serialize(void* buf, uint32_t bufsize) const {
          std::shared_ptr<Caps> caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_DEVICESTATUS));
          int32_t wRstSn;
          assert(sn);
          wRstSn = caps->write(sn->c_str());
          if (wRstSn != CAPS_SUCCESS) return wRstSn;
          int32_t wRstVersion;
          assert(version);
          wRstVersion = caps->write(version->c_str());
          if (wRstVersion != CAPS_SUCCESS) return wRstVersion;
          int32_t wRstStatus = caps->write((int32_t)status);
          if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
          int32_t wRstShellId = caps->write((int32_t)shellId);
          if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return caps->serialize(buf, bufsize);
        }
        /*
         * deserialize this object as caps (with message type)
         */
        int32_t serialize(std::shared_ptr<Caps> &caps) const {
          if (!caps)
            caps = Caps::new_instance();
          caps->write(static_cast<int32_t>(MessageType::TYPE_DEVICESTATUS));
          int32_t wRstSn;
          assert(sn);
          wRstSn = caps->write(sn->c_str());
          if (wRstSn != CAPS_SUCCESS) return wRstSn;
          int32_t wRstVersion;
          assert(version);
          wRstVersion = caps->write(version->c_str());
          if (wRstVersion != CAPS_SUCCESS) return wRstVersion;
          int32_t wRstStatus = caps->write((int32_t)status);
          if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
          int32_t wRstShellId = caps->write((int32_t)shellId);
          if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from buffer
         */
        int32_t deserialize(void* buf, uint32_t bufSize) {
          std::shared_ptr<Caps> caps;
          int32_t pRst = Caps::parse(buf, bufSize, caps);
          if(pRst != CAPS_SUCCESS) return pRst;
          if (!sn) sn = std::make_shared<std::string>();
          int32_t rRstSn = caps->read_string(*sn);
          if (rRstSn != CAPS_SUCCESS) return rRstSn;
          if (!version) version = std::make_shared<std::string>();
          int32_t rRstVersion = caps->read_string(*version);
          if (rRstVersion != CAPS_SUCCESS) return rRstVersion;
          int32_t rRstStatus = caps->read(status);
          if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
          int32_t rRstShellId = caps->read(shellId);
          if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (with message type)
         */
        int32_t deserialize(std::shared_ptr<Caps> &caps) {
          if (!sn) sn = std::make_shared<std::string>();
          int32_t rRstSn = caps->read_string(*sn);
          if (rRstSn != CAPS_SUCCESS) return rRstSn;
          if (!version) version = std::make_shared<std::string>();
          int32_t rRstVersion = caps->read_string(*version);
          if (rRstVersion != CAPS_SUCCESS) return rRstVersion;
          int32_t rRstStatus = caps->read(status);
          if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
          int32_t rRstShellId = caps->read(shellId);
          if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * serialize this object as caps (without message type)
         */
        int32_t serializeForCapsObj(std::shared_ptr<Caps> &caps) const {
          caps = Caps::new_instance();
          int32_t wRstSn;
          assert(sn);
          wRstSn = caps->write(sn->c_str());
          if (wRstSn != CAPS_SUCCESS) return wRstSn;
          int32_t wRstVersion;
          assert(version);
          wRstVersion = caps->write(version->c_str());
          if (wRstVersion != CAPS_SUCCESS) return wRstVersion;
          int32_t wRstStatus = caps->write((int32_t)status);
          if (wRstStatus != CAPS_SUCCESS) return wRstStatus;
          int32_t wRstShellId = caps->write((int32_t)shellId);
          if (wRstShellId != CAPS_SUCCESS) return wRstShellId;
          int32_t wRstTimestamp = caps->write((int64_t)timestamp);
          if (wRstTimestamp != CAPS_SUCCESS) return wRstTimestamp;
          return CAPS_SUCCESS;
        }
        /*
         * deserialize this object from caps (without message type)
         */
        int32_t deserializeForCapsObj(std::shared_ptr<Caps> &caps) {
          if (!sn) sn = std::make_shared<std::string>();
          int32_t rRstSn = caps->read_string(*sn);
          if (rRstSn != CAPS_SUCCESS) return rRstSn;
          if (!version) version = std::make_shared<std::string>();
          int32_t rRstVersion = caps->read_string(*version);
          if (rRstVersion != CAPS_SUCCESS) return rRstVersion;
          int32_t rRstStatus = caps->read(status);
          if (rRstStatus != CAPS_SUCCESS) return rRstStatus;
          int32_t rRstShellId = caps->read(shellId);
          if (rRstShellId != CAPS_SUCCESS) return rRstShellId;
          int32_t rRstTimestamp = caps->read(timestamp);
          if (rRstTimestamp != CAPS_SUCCESS) return rRstTimestamp;
          return CAPS_SUCCESS;
        }
    };

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
    static MessageType get_msg_type(const unsigned char * buff, int32_t buff_len, std::shared_ptr<Caps> &caps) {
      if (Caps::parse(buff, buff_len, caps) != CAPS_SUCCESS)
        goto ERROR;
      int32_t msg_type;
      if (caps->read(msg_type) != CAPS_SUCCESS)
        goto ERROR;
      return static_cast<MessageType>(msg_type);
      ERROR:
      return MessageType::TYPE_UNKNOWN;
    }
    static MessageType get_msg_type(std::shared_ptr<Caps> &caps) {
      int32_t msg_type;
      if (caps->read(msg_type) != CAPS_SUCCESS)
        goto ERROR;
      return static_cast<MessageType>(msg_type);
      ERROR:
      return MessageType::TYPE_UNKNOWN;
    }
}
#endif //_CAPSMESSAGE_H
