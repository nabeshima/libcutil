/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cstdarg>
#include <cstdio>
#include <unistd.h>

namespace cutil {

inline void FileWriteProxy::operator()() {
  while (true) {
    {
      Lock lk(&write_mutex);

      if (write_queue.empty() && stop_flag) {
        return;
      }

      while (write_queue.empty() && !stop_flag) {
        write_wcond.wait(&write_mutex);
      }

      if (write_queue.empty() && stop_flag) {
        return;
      }

      fwrite(write_queue.data(), sizeof(uint8_t), write_queue.size(), fp);
      write_queue.clear();
    }

    fflush(fp);

#ifdef F_FULLFSYNC
    fcntl(fileno(fp), F_FULLFSYNC);
#else
    fdatasync(fileno(fp));
#endif
  }
}

inline FileWriteProxy::FileWriteProxy()
    : fp(NULL), stop_flag(false), oholder(this), ostrm(&oholder) {}

inline FileWriteProxy::FileWriteProxy(const std::string &filename, Mode mode)
    : oholder(this), ostrm(&oholder) {
  open(filename, mode);
}

inline FileWriteProxy::~FileWriteProxy() { close(); }

inline bool FileWriteProxy::open(const std::string &filename, Mode mode) {
  if (!isOpen()) {
    FILE *tmp = fopen(filename.c_str(), ((mode == WRITE) ? "w" : "a"));

    if (tmp != NULL) {
      fp = tmp;
      stop_flag = false;
      start();
      return true;
    }
  }
  return false;
}

inline bool FileWriteProxy::close() {
  if (isOpen()) {
    {
      Lock lk(&write_mutex);
      stop_flag = true;
      write_wcond.notify();
    }
    join();

    int res = fclose(fp);
    fp = NULL;

    if (res != 0) {
      return false;
    }
  }
  return true;
}

inline bool FileWriteProxy::isOpen() const { return (fp != NULL); }

inline void FileWriteProxy::write(const void *buf, int size) {
  const char *b = (const char *)buf;
  Lock lk(&write_mutex);
  for (int i = 0; i < size; ++i) {
    write_queue.push_back(b[i]);
  }
  write_wcond.notify();
}

inline int FileWriteProxy::printf(const char *format, ...) {
  va_list args;
  va_start(args, format);

  char *buf = NULL;
  int ret = ::vasprintf(&buf, format, args);

  if (ret >= 0) {
    write(buf, ret + 1);  // including '\0'
  }
  free(buf);
  va_end(args);

  return ret;
}

inline int FileWriteProxy::byteInWriteQueue() {
  Lock lk(&write_mutex);
  return write_queue.size();
}

inline void FileWriteProxy::clearWriteQueue() {
  Lock lk(&write_mutex);
  write_queue.clear();
}

inline std::ostream &FileWriteProxy::ostream() { return ostrm; }

inline FileWriteProxy::OHolder::OHolder(FileWriteProxy *fwp) : fwp(fwp) {}

inline int FileWriteProxy::OHolder::write(const void *data, int size) {
  fwp->write(data, size);
  return size;
}

inline bool FileWriteProxy::OHolder::isOpen() const { return fwp->isOpen(); }

}  // namespace cutil
