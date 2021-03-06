//
// Created by ximin.chen@rokid.com on 2018/12/31.
//

#include "crash_reporter.h"
#include "zip.h"
#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"
#include "options.h"
#include "device_info.h"
#include "busy_box.h"
#include "util.h"

#define UPLOAD_PATH "/server/coredump/put"

YODA_NS_BEGIN

std::vector<std::string> coredumpSuffix = {
  "core",
};

CrashReporter::CrashReporter() :
  IMultiThreadExecutor("CrashReporter"),
  _scanDir({"/data"}),
  _uploadURL() {
  _uploadURL = Options::get<std::string>("uploadUrl", "");
  LOG_INFO("coredump upload: %s", _uploadURL.c_str());
  auto sysroot = Options::get<std::string>("sysroot", "");
  auto coredumpDir = Options::get<std::string>("coredumpDir", "");
  if (!coredumpDir.empty()) {
    _scanDir.push_back(sysroot + coredumpDir);
  }
}

CrashReporter::~CrashReporter() {
}

void CrashReporter::doExecute(uv_work_t *) {
  for (auto &dir : _scanDir) {
    Util::scanDir(dir, [this, &dir](const char *filename) {
      size_t namelength = strlen(filename);
      for (auto &suffix : coredumpSuffix) {
        size_t suffixlen = suffix.size();
        if (namelength < suffixlen) {
          continue;
        }
        const char *fileSuffix = filename + namelength - suffixlen;
        if (strcmp(fileSuffix, suffix.c_str()) == 0) {
          this->compressAndUpload(dir, filename);
          sleep(1);
        }
      }
    });
  }
}

void CrashReporter::compressAndUpload(const std::string &dir,
                                      const std::string &filename) {
  // compress -> upload -> unlink
  char zippath[256];
  char filepath[256];
  sprintf(filepath, "%s/%s", dir.c_str(), filename.c_str());
  sprintf(zippath, "%s/%s.zip", dir.c_str(), filename.c_str());
  zip_t *zip = zip_open(zippath, ZIP_DEFAULT_COMPRESSION_LEVEL, 'w');
  if (!zip) {
    LOG_ERROR("open zip %s error", zippath);
    return;
  }
  do {
    if (zip_entry_open(zip, filename.c_str()) != 0) {
      LOG_ERROR("open zip entry %s error", zippath);
      break;
    }
    if (zip_entry_fwrite(zip, filepath) != 0) {
      LOG_ERROR("open zip entry write %s error", zippath);
      break;
    }
    if (zip_entry_close(zip) != 0) {
      LOG_ERROR("zip entry close %s error", zippath);
      break;
    }
  } while(0);
  if (!zip) {
    return;
  }
  zip_close(zip);
  LOG_INFO("zip file path: %s", zippath);
  LOG_INFO("coredump file path: %s", filepath);
  FILE *zipfile = fopen(zippath, "r");
  if (!zipfile) {
    LOG_ERROR("open zip file %s error", zippath);
    unlink(zippath);
    return;
  }
  fseek(zipfile, 0, SEEK_END);
  long size = ftell(zipfile);
  if (size < 0) {
    LOG_ERROR("get file %s size error: %s", zippath, strerror(errno));
    return;
  }
  std::string buf(size, '\0');
  fseek(zipfile, 0, SEEK_SET);
  size_t r = fread(&buf[0], 1, size, zipfile);
  fclose(zipfile);
  unlink(zippath);
  if (r != (size_t)size) {
    LOG_ERROR("read zip file %s error", zippath);
    return;
  }

  LOG_INFO("zip size: %ld, read size: %zu", size, r);
  static const int32_t fieldsCount = 5;
  char coredumpFields[fieldsCount][64] = {{'\0'}};
  int tokCount = 0;
  const char *sep = ".";
  char *p = const_cast<char*>(filename.c_str());
  p = strtok(p, sep);
  while (p && tokCount < fieldsCount) {
    strcpy(coredumpFields[tokCount++], p);
    p = strtok(nullptr, sep);
  }
  char *binName = coredumpFields[0];
  char *reportTime = coredumpFields[1];
  char *appPid = coredumpFields[2];
  // char *unknownField = coredumpFields[3];
  // char *suffix = coredumpFields[4];
  if (tokCount != fieldsCount) {
    strcpy(binName, "unknown-bin");
    strcpy(reportTime, std::to_string(time(nullptr)).c_str());
    strcpy(appPid, "0");
  }
  std::string fullname = "unknown-name";
  int32_t pid;
  if (Util::lexicalCast<int32_t>(appPid, &pid)) {
    std::shared_ptr<ProcessTopInfo> process = busybox::getProcessTopCache(pid);
    if (process) {
      fullname = process->fullname;
    }
  }
  LOG_INFO("%s %s %s", binName, fullname.c_str(), appPid);
  RestClient::Connection *conn = new RestClient::Connection(_uploadURL);
  conn->AppendHeader("Content-Type", "application/zip");
  conn->AppendHeader("Content-Length", std::to_string(size));
  conn->AppendHeader("Device-SN", DeviceInfo::sn);
  conn->AppendHeader("Device-Type-Id", DeviceInfo::typeId);
  conn->AppendHeader("Image-Version", DeviceInfo::imageVersion);
  conn->AppendHeader("Lothal-Version", DeviceInfo::turenVersion);
  conn->AppendHeader("Product-Name", DeviceInfo::productName);
  conn->AppendHeader("VSP-Version", DeviceInfo::vspVersion);
  conn->AppendHeader("Report-Time", reportTime);
  conn->AppendHeader("Report-Type", std::to_string(0));
  conn->AppendHeader("APP-BinName", binName);
  conn->AppendHeader("APP-Fullname", fullname);
  conn->AppendHeader("APP-PID", appPid);
  conn->AppendHeader("APP-ARGS", "{}");
  LOG_INFO("uploading %s", zippath);
  while (true) {
    RestClient::Response res = conn->post(UPLOAD_PATH, buf);
    if (200 <= res.code && res.code < 300) {
      LOG_INFO("upload finish %s %d %s", zippath, res.code, res.body.c_str());
      unlink(filepath);
      break;
    } else {
      LOG_ERROR("upload error: %s %d %s", zippath, res.code, res.body.c_str());
      sleep(30);
    }
  }
  delete conn;
}

int CrashReporter::afterExecute(uv_work_t *, int status) {
  return 0;
}

YODA_NS_END
