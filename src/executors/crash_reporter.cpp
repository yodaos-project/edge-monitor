//
// Created by ximin.chen@rokid.com on 2018/12/31.
//

#include "crash_reporter.h"
#include "zip.h"
#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"
#include "options.h"
#include "device_info.h"

YODA_NS_BEGIN

std::vector<std::string> coredumpSuffix = {
  "core",
};

CrashReporter::CrashReporter() :
  IMultiThreadExecutor("CrashReporter"),
  _scanDir({"/data"}),
  _uploadURL() {
  auto serverAddress = Options::get<std::string>("serverAddress", "");
  auto serverPort = Options::get<std::string>("serverPort", "0");
  _uploadURL = serverAddress + ":" + serverPort + "/upload";
  YODA_SIXSIX_FLOG("coredump upload: %s", _uploadURL.c_str());
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
    YODA_SIXSIX_FERROR("open zip %s error", zippath);
    return;
  }
  do {
    if (zip_entry_open(zip, filename.c_str()) != 0) {
      YODA_SIXSIX_FERROR("open zip entry %s error", zippath);
      break;
    }
    if (zip_entry_fwrite(zip, filepath) != 0) {
      YODA_SIXSIX_FERROR("open zip entry write %s error", zippath);
      break;
    }
    if (zip_entry_close(zip) != 0) {
      YODA_SIXSIX_FERROR("zip entry close %s error", zippath);
      break;
    }
  } while(0);
  if (!zip) {
    return;
  }
  zip_close(zip);
  std::ifstream ifs(zippath, std::ios::binary);
  if (!ifs.is_open()) {
    YODA_SIXSIX_FERROR("open zip file %s error", zippath);
    unlink(zippath);
    return;
  }
  ifs.seekg(0, std::ios::end);
  size_t size = ifs.tellg();
  std::string buf(size, '\0');
  ifs.seekg(0);
  ifs.read(&buf[0], size);
  ifs.close();

  static const int32_t fieldsCount = 6;
  char coredumpFields[fieldsCount][64] = { '\0' };
  int tokCount = 0;
  const char *sep = ".";
  char *p = const_cast<char*>(filename.c_str());
  p = strtok(p, sep);
  while (p && tokCount < fieldsCount) {
    strcpy(coredumpFields[tokCount++], p);
    p = strtok(nullptr, sep);
  }
  char *prefix = coredumpFields[0];
  char *binName = coredumpFields[1];
  char *reportTime = coredumpFields[2];
  char *appPid = coredumpFields[3];
  char *unknownField = coredumpFields[4];
  char *suffix = coredumpFields[5];
  if (tokCount != fieldsCount) {
    strcpy(binName, "unknownApp");
    strcpy(reportTime, std::to_string(time(nullptr)).c_str());
    strcpy(appPid, "0");
  }

  RestClient::Connection *conn = new RestClient::Connection(_uploadURL);
  conn->AppendHeader("Content-Type", "application/zip");
  conn->AppendHeader("Content-Length", std::to_string(size));
  conn->AppendHeader("Device-SN", DeviceInfo::sn);
  conn->AppendHeader("Report-Time", reportTime);
  conn->AppendHeader("Report-Type", std::to_string(0));
  conn->AppendHeader("APP-Fullname", binName);
  conn->AppendHeader("APP-PID", appPid);
  RestClient::Response res = conn->post("/upload", buf);
  if (200 <= res.code && res.code < 300) {
    YODA_SIXSIX_FLOG("uploaded %s", filepath);
    unlink(filepath);
  } else {
    YODA_SIXSIX_FERROR(
      "upload error: %s %d %s", zippath, res.code, res.body.c_str()
    );
  }
  unlink(zippath);
  delete conn;
}

void CrashReporter::afterExecute(uv_work_t *, int status) {
}

YODA_NS_END
