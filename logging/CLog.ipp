/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>

#include "Date.h"
#include "FileUtil.h"
#include "lexical_cast.h"

namespace {
inline std::string levelToString(cutil::CLog::Level lv) {
  std::string ret = "[CLog::";
  switch (lv) {
    case cutil::CLog::INFO:
      ret += "INFO:";
      break;
    case cutil::CLog::NOTICE:
      ret += "NOTI:";
      break;
    case cutil::CLog::WARN:
      ret += "WARN:";
      break;
    case cutil::CLog::ERROR:
      ret += "ERRO:";
      break;
    case cutil::CLog::CRITICAL:
      ret += "CRIT:";
      break;
    case cutil::CLog::ALERT:
      ret += "ALRT:";
      break;
    case cutil::CLog::EMERG:
    default:
      ret += "EMRG:";
      break;
  }

  std::string millisecond;
  ret += cutil::Date::getDateTimeString("%Y%m%d-%H%M%S.", &millisecond) +
         millisecond + "] ";

  return ret;
}
}

namespace cutil {

inline void CLog::operator()() {
  while (true) {
    FileWriteProxy* fwriter_old = NULL;
    {
      Lock lk(&fwriter_mutex);

      if (stop_flag) {
        return;
      }

      while (!rotate_at_first &&
             FileUtil::getFileSize(filename) < size_to_rotate && !stop_flag) {
        fwriter_wcond.wait(&fwriter_mutex);
      }

      if (stop_flag) {
        return;
      }

      // list log files.
      {
        int suffix = 0;
        while (true) {
          if (FileUtil::isExist(filename + "." +
                                lexical_cast<std::string>(suffix))) {
            ++suffix;
          } else {
            break;
          }
        }

        // rotate log files.
        for (int i = suffix - 1; i >= 0; --i) {
          FileUtil::move(filename + "." + lexical_cast<std::string>(i),
                         filename + "." + lexical_cast<std::string>(i + 1));
        }
        FileUtil::move(filename, filename + ".0");

        rotate_at_first = false;
      }

      // reopen fwriter_current
      fwriter_old = fwriter_current;
      fwriter_current = new FileWriteProxy();
      fwriter_current->open(filename, FileWriteProxy::WRITE);
    }
    fwriter_old->close();
    delete fwriter_old;
  }
}

inline CLog::CLog()
    : fwriter_current(NULL),
      stop_flag(false),
      size_to_rotate(0),
      rotate_at_first(false),
      log_level(INFO),
      mirror_stdout_flag(false) {}

inline CLog::CLog(const std::string& filename, uint32_t size_to_rotate,
                  bool rotate_at_first)
    : log_level(INFO), mirror_stdout_flag(false) {
  open(filename, size_to_rotate, rotate_at_first);
}

inline CLog::~CLog() { close(); }

inline bool CLog::open(const std::string& filename, uint32_t size_to_rotate,
                       bool rotate_at_first) {
  if (isOpen()) {
    return false;
  }

  FileWriteProxy* tmp = new FileWriteProxy();
  bool ret = tmp->open(filename, FileWriteProxy::APPEND);

  if (ret) {
    fwriter_current = tmp;
    this->filename = filename;
    this->size_to_rotate = size_to_rotate;
    this->rotate_at_first = rotate_at_first;
    stop_flag = false;
    start();
  } else {
    fwriter_current = NULL;
  }

  return ret;
}

inline bool CLog::close() {
  if (!isOpen()) {
    return false;
  }

  {
    Lock lk(&fwriter_mutex);
    stop_flag = true;
    fwriter_wcond.notify();
  }

  join();

  fwriter_current->close();
  delete fwriter_current;
  fwriter_current = NULL;

  return true;
}

inline bool CLog::isOpen() const { return (fwriter_current != NULL); }

// store log whose level is not less than log_level
inline void CLog::setLogLevel(Level log_level) { this->log_level = log_level; }

inline CLog::Level CLog::getLogLevel() const { return log_level; }

inline int CLog::write(Level lv, const std::string& message) {
  if (!isOpen() || lv < log_level) {
    return 0;
  }

  std::string buf = levelToString(lv) + message;

  Lock lk(&fwriter_mutex);
  fwriter_current->write(buf.c_str(), buf.size() + 1);  // including '\0'
  fwriter_wcond.notify();
  return buf.size();
}

inline int CLog::printf(Level lv, const char* format, ...) {
  if (!isOpen() || lv < log_level) {
    return 0;
  }

  std::string new_format = levelToString(lv) + format + '\n';

  va_list args;
  va_start(args, format);

  char* buf = NULL;
  int ret = ::vasprintf(&buf, new_format.c_str(), args);

  if (ret >= 0) {
    Lock lk(&fwriter_mutex);
    fwriter_current->write(buf, ret + 1);  // including '\0'
    fwriter_wcond.notify();
  }
  free(buf);
  va_end(args);

  return ret;
}

}  // namespace cutil
