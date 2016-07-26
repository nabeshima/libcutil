/**
 * Copyright (c) 2016 CYBERDYNE Inc.
 */

#include <iostream>

#include "Thread.h"

using namespace std;
using namespace cutil;

#ifndef CUTIL_RT_ENABLED
int main() { return 0; }
#else

class MyRealtimeThread : public RealtimeThread {
 private:
  uint64_t count;
  timeval tv, tv_p;

 protected:
  virtual void headFunc() { gettimeofday(&tv_p, NULL); }

  virtual void eachLoop() {
    if (count >= 1000L) {
      stop();
      return;
    }
    // 5000 * 1ms = 5s
    int64_t overrun = getOverrun();  // 1000us = 1ms
    gettimeofday(&tv, NULL);
    double t = (tv.tv_sec - tv_p.tv_sec) * 1e6 + (tv.tv_usec - tv_p.tv_usec);

    cout << t << ", count: " << count << ", overrun: " << overrun << endl;
    tv_p = tv;
    ++count;
  }
  virtual void tailLoop() { cout << "tailFunc()" << endl; }

 public:
  MyRealtimeThread() : RealtimeThread(1000), count(0) {}  // 1000ms period
  virtual ~MyRealtimeThread() {}
};

int main() {
  cout << "make realtime" << endl;
  Thread th(Thread::self());
  th.enterRealtime();

  uint64_t count = 0;
  timeval tv, tv_p;
  gettimeofday(&tv_p, NULL);
  while (count < 1000L) {                 // 5000 * 1ms = 5s
    int64_t overrun = th.waitNext(1000);  // 1000us = 1ms
    gettimeofday(&tv, NULL);
    double t = (tv.tv_sec - tv_p.tv_sec) * 1e6 + (tv.tv_usec - tv_p.tv_usec);

    cout << t << ", count: " << count << ", overrun: " << overrun << endl;
    tv_p = tv;
    ++count;
  }
  cout << "end realtime" << endl;

  cout << "start realtime thread" << endl;
  MyRealtimeThread rth;
  rth.setPriority(59);
  rth.start();
  sleep(6);
  rth.join();
  cout << "join realtime thread" << endl;
}

#endif
