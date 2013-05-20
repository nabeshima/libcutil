/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_THREAD_H_
#define _CUTIL_THREAD_H_

extern "C" {
#include <pthread.h>
}

#ifdef linux
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 25)
// #define CUTIL_TIMERFD_ENABLED
#endif
#endif

#include <map>

#include "ThreadException.h"

namespace cutil {

/*!
  Mutex
*/
class Mutex {
  friend class Lock;
  friend class WaitCondition;
  
private:
  pthread_mutex_t pmutex;
  
public:
  Mutex() throw ();
  ~Mutex() throw ();
  
  bool trylock() 
    throw ( ThreadException );
  void lock()
    throw ( ThreadException );
  void unlock()
    throw ( ThreadException );
};


/*!
  Scoped lock
*/

class Lock {
  friend class WaitCondition;
  
private:
  pthread_mutex_t &pmutex;
  
public:
  Lock( Mutex &mutex ) throw ();
  Lock( pthread_mutex_t &pmutex ) throw ();
  ~Lock() throw ();
};


/*!
  Wait condition
*/
class WaitCondition {
private:
  pthread_cond_t cond;
  
public:
  WaitCondition() throw ();
  ~WaitCondition() throw ();
  
  bool wait( pthread_mutex_t &pm, long timeout_usec = 0 ) 
    throw ( ThreadException );
  
  bool wait( Mutex &m, long timeout_usec = 0 ) 
    throw ( ThreadException );

  bool wait( Lock &l, long timeout_usec = 0 ) 
    throw ( ThreadException );

  bool notify() throw ();
  bool notifyAll() throw ();
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
  
  template< class Functor >
  struct Holder {
    Thread *instance;
    Functor *functor;
  };
  
  template< class Functor >
  static void* thread_function( void *holder ) throw ();
  
  template< class Functor >
  bool start( Holder< Functor > holder )
    throw ( ThreadException );
  
  Thread( pthread_t tid ) throw ();


  static std::map< pthread_t, int >& timerfdMap() throw ();
  
protected:
  Exception exception;
  
public:
  Thread() throw ();
  virtual ~Thread();
  
  virtual void operator()() {}
  
  /*!
    Runs operator() as a thread.
  */
  bool start() 
    throw ( ThreadException );
  
  /*!
    Runs t() as a thread.
    It copies t internally.
  */
  template< class Functor >
  bool start( Functor func )
    throw ( ThreadException );

  template< class Functor >
  bool start( Functor *func )
    throw ( ThreadException );
  
  bool join() 
    throw ( ThreadException );
  bool detach()
    throw ( ThreadException );
  
  bool isRunning() throw ();
  const Exception& caughtException() throw ();
  
  bool setPriority( int priority ) throw ();
  int getPriority() 
    throw ( ThreadException );
  

  static Thread self() throw ();
  
#ifdef CUTIL_TIMERFD_ENABLED
  static bool enterPeriodic( long period_usec ) 
    throw ( ThreadException );
  static bool setPeriod( long period_usec ) 
    throw ( ThreadException );
  static int waitPeriod()
    throw ( ThreadException );
  static bool exitPeriodic()     
    throw ( ThreadException );
#endif
};


class LoopThreadBase: public Thread {
private:
  volatile bool stopFlag;
  volatile bool suspendFlag;
  Mutex suspendMutex;
  WaitCondition suspendCondition;
  
  /*!
    Hide unsupported operations.
  */
  template< class Functor >
  bool start( Functor func )
    throw ( ThreadException );

  template< class Functor >
  bool start( Functor *func )
    throw ( ThreadException );
  
public:
  LoopThreadBase() throw ();
  virtual ~LoopThreadBase();
  
  virtual void operator()();
  
protected:
  virtual void sleepFunc() {}

  virtual void headFunc() {}
  virtual void eachLoop() {}
  virtual void tailFunc() {}
  
public:
  bool start() 
    throw ( ThreadException );
  bool stop() throw ();
  bool resume() throw ();
  bool suspend() throw ();
};


class LoopThread: public LoopThreadBase {
private:
  volatile long interval;
  
public:
  LoopThread() throw ();
  virtual ~LoopThread();

protected:  
  virtual void sleepFunc();
  
public:  
  void setInterval( long usec ) throw ();
  long getInterval() const throw ();
};


#ifdef CUTIL_TIMERFD_ENABLED

class PeriodicThread: public LoopThreadBase {
private:
  volatile long period_usec;
  volatile int overrun;
  
public:
  PeriodicThread( long period_usec ) throw ();
  virtual ~PeriodicThread();
  
  virtual void operator()();
  
protected:  
  virtual void sleepFunc();

protected:
  int getOverrun() const throw ();
};

#endif

}

#include "Thread.ipp"

#undef CUTIL_TIMERFD_ENABLED

#endif
