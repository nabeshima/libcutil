/**
 * $Id: period_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>

#ifdef linux
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 25)
// #define CUTIL_TIMERFD_ENABLED
#endif
#endif

#include "Thread.h"

using namespace std;
using namespace cutil;

#ifndef CUTIL_TIMERFD_ENABLED
int main() { return 0; }
#else

class MyPeriodicThread : public PeriodicThread {
 private:
  int i;
  timeval tv, tv_p;

 protected:
  virtual void headFunc() {
    i = 0;
    cout << "headFunc()" << endl;
    gettimeofday(&tv_p, NULL);
  }
  virtual void eachLoop() {
    gettimeofday(&tv, NULL);
    double t = (tv.tv_sec - tv_p.tv_sec) * 1e6 + (tv.tv_usec - tv_p.tv_usec);

    cout << t << ", count: " << i << ", overrun: " << getOverrun() << endl;
    tv_p = tv;
    i++;
    if (i > 1000) {
      cout << " stop " << endl;
      stop();
    }
  }
  virtual void tailFunc() { cout << "tailFunc()" << endl; }

 public:
  explicit MyPeriodicThread(int64_t period_usec)
      : PeriodicThread(period_usec), i(0) {}
  virtual ~MyPeriodicThread() {}
};

int main() {
  cout << "make periodic " << endl;
  Thread th(Thread::self());
  th.enterPeriodic();
  th.setPeriod(1000);

  int i = 0;
  timeval tv, tv_p;
  gettimeofday(&tv_p, NULL);

  while (i < 1000) {
    int overrun = th.waitPeriod();

    gettimeofday(&tv, NULL);
    double t = (tv.tv_sec - tv_p.tv_sec) * 1e6 + (tv.tv_usec - tv_p.tv_usec);

    cout << t << ", count: " << i << ", overrun: " << overrun << endl;

    tv_p = tv;
    ++i;
  }
  th.exitPeriodic();

  MyPeriodicThread pth(1000);
  pth.start();
  sleep(4);
  pth.join();

  return 0;
}

#endif
