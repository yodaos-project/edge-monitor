//
// Created by ximin.chen@rokid.com on 2018/11/22.
//

#include "./busy_box.h"

#define PROCPS_BUFSIZE 1024
#define LINE_BUF_SIZE 512

namespace {

std::string parseCmdline(const std::string &cmdlinePath) {
  std::string cmdline = yoda::Util::readSmallFile(cmdlinePath);
  std::string output;
  bool ignore0 = false;
  for (auto &c : cmdline) {
    if (c == '\0') {
      // skip consecutive '\0'
      if (!ignore0) {
        ignore0 = true;
        output.push_back(' ');
      }
    } else {
      ignore0 = false;
      output.push_back(c);
    }
  }
  return output;
}

int index_in_strings(const char *strings, const char *key) {
  int idx = 0;

  while (*strings) {
    if (strcmp(strings, key) == 0) {
      return idx;
    }
    strings += strlen(strings) + 1; /* skip NUL */
    idx++;
  }
  return -1;
}

int32_t get_shift_pages_to_kb() {
  int32_t n = getpagesize();
  int32_t shift_pages_to_bytes = 0;
  int32_t shift_pages_to_kb = 0;
  while (true) {
    n >>= 1;
    if (!n) break;
    shift_pages_to_bytes++;
  }
  shift_pages_to_kb = shift_pages_to_bytes - 10;
  return shift_pages_to_kb;
}

char *skip_whitespace(char *s) {
  while (*s == ' ' || *s == '\t') ++s;

  return s;
}

char *skip_non_whitespace(char *s) {
  while (*s != '\0' && *s != ' ' && (unsigned char) (*s - 9) > (13 - 9))
    s++;

  return s;
}

FILE *fopen_for_read(const char *path) {
  return fopen(path, "r");
}

char *is_prefixed_with(char *string, const char *key) {
  while (*key != '\0') {
    if (*key != *string)
      return nullptr;
    key++;
    string++;
  }
  return string;
}

char *skip_fields(char *str, int count) {
  do {
    while (*str++ != ' ')
      continue;
    /* we found a space char, str points after it */
  } while (--count);
  return str;
}

unsigned long fast_strtoul_16(char **endptr) {
  char c;
  char *str = *endptr;
  unsigned long n = 0;

  /* Need to stop on both ' ' and '\n' */
  while ((c = *str++) > ' ') {
    c = ((c | (char) 0x20) - '0');
    if (c > 9)
      /* c = c + '0' - 'a' + 10: */
      c = c - ('a' - '0' - (char) 10);
    n = n * 16 + c;
  }
  *endptr = str; /* We skip trailing space! */
  return n;
}

template<typename T=unsigned long>
T fast_strtoul_10(char **endptr) {
  char c;
  char *str = *endptr;
  T n = *str - '0';

  /* Need to stop on both ' ' and '\n' */
  while ((c = *++str) > ' ')
    n = n * 10 + (c - '0');

  *endptr = str + 1; /* We skip trailing space! */
  return n;
}

std::map<int32_t, std::shared_ptr<ProcessTopInfo>> processesJif;
std::shared_ptr<SystemCPUInfo> cpuTotalJif(nullptr);
std::shared_ptr<SystemCPUInfo> cpuTotalPrevJif(nullptr);
std::vector<std::shared_ptr<SystemCPUInfo>> cpuCoresJif;
std::vector<std::shared_ptr<SystemCPUInfo>> cpuCoresPrevJif;
char line_buf[LINE_BUF_SIZE] = {0};

}

namespace busybox {

std::shared_ptr<ProcessTopInfo>
getProcessTop(const std::string &dir, uint32_t pid) {
  std::string buf(yoda::Util::readSmallFile(dir + "/stat"));
  if (buf.empty()) {
    return nullptr;
  }
  std::shared_ptr<ProcessTopInfo> stat(new ProcessTopInfo);
  stat->pid = pid;
  size_t commStart = buf.find_first_of('(');
  size_t commEnd = buf.find_first_of(')');
  stat->comm = buf.substr(commStart + 1, commEnd - commStart - 1);
  stat->cmdline = parseCmdline(dir + "/cmdline");
  stat->fullname = stat->cmdline.empty() ? stat->comm : stat->cmdline;
  yoda::Util::replaceChar(stat->fullname, "\r\n", ' ');
  memset(stat->state  , '\0', sizeof(stat->state));
  stat->state[0] = buf.at(commEnd + 2);

  char *cp = &buf[0] + commEnd + 4;
  stat->ppid = (uint32_t) fast_strtoul_10(&cp);
  stat->pgid = (uint32_t) fast_strtoul_10(&cp);
  stat->sid = (uint32_t) fast_strtoul_10(&cp);
  auto tty = (int32_t) fast_strtoul_10(&cp);
  stat->ttyMajor = (uint32_t) (tty >> 8) & 0xfff;
  stat->ttyMinor = (uint32_t) (tty & 0xff) | ((tty >> 12) & 0xfff00);
  cp = skip_fields(cp, 6); /* tpgid, flags, min_flt, cmin_flt, maj_flt, cmaj_flt */
  stat->utime = fast_strtoul_10(&cp);
  stat->stime = fast_strtoul_10(&cp);
  stat->ticks = stat->utime + stat->stime;
  cp = skip_fields(cp, 3); /* cutime, cstime, priority */
  stat->nice = (*cp == '-' ? ++cp, -1 : 1) * fast_strtoul_10(&cp);
  cp = skip_fields(cp, 2); /* timeout, it_real_value */
  stat->startTime = fast_strtoul_10(&cp);
  /* vsz is in bytes and we want kb */
  stat->vsz = fast_strtoul_10(&cp) >> 10;
  /* vsz is in bytes but rss is in *PAGES*! Can you believe that? */
  stat->rss = fast_strtoul_10(&cp) << get_shift_pages_to_kb();
  /* (6): rss_rlim, start_code, end_code, start_stack, kstk_esp, kstk_eip */
  /* (4): signal, blocked, sigignore, sigcatch */
  /* (4): wchan, nswap, cnswap, exit_signal */
  cp = skip_fields(cp, 14);
  // FIXME: is it safe to assume this field exists?
  stat->lastSeenOnCPU = (int32_t) fast_strtoul_10(&cp);
  stat->state[1] = ' ';
  stat->state[2] = ' ';
  uint32_t s_idx = 1;
  if (stat->vsz == 0 && stat->state[0] != 'Z') {
    /* not sure what the purpose of this flag */
    stat->state[1] = 'W';
    s_idx = 2;
  }
  if (stat->nice != 0) {
    stat->state[s_idx] = stat->nice < 0 ? '<' : 'N';
  }

  return stat;
}

#define SCAN(S, X) \
  if ((tp = is_prefixed_with(buf, S)) != nullptr) {         \
    tp = skip_whitespace(tp);                               \
    total->X += currec->X = fast_strtoul_10(&tp);           \
    /*if (currec->X > 0) {                                    \
      auto ite = total->sections.find(currec->fullname);   \
      if (ite == total->sections.end()) {                   \
        total->sections.insert({currec->fullname, currec});\
      } else {                                              \
        ite->second->X += currec->X;                        \
      }                                                     \
    } */                                                     \
    continue;                                               \
  }

std::shared_ptr<ProcessSmapInfo> getProcessSmap(const std::string &dir,
                                                uint32_t pid) {
  std::string filename = dir + "/smaps";
  char buf[PROCPS_BUFSIZE];

  FILE *file = fopen_for_read(filename.c_str());
  if (!file) {
    return nullptr;
  }
  std::shared_ptr<ProcessSmapInfo> total(new ProcessSmapInfo);
  total->pid = pid;
  total->comm = yoda::Util::readSmallFile(dir + "/comm");
  total->cmdline = parseCmdline(dir + "/cmdline");
  total->fullname = total->cmdline.empty() ? total->comm : total->cmdline;
  std::shared_ptr<ProcessSmapInfo> currec(new ProcessSmapInfo);

  while (fgets(buf, PROCPS_BUFSIZE, file)) {
    // Each mapping datum has this form:
    // f7d29000-f7d39000 rw-s FILEOFS M:m INODE FILENAME
    // Size:                nnn kB
    // Rss:                 nnn kB
    // .....

    char *tp, *p;

    SCAN("Pss:", pss);
    SCAN("Swap:", swap);
    SCAN("Rss:", rss);
    SCAN("Private_Dirty:", private_dirty);
    SCAN("Private_Clean:", private_clean);
    SCAN("Shared_Dirty:", shared_dirty);
    SCAN("Shared_Clean:", shared_clean);
    tp = strchr(buf, '-');
    if (tp) {
      // We reached next mapping - the line of this form:
      // f7d29000-f7d39000 rw-s FILEOFS M:m INODE FILENAME

      currec = std::make_shared<ProcessSmapInfo>();

      *tp = ' ';
      tp = buf;
      currec->start = fast_strtoul_16(&tp);
      currec->size = (fast_strtoul_16(&tp) - currec->start) >> 10;

      strncpy(currec->smap_mode, tp, sizeof(currec->smap_mode) - 1);

      // skipping "rw-s FILEOFS M:m INODE "
      tp = skip_whitespace(skip_fields(tp, 4));
      // filter out /dev/something (something != zero)
      if (!is_prefixed_with(tp, "/dev/") || strcmp(tp, "/dev/zero\n") == 0) {
        if (currec->smap_mode[1] == 'w') {
          currec->rw = currec->size;
          total->rw += currec->size;
        } else if (currec->smap_mode[1] == '-') {
          currec->ro = currec->size;
          total->ro += currec->size;
        }
      }

      if (strcmp(tp, "[stack]\n") == 0) {
        total->stack += currec->size;
      }
      p = skip_non_whitespace(tp);
      if (p == tp) {
        currec->fullname = "  [ anon ]";
      } else {
        *p = '\0';
        currec->fullname = tp;
      }
      total->size += currec->size;
    }
  }
  fclose(file);

  return total;
}

std::shared_ptr<SystemMemoryInfo> getSystemMemory(const std::string &dir) {
  static const char *fields =
    "MemTotal\0"
    "MemFree\0"
    "Buffers\0"
    "MemAvailable\0"
    "Cached\0";
  std::shared_ptr<SystemMemoryInfo> meminfo(new SystemMemoryInfo);
  auto pmeminfo = (uint64_t *) meminfo.get();
  std::string meminfoFile = dir + "/meminfo";
  FILE *file = fopen_for_read(meminfoFile.c_str());
  if (!file) {
    return nullptr;
  }
  char buf[60]; /* actual lines we expect are ~30 chars or less */
  while (fgets(buf, sizeof(buf), file)) {
    char *c = strchr(buf, ':');
    if (!c) {
      continue;
    }
    *c = '\0';
    int index = index_in_strings(fields, buf);
    if (index >= 0) {
      pmeminfo[index] = strtoul(c + 1, nullptr, 10);
    }
  }
  fclose(file);
  return meminfo;
}

#define CAL_PERCENT_1000(num, deno)\
  ((deno) ? std::floor(1000.0f * (num) / (deno)) / 10.0f : 0)

std::shared_ptr<SystemTopInfo> getSystemTop(const std::string &dir) {
  std::shared_ptr<SystemTopInfo> top(new SystemTopInfo);
  top->cpu = getCPUTop(dir);
  cpuTotalJif->processTickTotal = 0;

  std::vector<std::string> files(yoda::Util::getFileList(dir));
  bool isFirstTime = processesJif.empty();
  for (auto &file : files) {
    uint32_t pid;
    // determine if file is pid dir
    if (yoda::Util::lexicalCast<uint32_t>(file, &pid)) {
      std::string pidDir(dir);
      pidDir += "/" + file;
      if (yoda::Util::isDir(pidDir)) {
        std::shared_ptr<ProcessTopInfo> process(getProcessTop(pidDir, pid));
        if (!process) {
          continue;
        }
        top->processes.insert({process->pid, process});
        auto ite = processesJif.find(process->pid);
        if (ite != processesJif.end()) {
          process->ticksDelta = process->ticks - ite->second->ticks;
          cpuTotalJif->processTickTotal += process->ticksDelta;
          ite->second = process;
        } else {
          processesJif.insert({process->pid, process});
        }
      }
    }
  }
  /* This happens if there were lots of short-lived processes
  * between two top updates (e.g. compilation) */
  if (cpuTotalJif->processTickTotal < cpuTotalJif->busyDelta) {
    cpuTotalJif->processTickTotal = cpuTotalJif->busyDelta;
  }
  if (!isFirstTime) {
    auto cpup = float(cpuTotalJif->busyDelta) / cpuTotalJif->totalDelta;
    auto &totalTick = cpuTotalJif->processTickTotal;
    for (auto &pair : top->processes) {
      auto &p = pair.second;
      /*
       * CPU% = s->pcpu/sum(s->pcpu) * busy_cpu_ticks/total_cpu_ticks
       * (pcpu is delta of sys+user time between samples)
      */
      p->cpuUsagePercent = CAL_PERCENT_1000(p->ticksDelta * cpup, totalTick);
    }
  }
  processesJif = top->processes;

  return top;
}

static int32_t
readCPUJif(FILE *fp, const std::shared_ptr<SystemCPUInfo> &coreJif) {
  static const char fmt[] = "cp%*s %" PRIu64 " %" PRIu64 " %" PRIu64 \
      " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64 " %" PRIu64;

  /* not "cpu" */
  if (!fgets(line_buf, LINE_BUF_SIZE, fp) || line_buf[0] != 'c') {
    return -1;
  }
  int r = sscanf(line_buf, fmt,
                 &coreJif->usr, &coreJif->nic, &coreJif->sys, &coreJif->idle,
                 &coreJif->iowait, &coreJif->irq, &coreJif->softirq,
                 &coreJif->steal);
  if (r < 4) {
    return -2;
  }
  coreJif->total = coreJif->usr + coreJif->nic + coreJif->sys +
                   coreJif->idle + coreJif->iowait + coreJif->irq +
                   coreJif->softirq + coreJif->steal;
  /* procps 2.x does not count iowait as busy time */
  coreJif->busy = coreJif->total - coreJif->idle - coreJif->iowait;

  return 0;
}

#define CAL_CPU_USAGE_PERCENT(cur, prev)\
  cur->totalDelta = cur->total - prev->total;\
  cur->busyDelta = cur->busy - prev->busy;\
  cur->busyPercent = CAL_PERCENT_1000(cur->busyDelta, cur->totalDelta);\
  cur->idlePercent = CAL_PERCENT_1000(cur->idle - prev->idle, cur->totalDelta);\
  cur->sysPercent = CAL_PERCENT_1000(cur->sys - prev->sys, cur->totalDelta);\
  cur->usrPercent = CAL_PERCENT_1000(cur->usr - prev->usr, cur->totalDelta);

std::shared_ptr<SystemCPUDetailInfo> getCPUTop(const std::string &dir) {
  std::string statDir = dir + "/stat";
  FILE *fp = fopen(statDir.c_str(), "r");
  YODA_SIXSIX_FASSERT(fp, "read %s failed", dir.c_str());
  int32_t r;

  /* We need to parse cumulative counts even if SMP CPU display is on,
   * they are used to calculate per process CPU% */
  if (!cpuTotalJif) {
    /* First time here. How many CPUs?
     * There will be at least 1 /proc/stat line with cpu%d
     */
    YODA_SIXSIX_SLOG("reading first time");
    cpuTotalJif = std::make_shared<SystemCPUInfo>();
    cpuTotalPrevJif = std::make_shared<SystemCPUInfo>();
    r = readCPUJif(fp, cpuTotalJif);
    YODA_SIXSIX_FASSERT(r == 0, "read total cpu failed code %d", r);
    while (true) {
      std::shared_ptr<SystemCPUInfo> cpuCoreJif(new SystemCPUInfo);
      r = readCPUJif(fp, cpuCoreJif);
      if (r == 0) {
        cpuCoresJif.emplace_back(cpuCoreJif);
        cpuCoresPrevJif.emplace_back(std::make_shared<SystemCPUInfo>());
      } else {
        break;
      }
    }
  } else {
    cpuTotalJif.swap(cpuTotalPrevJif);
    r = readCPUJif(fp, cpuTotalJif);
    YODA_SIXSIX_FASSERT(r == 0, "read total cpu failed code %d", r);
    CAL_CPU_USAGE_PERCENT(cpuTotalJif, cpuTotalPrevJif);

    cpuCoresJif.swap(cpuCoresPrevJif);
    /* Get the new samples */
    for (size_t i = 0; i < cpuCoresJif.size(); ++i) {
      auto coreJif = cpuCoresJif.at(i);
      auto corePrevJif = cpuCoresPrevJif.at(i);
      r = readCPUJif(fp, coreJif);
      YODA_SIXSIX_FASSERT(r == 0, "read cpu %zu failed code %d", i, r);
      CAL_CPU_USAGE_PERCENT(coreJif, corePrevJif);
    }
  }
  fclose(fp);

  std::shared_ptr<SystemCPUDetailInfo> detail(new SystemCPUDetailInfo);
  detail->total = cpuTotalJif;
  detail->cores = cpuCoresJif;
  return detail;
}

}
