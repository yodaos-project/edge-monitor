/*
 * Created by xiaofei.lan@rokid.com on 2019/05/09.
 */

#include "watch_pid.h"
#include "options.h"
#include "busy_box.h"
#include <rapidjson/istreamwrapper.h>

YODA_NS_BEGIN

WatchPID::WatchPID() : IJobExecutor("WatchPID"),
                       _workReq(nullptr) {
  std::string sysroot = Options::get<std::string>("sysroot", "");
  std::string confpath = Options::get<std::string>("conf", "");
  _scanDir = sysroot + "/proc";
  initConf(confpath);
}

WatchPID::~WatchPID() {
  ASSERT(_workReq == nullptr, "%s uv_work_t* _workReq not nullptr", _name.c_str());
}

void WatchPID::initConf(std::string path) {
  if (path.empty()) {
    LOG_INFO("Not found config file. will be watch all process.");
    return;
  }
  std::ifstream ifs(path);
  if (!ifs.is_open()) {
    LOG_INFO("cannot load config file from %s", path.c_str());
    return;
  }
  rapidjson::IStreamWrapper ifsWrapper(ifs);
  _doc.ParseStream(ifsWrapper);
  if (_doc.HasParseError()) {
    LOG_ERROR("config file parse error %s", path.c_str());
    return;
  }
  if (!_doc.HasMember("watch")) {
    LOG_INFO("Not found watch field. will be watch all process.");
    return;
  }
}

void WatchPID::execute() {
  ASSERT(!_workReq, "WatchPID is running");
  _workReq = new uv_work_t;
  UV_CB_WRAP1(_workReq, cb1, WatchPID, doCollect, uv_work_t);
  UV_CB_WRAP2(_workReq, cb2, WatchPID, afterCollect, uv_work_t, int);
  uv_queue_work(uv_default_loop(), _workReq, cb1, cb2);
}

void WatchPID::doCollect(uv_work_t *req) {
  busybox::getSystemTop(_scanDir);
}

void WatchPID::afterCollect(uv_work_t *req, int32_t status) {
  std::shared_ptr<std::map<uint32_t, std::string>> diff;
  diff = busybox::getSystemTopDiff();

  if (diff->empty()) {
    LOG_INFO("There is no program change.");
    YODA_SIXSIX_SAFE_FREE(_workReq);
    this->onJobDone();
    return;
  }
  const rapidjson::Value& watch = _doc["watch"];
  if (!watch.IsArray()) {
    for (auto p : *diff) {
      LOG_INFO("The program: %s was changed. PID: %d", p.second.c_str(), p.first);
    }
    YODA_SIXSIX_SAFE_FREE(_workReq);
    this->onJobDone();
    return;
  }
  std::string name;
  std::size_t found;
  rapidjson::SizeType len = watch.Size();
  for (rapidjson::SizeType i = 0; i < len; i++) {
    name = watch[i].GetString();
    for (auto p : *diff) {
      found = p.second.find(name);
      if (found != std::string::npos) {
        LOG_INFO("The program: %s was changed: PID: %d", name.c_str(), p.first);
        break;
      }
    }
  }
}

int WatchPID::stop() {
  if (!_workReq) {
    return 0;
  }
  int r = uv_cancel((uv_req_t *) _workReq);
  if (r == 0) {
    YODA_SIXSIX_SAFE_DELETE(_workReq);
  }
  return r;
}

YODA_NS_END