/**
 * $Id: Process.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_ProcESS_H_
#define _CUTIL_ProcESS_H_

#include <cstdio>

#include <string>

#include "IOStream.h"

namespace cutil {

/*!
  Process class.
  A wrapper of fork, exec and pipe.
*/

class Process {
 protected:
  pid_t pid;
  int status;
  int pin, pout, perr;

 private:
  //! Holder for IOStream
  enum IOHolderType { STDIN, STDOUT, STDERR };

  template <IOHolderType type>
  class IOHolder {
   private:
    Process *proc;

   public:
    explicit IOHolder(Process *proc);
    int write(const void *data, int size);
    int read(void *data, int size);
    bool isOpen();
  };

  IOHolder<STDIN> pin_holder;
  IOHolder<STDOUT> pout_holder;
  IOHolder<STDERR> perr_holder;

  OStream<IOHolder<STDIN> > pin_stream;
  IStream<IOHolder<STDOUT> > pout_stream;
  IStream<IOHolder<STDERR> > perr_stream;

 protected:
  void pin_close();
  void pout_close();
  void perr_close();

 public:
  Process();
  virtual ~Process();

  bool open(const std::string &command);
  virtual void close();

  virtual bool kill(int signum, int timeout_usec);
  bool isLiving();
  virtual bool isRunning();

  int returnCode();

  int writeIn(const void *data, int size);  //! return wrote byte
  int readOut(void *data, int size);        //! return read byte
  int readErr(void *data, int size);        //! return read byte

  std::ostream &stdin();
  std::istream &stdout();
  std::istream &stderr();
};
}  // namespace cutil

#include "Process.ipp"

#endif
