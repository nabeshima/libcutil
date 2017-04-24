/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_FILEWRITEPROXY_H_
#define _CUTIL_FILEWRITEPROXY_H_

#include <list>
#include <string>
#include <utility>
#include <vector>

#include "IOStream.h"
#include "Thread.h"

namespace cutil {

/*
 * File writing are processed later by another thread.
 * It could be helpful to avoid blocking file write and sync in real-time
 * application.
 * */
class FileWriteProxy : public Thread {
 private:
  FILE *fp;

  Mutex write_mutex;
  WaitCondition write_wcond;
  std::vector<char> write_queue;
  bool stop_flag;

  class OHolder {
   private:
    FileWriteProxy *fwp;
    std::string buf;

   public:
    explicit OHolder(FileWriteProxy *fwp);
    int write(const void *data, int size);
    int read(void *data, int n) { return 0; }
    bool isOpen() const;
  };

  OHolder oholder;
  OStream<OHolder> ostrm;

 protected:
  virtual void operator()();

 public:
  enum Mode { WRITE, APPEND };

  FileWriteProxy();
  explicit FileWriteProxy(const std::string &filename, Mode mode);
  virtual ~FileWriteProxy();

  bool open(const std::string &filename, Mode mode);
  bool close();
  bool isOpen() const;

  void write(const void *buf, int size);
  int printf(const char *format, ...);

  int byteInWriteQueue();
  void clearWriteQueue();

  std::ostream &ostream();
};
}  // namespace cutil

#include "FileWriteProxy.ipp"

#endif /* _CUTIL_FILEWRITEPROXY_H_*/
