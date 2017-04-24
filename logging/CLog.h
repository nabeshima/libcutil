/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_CLOG_H_
#define _CUTIL_CLOG_H_

#include <list>
#include <string>

#include "FileWriteProxy.h"
#include "Thread.h"

namespace cutil {

class CLog : public Thread {
 public:
  enum Level {
    INFO = 0,
    NOTICE = 1,
    WARN = 2,
    ERROR = 3,
    CRITICAL = 4,
    ALERT = 5,
    EMERG = 6
  };

 private:
  FileWriteProxy* fwriter_current;

  Mutex fwriter_mutex;
  WaitCondition fwriter_wcond;
  bool stop_flag;

  std::string filename;
  uint32_t size_to_rotate;
  bool rotate_at_first;
  Level log_level;
  bool mirror_stdout_flag;

 protected:
  virtual void operator()();

 public:
  CLog();
  explicit CLog(const std::string& filename, uint32_t size_to_rotate = 0,
                bool rotate_at_first = false);
  virtual ~CLog();

  bool open(const std::string& filename, uint32_t size_to_rotate = 0,
            bool rotate_at_first = false);
  bool close();
  bool isOpen() const;

  // store log whose level is not less than log_level
  void setLogLevel(Level log_level);
  Level getLogLevel() const;

  int write(Level lv, const std::string& message);
  int printf(Level lv, const char* format, ...);
};

}  // namespace cutil

#include "CLog.ipp"

#endif  // _CUTIL_CLOG_H_
