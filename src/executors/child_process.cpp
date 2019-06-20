//
// Created by ximin.chen@rokid.com on 2018-11-28.
//

#include "child_process.h"
#include "options.h"

YODA_NS_BEGIN

ChildProcess::ChildProcess(const std::shared_ptr<JobConf> &conf) :
  IJobExecutor("ChildProcess"),
  _cp(),
  _conf(conf),
  _filePath{0},
  _code(0) {
  auto task = conf->task;
  ASSERT(task, "conf task is empty");
  auto unzipRoot = Options::get<std::string>("unzipRoot", "/tmp");
  sprintf(_filePath, "%s/yoda-sixsix-%d-%d.sh",
    unzipRoot.c_str(), task->id, task->shellId);
}

ChildProcess::~ChildProcess() {
  ASSERT(!_cp, "child process is not exit yet");
}

void ChildProcess::execute() {
  uv_fs_t openReq;
  int32_t r;
  auto loop = uv_default_loop();

  uv_fs_t unlinkReq;
  uv_fs_unlink(uv_default_loop(), &unlinkReq, _filePath, nullptr);
  uv_fs_req_cleanup(&unlinkReq);

  int32_t openFlags = O_WRONLY | O_CREAT | S_IWUSR | S_IRUSR;
  r = uv_fs_open(loop, &openReq, _filePath, openFlags, 0777, nullptr);
  ASSERT(r >= 0, "open %s error: %s", _filePath, uv_err_name(r));
  uv_fs_req_cleanup(&openReq);
  auto fd = (uv_file) openReq.result;

  auto shellSize = _conf->task->shell->size();
  auto bufSize = shellSize < 256 ? 256 : shellSize;
  auto buf = (char *) malloc(bufSize + 1);
  memcpy(buf, _conf->task->shell->c_str(), shellSize);
  buf[shellSize] = '\0';
  auto uv_buf = uv_buf_init(buf, (uint32_t) shellSize);
  uv_fs_t writeReq;
  r = uv_fs_write(loop, &writeReq, fd, &uv_buf, 1, -1, nullptr);
  ASSERT(r >= 0, "write %s error: %s", _filePath, uv_err_name(r));
  uv_fs_req_cleanup(&writeReq);

  uv_fs_t closeReq;
  r = uv_fs_close(loop, &closeReq, fd, nullptr);
  ASSERT(r >= 0, "close %s error: %s", _filePath, uv_err_name(r));
  uv_fs_req_cleanup(&closeReq);

  uv_stdio_container_t io[3];
  io[0].flags = UV_IGNORE;
  io[1].flags = UV_INHERIT_FD;
  io[1].data.fd = STDOUT_FILENO;
  io[2].flags = UV_INHERIT_FD;
  io[2].data.fd = STDERR_FILENO;

  uv_process_options_t options;
  memset(&options, 0, sizeof(uv_process_options_t));
  const char *args[2];
  args[0] = _filePath;
  args[1] = nullptr;
  options.cwd = getcwd(buf, bufSize);
  options.file = args[0];
  options.args = const_cast<char **>(args);
  options.stdio = io;
  options.stdio_count = sizeof(io) / sizeof(uv_stdio_container_t);
  _cp = (uv_process_t *) malloc(sizeof(uv_process_t));
  UV_CB_WRAP3(_cp, cb, ChildProcess, onChildProcessExit,
                   uv_process_t, int64_t, int32_t);
  options.exit_cb = cb;

  r = uv_spawn(uv_default_loop(), _cp, &options);
  ASSERT(r == 0, "spawn %s error: %s", _filePath, uv_err_name(r));

  free(buf);
}

int ChildProcess::stop() {
  if (_cp) {
    int32_t r = uv_process_kill(_cp, SIGTERM);
    if (r == UV_ESRCH) {
      r = 0;
    }
    if (r) {
      const char *err = uv_err_name(r);
      LOG_ERROR("stop child process %d error %s", _cp->pid, err);
    } else {
      LOG_INFO("stop child process %d succeed", _cp->pid);
    }
    return 1;
  }
  return 0;
}

void ChildProcess::onChildProcessExit(uv_process_t *,
                                      int64_t code,
                                      int32_t signal) {
  LOG_INFO("process exit: %" PRId64 " %d", code, signal);
  UV_CLOSE_HANDLE(_cp, ChildProcess, onUVHandleClosed);
  _code = code;
}

void ChildProcess::onUVHandleClosed(uv_handle_t *handle) {
  if ((uv_handle_t *) _cp == handle) {
    YODA_SIXSIX_SAFE_FREE(_cp);
  } else {
    LOG_INFO("cp receive unknown handle close, free it");
    YODA_SIXSIX_SAFE_FREE(handle);
  }
  LOG_INFO("child process closed");
  this->onJobDone(_code);
}

YODA_NS_END
