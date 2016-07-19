/**
 * $Id: Thread.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_THREAD_H_
#define _CUTIL_THREAD_H_

extern "C" {
#include <pthread.h>
#include <stdint.h>
}

#ifdef linux
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 25)
// #define CUTIL_TIMERFD_ENABLED
#endif
#endif

namespace cutil {

/*!
  Mutex
*/
class Mutex {
  friend class Lock;
  friend class WaitCondition;

 private:
  pthread_mutex_t mutex;

 public:
  Mutex();
  ~Mutex();

  bool trylock();
  bool lock();
  bool unlock();
};

/*!
  Scoped lock
*/

class Lock {
  friend class WaitCondition;

 private:
  pthread_mutex_t *pmutex;

 public:
  explicit Lock(Mutex *mutex);
  explicit Lock(pthread_mutex_t *pmutex);
  ~Lock();
};

/*!
  Wait condition
*/
class WaitCondition {
 private:
  pthread_cond_t cond;

 public:
  WaitCondition();
  ~WaitCondition();

  bool wait(pthread_mutex_t *pm, int64_t timeout_usec = 0);

  bool wait(Mutex *m, int64_t timeout_usec = 0);

  bool wait(Lock *l, int64_t timeout_usec = 0);

  bool notify();
  bool notifyAll();
};

/*!
  Thread
*/
class Thread {
 private:
  pthread_t tid;
  int priority;
  volatile bool startFlag;
  Mutex mutex;
  WaitCondition condition;

  template <class Functor>
  struct Holder {
    Thread *instance;
    Functor *functor;
  };

  template <class Functor>
  static void *thread_function(void *holder);

  template <class Functor>
  bool start(Holder<Functor> holder);

  explicit Thread(pthread_t tid);

#ifdef CUTIL_TIMERFD_ENABLED
  int timer_fd;
#endif

#ifdef CUTIL_RT_ENABLED
  timespec prev_ts;
#endif

 public:
  Thread();
  virtual ~Thread();

  virtual void operator()() {}

  /*!
    Runs operator() as a thread.
  */
  bool start();

  /*!
    Runs t() as a thread.
    It copies t internally.
  */
  template <class Functor>
  bool start(Functor func);

  template <class Functor>
  bool start(Functor *func);

  bool join();
  bool detach();

  bool isRunning();

  bool setPriority(int priority);
  int getPriority();

  static Thread self();

#ifdef CUTIL_TIMERFD_ENABLED
  bool enterPeriodic();
  bool setPeriod(int64_t period_usec);
  int waitPeriod();
  bool exitPeriodic();
#endif

#ifdef CUTIL_RT_ENABLED
  bool enterRealtime();
  // return overrun usec. return 0 when succeeded. return negative values when
  // error occurs.
  int64_t waitNext(int64_t period_usec);
#endif
};

class LoopThreadBase : public Thread {
 private:
  volatile bool stopFlag;
  volatile bool suspendFlag;
  Mutex suspendMutex;
  WaitCondition suspendCondition;

  /*!
    Hide unsupported operations.
  */
  template <class Functor>
  bool start(Functor func);

  template <class Functor>
  bool start(Functor *func);

 public:
  LoopThreadBase();
  virtual ~LoopThreadBase();

  virtual void operator()();

 protected:
  virtual void sleepFunc() {}

  virtual void headFunc() {}
  virtual void eachLoop() {}
  virtual void tailFunc() {}

 public:
  bool start();
  bool stop();
  bool resume();
  bool suspend();
};

class LoopThread : public LoopThreadBase {
 private:
  volatile int64_t interval;

 public:
  LoopThread();
  virtual ~LoopThread();

 protected:
  virtual void sleepFunc();

 public:
  void setInterval(int64_t usec);
  int64_t getInterval() const;
};

#ifdef CUTIL_TIMERFD_ENABLED

class PeriodicThread : public LoopThreadBase {
 private:
  volatile int64_t period_usec;
  volatile int overrun;

 public:
  explicit PeriodicThread(int64_t period_usec);
  virtual ~PeriodicThread();

  virtual void operator()();

 protected:
  virtual void sleepFunc();

 protected:
  int getOverrun() const;
};

#endif

#ifdef CUTIL_RT_ENABLED

class RealtimeThread : public LoopThreadBase {
 private:
  volatile int64_t period_usec;
  volatile int overrun;

 public:
  explicit RealtimeThread(int64_t period_usec);
  virtual ~RealtimeThread();

  virtual void operator()();

 protected:
  virtual void sleepFunc();

 protected:
  int getOverrun() const;
};
#endif

}  // namespace cutil

#include "Thread.ipp"

#endif
