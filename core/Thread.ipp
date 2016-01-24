/**
 * $Id: Thread.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <stdint.h>
#include <sys/time.h>
#include <fcntl.h>
#include <errno.h>

#ifdef CUTIL_TIMERFD_ENABLED
#include <sys/timerfd.h>
#endif
}

namespace cutil {

inline Mutex::Mutex() { ::pthread_mutex_init(&mutex, NULL); }

inline Mutex::~Mutex() { ::pthread_mutex_destroy(&mutex); }

inline bool Mutex::trylock() {
  int ret = ::pthread_mutex_trylock(&mutex);
  if (ret == 0) {
    return true;
  } else if (ret == EBUSY) {
    return false;
  } else {
    return false;
  }
}

inline bool Mutex::lock() { return (::pthread_mutex_lock(&mutex) == 0); }

inline bool Mutex::unlock() { return (::pthread_mutex_unlock(&mutex) == 0); }

inline Lock::Lock(Mutex *mutex) : pmutex(&mutex->mutex) {
  ::pthread_mutex_lock(pmutex);
}

inline Lock::Lock(pthread_mutex_t *pmutex) : pmutex(pmutex) {
  ::pthread_mutex_lock(pmutex);
}

inline Lock::~Lock() { ::pthread_mutex_unlock(pmutex); }

inline WaitCondition::WaitCondition() { ::pthread_cond_init(&cond, NULL); }

inline WaitCondition::~WaitCondition() { ::pthread_cond_destroy(&cond); }

inline bool WaitCondition::wait(pthread_mutex_t *pm, int64_t timeout_usec) {
  if (timeout_usec <= 0) {
    return (::pthread_cond_wait(&cond, pm) == 0);
  }

  timeval tv;
  ::gettimeofday(&tv, NULL);
  timespec ts;
  int64_t sec = timeout_usec / 1000000L;
  ts.tv_sec = sec + tv.tv_sec;
  ts.tv_nsec = (timeout_usec - sec * 1000000L) * 1000;
  int ret = pthread_cond_timedwait(&cond, pm, &ts);

  if (ret == 0) {
    return true;
  } else if (ret == ETIMEDOUT) {
    return false;
  } else {
    return false;
  }
}

inline bool WaitCondition::wait(Mutex *m, int64_t timeout_usec) {
  return wait(&m->mutex, timeout_usec);
}

inline bool WaitCondition::wait(Lock *l, int64_t timeout_usec) {
  return wait(l->pmutex, timeout_usec);
}

inline bool WaitCondition::notify() {
  return ::pthread_cond_signal(&cond) == 0;
}

inline bool WaitCondition::notifyAll() {
  return ::pthread_cond_broadcast(&cond) == 0;
}

template <class Functor>
inline void *Thread::thread_function(void *holder) {
  Holder<Functor> data = *static_cast<Holder<Functor> *>(holder);
  {
    Lock lock(&data.instance->mutex);
    data.instance->startFlag = true;
    data.instance->condition.notify();
  }

  (*data.functor)();
  return holder;
}

template <class Functor>
inline bool Thread::start(Holder<Functor> holder) {
  if (isRunning()) {
    return false;
  }

  pthread_attr_t tattr;
  ::pthread_attr_init(&tattr);

  if (priority != 0) {
    sched_param param;
    param.sched_priority = priority;
    ::pthread_attr_setschedparam(&tattr, &param);
  }

  Lock lock(&mutex);
  startFlag = false;

  if (::pthread_create(&tid, &tattr, Thread::thread_function<Functor>,
                       &holder) != 0) {
    tid = 0;
    return false;
  }
  while (!startFlag) {
    condition.wait(&mutex);
  }

  return true;
}

inline Thread::Thread(pthread_t tid)
    : tid(tid), priority(0), startFlag(false) {}

inline Thread::Thread() : tid(0), priority(0), startFlag(false) {}

inline Thread::~Thread() {
  if (isRunning()) {
    return;
  }

  join();
}

inline std::map<pthread_t, int> &Thread::timerfdMap() {
  static std::map<pthread_t, int> timerfdmap;
  return timerfdmap;
}

inline bool Thread::start() {
  Holder<Thread> holder;
  holder.instance = this;
  holder.functor = this;

  return start(holder);
}

template <class Functor>
inline bool Thread::start(Functor func) {
  Holder<Functor> holder;
  holder.instance = this;
  holder.functor = &func;

  return start(holder);
}

template <class Functor>
inline bool Thread::start(Functor *func) {
  Holder<Functor> holder;
  holder.instance = this;
  holder.functor = func;

  return start(holder);
}

inline bool Thread::join() {
  if (!isRunning()) {
    return false;
  }

  int ret = ::pthread_join(tid, NULL);
  if (ret == EDEADLK) {
    return false;
  } else if (ret != 0) {
    return false;
  }
  return true;
}

inline bool Thread::detach() {
  if (!isRunning()) {
    return false;
  }

  if (::pthread_detach(tid) != 0) {
    return false;
  }
  return true;
}

inline bool Thread::isRunning() {
  if (tid == 0) {
    return false;
  }

  int ret = ::pthread_kill(tid, 0);
  if (ret == ESRCH) {
    tid = 0;
    return false;
  }
  return true;
}

inline bool Thread::setPriority(int priority) {
  if (isRunning()) {
    int policy;
    sched_param param;

    if (::pthread_getschedparam(tid, &policy, &param) != 0) {
      return false;
    }

    param.sched_priority = priority;

    if (::pthread_setschedparam(tid, policy, &param) != 0) {
      return false;
    }
  }

  this->priority = priority;
  return true;
}

inline int Thread::getPriority() {
  if (isRunning()) {
    int policy;
    sched_param param;

    if (::pthread_getschedparam(tid, &policy, &param) != 0) {
      return -1;
    }

    this->priority = param.sched_priority;
  }

  return this->priority;
}

inline Thread Thread::self() { return Thread(::pthread_self()); }

#ifdef CUTIL_TIMERFD_ENABLED

inline bool Thread::enterPeriodic(int64_t period_usec) {
  std::map<pthread_t, int>::iterator it = timerfdMap().find(::pthread_self());

  if (it != timerfdMap().end()) {
    return false;
  }

  int timer_fd = ::timerfd_create(CLOCK_REALTIME, 0);

  if (timer_fd == -1) {
    return false;
  }

  timerfdMap().insert(std::pair<pthread_t, int>(::pthread_self(), timer_fd));

  return setPeriod(period_usec);
}

inline bool Thread::setPeriod(int64_t period_usec) {
  std::map<pthread_t, int>::iterator it = timerfdMap().find(::pthread_self());

  if (it == timerfdMap().end()) {
    return false;
  }

  itimerspec itval;

  itval.it_interval.tv_sec = itval.it_value.tv_sec = period_usec / 1000000;

  itval.it_interval.tv_nsec = itval.it_value.tv_nsec =
      (period_usec - itval.it_interval.tv_sec * 1000000) * 1000;

  if (::timerfd_settime(it->second, 0, &itval, NULL) == -1) {
    return false;
  }

  return true;
}

inline int Thread::waitPeriod() {
  std::map<pthread_t, int>::iterator it = timerfdMap().find(::pthread_self());

  if (it == timerfdMap().end()) {
    return -1;
  }

  uint64_t missed = 0, tmp = 0;

  //! save flag
  int flag = ::fcntl(it->second, F_GETFL, NULL);
  if (flag < 0) {
    return -1;
  }

  //! make non-blocking
  if (::fcntl(it->second, F_SETFL, flag | O_NONBLOCK) < 0) {
    return -1
  }

  //! non-blocking read and accumulate overrun
  int ret = ::read(it->second, &tmp, sizeof(tmp));

  if (ret == -1 && errno != EAGAIN) {
    return -1
  } else if (ret > 0) {
    missed += tmp;
  }

  //! make blocking
  if (::fcntl(it->second, F_SETFL, flag) < 0) {
    return -1;
  }

  //! blocking read and wait next period
  ret = ::read(it->second, &tmp, sizeof(tmp));

  if (ret == -1) {
    return -1;
  }

  return (missed + tmp) - 1;
}

inline bool Thread::exitPeriodic() {
  std::map<pthread_t, int>::iterator it = timerfdMap().find(::pthread_self());

  if (it == timerfdMap().end()) {
    return false;
  }

  ::close(it->second);

  timerfdMap().erase(it);
  return true;
}

#endif

inline LoopThreadBase::LoopThreadBase() : stopFlag(false), suspendFlag(false) {}

inline LoopThreadBase::~LoopThreadBase() { stop(); }

inline void LoopThreadBase::operator()() {
  headFunc();

  while (true) {
    {
      Lock lock(&suspendMutex);

      if (stopFlag) {
        break;
      }

      while (suspendFlag && !stopFlag) {
        suspendCondition.wait(&suspendMutex);
      }

      if (stopFlag) {
        break;
      }
    }

    sleepFunc();
    eachLoop();
  }

  tailFunc();
}

inline bool LoopThreadBase::start() { return Thread::start(); }

inline bool LoopThreadBase::stop() {
  if (!isRunning()) {
    return false;
  }
  Lock lock(&suspendMutex);
  stopFlag = true;
  suspendCondition.notify();
  return true;
}

inline bool LoopThreadBase::resume() {
  if (!isRunning()) {
    return false;
  }
  Lock lock(&suspendMutex);
  suspendFlag = false;
  suspendCondition.notify();
  return true;
}

inline bool LoopThreadBase::suspend() {
  if (!isRunning()) {
    return false;
  }
  Lock lock(&suspendMutex);
  suspendFlag = true;
  suspendCondition.notify();
  return true;
}

inline LoopThread::LoopThread() : interval(0) {}

inline LoopThread::~LoopThread() {}

inline void LoopThread::sleepFunc() { ::usleep(interval); }

inline void LoopThread::setInterval(int64_t usec) { interval = usec; }

inline int64_t LoopThread::getInterval() const { return interval; }

#ifdef CUTIL_TIMERFD_ENABLED

inline PeriodicThread::PeriodicThread(int64_t period_usec)
    : period_usec(period_usec), overrun(0) {}

inline PeriodicThread::~PeriodicThread() {}

inline void PeriodicThread::operator()() {
  Thread::enterPeriodic(period_usec);
  LoopThreadBase::operator()();
  Thread::exitPeriodic();
}

inline void PeriodicThread::sleepFunc() { overrun = Thread::waitPeriod(); }

inline int PeriodicThread::getOverrun() const { return overrun; }

#endif
}
