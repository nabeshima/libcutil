/**
 * $Id: Process.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <sys/wait.h>
}
#include <cstdlib>
#include <cerrno>
#include <csignal>

namespace cutil {

namespace {

const int READ = 0;
const int WRITE = 1;

inline void closePipe(int *pp) {
  ::close(pp[READ]);
  ::close(pp[WRITE]);
}
}

template <Process::IOHolderType type>
inline Process::IOHolder<type>::IOHolder(Process *proc)
    : proc(proc) {}

template <Process::IOHolderType type>
inline int Process::IOHolder<type>::write(const void *data, int size) {
  switch (type) {
    case STDIN:
      return proc->writeIn(data, size);
    case STDOUT:
    case STDERR:
      return 0;
  }
}

template <Process::IOHolderType type>
inline int Process::IOHolder<type>::read(void *data, int size) {
  switch (type) {
    case STDIN:
      return 0;
    case STDOUT:
      return proc->readOut(data, size);
    case STDERR:
      return proc->readErr(data, size);
  }
}

template <Process::IOHolderType type>
inline bool Process::IOHolder<type>::isOpen() {
  return proc->isLiving();
}

inline Process::Process()
    : pid(-1),
      status(0),
      pin(-1),
      pout(-1),
      perr(-1),
      pin_holder(this),
      pout_holder(this),
      perr_holder(this),
      pin_stream(&pin_holder),
      pout_stream(&pout_holder),
      perr_stream(&perr_holder) {}

inline Process::~Process() {
  try {
    this->close();
  } catch (...) {
  }
}

inline bool Process::open(const std::string &command) {
  if (isLiving()) {
    return false;
  }

  int pipeIn[2], pipeOut[2], pipeErr[2];

  if (::pipe(pipeIn) < 0) {
    return false;
  }
  if (::pipe(pipeOut) < 0) {
    closePipe(pipeIn);
    return false;
  }
  if (::pipe(pipeErr) < 0) {
    closePipe(pipeIn);
    closePipe(pipeOut);
    return false;
  }

  if ((pid = ::fork()) < 0) {
    closePipe(pipeIn);
    closePipe(pipeOut);
    closePipe(pipeErr);
    return false;
  } else if (pid == 0) {  //! child Process.
    ::close(pipeIn[WRITE]);
    ::close(pipeOut[READ]);
    ::close(pipeErr[READ]);

    ::dup2(pipeIn[READ], 0);
    ::dup2(pipeOut[WRITE], 1);
    ::dup2(pipeErr[WRITE], 2);

    ::close(pipeIn[READ]);
    ::close(pipeOut[WRITE]);
    ::close(pipeErr[WRITE]);

    if (::execlp("sh", "sh", "-c", command.c_str(), NULL) < 0) {
      ::exit(errno);
    } else {
      ::exit(0);
    }
    return false;  // dummy
  } else {
    ::close(pipeIn[READ]);
    ::close(pipeOut[WRITE]);
    ::close(pipeErr[WRITE]);

    pin = pipeIn[WRITE];
    pout = pipeOut[READ];
    perr = pipeErr[READ];

    return true;
  }
}

inline void Process::pin_close() {
  if (pin != -1) {
    ::close(pin);
    pin = -1;
  }
}

inline void Process::pout_close() {
  if (pout != -1) {
    ::close(pout);
    pout = -1;
  }
}

inline void Process::perr_close() {
  if (perr != -1) {
    ::close(perr);
    perr = -1;
  }
}

inline void Process::close() {
  if (!isLiving()) {
    pid = -1;
  }

  if (isRunning()) {
    this->kill(SIGKILL, 0);

    //! blocking
    while (isRunning()) {
      ::usleep(10000);
    }
  }

  //! Process has already killed here.
  perr_close();
  pout_close();
  pin_close();

  pid = -1;
}

inline bool Process::kill(int signum, int timeout_usec) {
  if (!isLiving()) {
    return true;
  }

  ::kill(pid, signum);

  int time = 0;

  while (isRunning()) {
    time += 10000;
    if (timeout_usec < time) {
      return false;
    }
    ::usleep(10000);
  }
  return true;
}

inline bool Process::isLiving() {
  if (pid == -1) {
    return false;
  }

  if (::kill(pid, 0) < 0 && errno == ESRCH) {
    return false;
  }

  return true;
}

inline bool Process::isRunning() {
  if (!isLiving()) {
    return false;
  }

  int val = ::waitpid(pid, &status, WNOHANG);

  if (val == 0) {
    return true;
  }

  //! zombie
  return false;
}

inline int Process::returnCode() { return WEXITSTATUS(status); }

inline int Process::writeIn(const void *data, int size) {
  if (!isLiving()) {
    return -1;
  }
  int ret = ::write(pin, data, size);
  if (ret < 0) {
    this->pin_close();
  }
  return ret;
}

inline int Process::readOut(void *data, int size) {
  if (!isLiving()) {
    return -1;
  }

  int ret = ::read(pout, data, size);
  if (ret <= 0) {
    this->pout_close();
  }
  return ret;
}

inline int Process::readErr(void *data, int size) {
  if (!isLiving()) {
    return -1;
  }
  int ret = ::read(perr, data, size);
  if (ret <= 0) {
    this->perr_close();
  }
  return ret;
}

inline std::ostream &Process::stdin() { return pin_stream; }

inline std::istream &Process::stdout() { return pout_stream; }

inline std::istream &Process::stderr() { return perr_stream; }
}
