/**
 * $Id: Exception.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <execinfo.h>
#include <cxxabi.h>
}

#include "Exception.h"

using cutil;

//#########################################

#ifdef _SIGEXCEPTION_

void SignalToException::signalHandler(int signum, siginfo_t *info, void *ctx) {
  //! retrieve.. stack trace
  siglongjmp(mark, signum);
}

void SignalToException::throwException(int signum) throw(Exception) {
  throw Exception(sys_siglist[signum], __EXCEPTION_INFO__);
}

SignalToException::SignalToException() {
  sigaction_t sigact;
  memset(&sigact, 0, sizeof(sigact));
  sigemptyset(&sigact.sa_mask);

  sigaddset(&sigact.sa_mask, SIGSEGV);
  sigaddset(&sigact.sa_mask, SIGFPE);
  sigaddset(&sigact.sa_mask, SIGILL);
  sigaddset(&sigact.sa_mask, SIGBUS);
  sigact.sa_flags = SA_SIGINFO | SA_ONSTACK | SA_RESTART;
  sigact.sa_sigaction = SignalToException::signalHandler;

  sigaction(SIGSEGV, &sigact, &oldact[0]);
  sigaction(SIGFPE, &sigact, &oldact[1]);
  sigaction(SIGILL, &sigact, &oldact[2]);
  sigaction(SIGBUS, &sigact, &oldact[3]);
}

SignalToException::~SignalToException() {
  sigaction(SIGSEGV, &oldact[0], NULL);
  sigaction(SIGFPE, &oldact[1], NULL);
  sigaction(SIGILL, &oldact[2], NULL);
  sigaction(SIGBUS, &oldact[3], NULL);
}

sigjmp_buf SignalToException::mark;

#endif
