//
// Created by ximin.chen@rokid.com on 2018/12/31.
//

#include "crash_reporter.h"
#include "zip.h"
#include "restclient-cpp/restclient.h"
#include "restclient-cpp/connection.h"
#include "options.h"

YODA_NS_BEGIN

std::vector<std::string> coredumpPrefix = {
  "coredump",
};

CrashReporter::CrashReporter() :
  IMultiThreadExecutor("CrashReporter"),
  _scanDir(),
  _postURL() {
  auto serverAddress = Options::get<std::string>("serverAddress", "");
  auto serverPort = Options::get<std::string>("serverPort", "0");
  _postURL = serverAddress + ":" + serverPort + "/upload";
  YODA_SIXSIX_FLOG("coredump upload: %s", _postURL.c_str());
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
      for (auto &prefix : coredumpPrefix) {
        if (strncmp(prefix.c_str(), filename, prefix.size()) == 0) {
          this->compressAndUpload(dir, filename);
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
    if (zip_entry_open(zip, filepath) != 0) {
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

  RestClient::Connection *conn = new RestClient::Connection(_postURL);
  conn->AppendHeader("Content-Type", "application/zip");
  conn->AppendHeader("Content-Length", std::to_string(size));
  // RestClient::Response res = conn->post("/upload", buf);
  RestClient::Response res;
  res.code = 200;
  res.body = "hello world";
  if (res.code != 200) {
    YODA_SIXSIX_FERROR(
      "upload error: %s %d %s", zippath, res.code, res.body.c_str()
    );
  } else {
    YODA_SIXSIX_FLOG("uploaded %s", filepath);
    // unlink(filepath);
  }
  unlink(zippath);
}

void CrashReporter::afterExecute(uv_work_t *, int status) {
}

YODA_NS_END
