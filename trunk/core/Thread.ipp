/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <stdint.h>
#include <sys/time.h>
#include <fcntl.h>

#ifdef CUTIL_TIMERFD_ENABLED
#include <sys/timerfd.h>
#endif
}


namespace cutil {

inline
Mutex::Mutex() throw () {
  ::pthread_mutex_init( &pmutex, NULL );
}

inline
Mutex::~Mutex() throw () {
  ::pthread_mutex_destroy( &pmutex );
}

inline
bool Mutex::trylock() 
  throw ( ThreadException ) {
  int ret = ::pthread_mutex_trylock( &pmutex );
  if ( ret == 0 ) {
    return true;
  }
  else if ( ret == EBUSY ) {
    return false;
  }
  throw ThreadException( "Fail to pthread_mutex_trylock", __EXCEPTION_INFO__ );
}

inline
void Mutex::lock()
  throw ( ThreadException ) {
  if ( ::pthread_mutex_lock( &pmutex ) != 0 ) {
    throw ThreadException( "Fail to pthread_mutex_lock", __EXCEPTION_INFO__ );
  }
}

inline
void Mutex::unlock()
  throw ( ThreadException ) {
  if ( ::pthread_mutex_unlock( &pmutex ) != 0 ) {
    throw ThreadException( "Fail to pthread_mutex_unlock", __EXCEPTION_INFO__ );
  }
}

inline
Lock::Lock( Mutex &mutex ) throw ()
  : pmutex( mutex.pmutex ) {
  ::pthread_mutex_lock( &pmutex );
}

inline
Lock::Lock( pthread_mutex_t &pmutex ) throw ()
  : pmutex( pmutex ) {
  ::pthread_mutex_lock( &pmutex );
}

inline
Lock::~Lock() throw () {
  ::pthread_mutex_unlock( &pmutex );
}


inline
WaitCondition::WaitCondition() throw () {
  ::pthread_cond_init( &cond, NULL );
}

inline
WaitCondition::~WaitCondition() throw () {
  ::pthread_cond_destroy( &cond );
}

inline
bool WaitCondition::wait( pthread_mutex_t &pm, long timeout_usec ) 
  throw ( ThreadException ) {
  
  if ( timeout_usec <= 0 ) {
    return ::pthread_cond_wait( &cond, &pm ) == 0;
  }

  timeval tv;
  ::gettimeofday( &tv, NULL );
  timespec ts;
  long sec = timeout_usec / 1000000L;
  ts.tv_sec = sec + tv.tv_sec;
  ts.tv_nsec = ( timeout_usec - sec * 1000000L ) * 1000;
  int ret = pthread_cond_timedwait( &cond, &pm, &ts );
  
  if ( ret == 0 ) {
    return true;
  }
  else if ( ret == ETIMEDOUT ) {
    return false;
  }
  throw ThreadException( "Fail to pthread_cond_timedwait", __EXCEPTION_INFO__ );
}

inline
bool WaitCondition::wait( Mutex &m, long timeout_usec ) 
  throw ( ThreadException ) {
  return wait( m.pmutex, timeout_usec );
}

inline
bool WaitCondition::wait( Lock &l, long timeout_usec ) 
  throw ( ThreadException ) {
  return wait( l.pmutex, timeout_usec );
}

inline
bool WaitCondition::notify() throw () {
  return ::pthread_cond_signal( &cond ) == 0;
}

inline
bool WaitCondition::notifyAll() throw () {
  return ::pthread_cond_broadcast( &cond ) == 0;
}


template< class Functor >
inline
void* Thread::thread_function( void *holder ) throw () {
  
  Holder< Functor > data = *static_cast< Holder< Functor >* >( holder );
  {
    Lock lock( data.instance->mutex );
    data.instance->startFlag = true;
    data.instance->condition.notify();
  }
  
  try {
    (*data.functor)();
  }
  catch ( const Exception &e ) {
    data.instance->exception = e;
  }
  
  return holder;
}

template< class Functor >
inline
bool Thread::start( Holder< Functor > holder )
  throw ( ThreadException ) {
  
  if ( isRunning() ) {
    return false;
  }
  
  pthread_attr_t tattr;
  ::pthread_attr_init( &tattr );
  
  if ( priority != 0 ) {
    sched_param param; 
    param.sched_priority = priority; 
    ::pthread_attr_setschedparam( &tattr, &param );
  }
  
  Lock lock( mutex );
  startFlag = false;
  
  if ( ::pthread_create( &tid, &tattr, Thread::thread_function< Functor >, &holder ) != 0 ) {
    tid = 0;
    throw ThreadException( "Fail to pthread_create", __EXCEPTION_INFO__ );
  }
  while ( !startFlag ) {
    condition.wait( mutex );
  }
  
  return true;
}

inline
Thread::Thread( pthread_t tid ) throw ()
  : tid( tid ), priority( 0 ), startFlag( false ) {}

inline
Thread::Thread() throw () 
  : tid( 0 ), priority( 0 ), startFlag( false ) {}

inline
Thread::~Thread() {
  if ( isRunning() ) {
    return;
  }
  
  try {
    join();
  }
  catch ( const Exception &e ) {}
}

inline
std::map< pthread_t, int >& Thread::timerfdMap() throw () {
  static std::map< pthread_t, int > timerfdmap;
  return timerfdmap;
}

inline
bool Thread::start() 
  throw ( ThreadException ) {

  Holder< Thread > holder;
  holder.instance = this;
  holder.functor = this;

  return start( holder );
}


template< class Functor >
inline
bool Thread::start( Functor func )
  throw ( ThreadException ) {

  Holder< Functor > holder;
  holder.instance = this;
  holder.functor = &func;

  return start( holder );
}

template< class Functor >
inline
bool Thread::start( Functor *func )
  throw ( ThreadException ) {
  
  Holder< Functor > holder;
  holder.instance = this;
  holder.functor = func;
  
  return start( holder );
}


inline
bool Thread::join() 
  throw ( ThreadException ) {
  if ( !isRunning() ) {
    return false;
  }
  
  int ret = ::pthread_join( tid, NULL );
  if ( ret == EDEADLK ) {
    throw ThreadException( "Fail to pthread_join (deadlock is suspected)", __EXCEPTION_INFO__ );
  }
  else if ( ret != 0 ) {
    return false;
  }
  return true;
}

inline
bool Thread::detach() 
  throw ( ThreadException ) {
  if ( !isRunning() ) {
    return false;
  }
  
  if ( ::pthread_detach( tid ) != 0 ) {
    throw ThreadException( "Fail to pthread_detach", __EXCEPTION_INFO__ );
  }
  return true;
}

inline
bool Thread::isRunning() throw () {
  if ( tid == 0 ) {
    return false;
  }
  
  int ret = ::pthread_kill( tid, 0 );
  if ( ret == ESRCH ) {
    tid = 0;
    return false;
  }
  return true;
}

inline
const Exception& Thread::caughtException() throw () {
  return exception;
}

inline
bool Thread::setPriority( int priority ) throw () {
  if ( isRunning() ) {
    int policy;
    sched_param param;

    if ( ::pthread_getschedparam( tid, &policy, &param ) != 0 ) {
      return false;
    }
    
    param.sched_priority = priority;
    
    if ( ::pthread_setschedparam( tid, policy, &param ) != 0 ) {
      return false;
    }
  }

  this->priority = priority;
  return true;
}

inline
int Thread::getPriority() 
  throw ( ThreadException ) {
  
  if ( isRunning() ) {
    int policy;
    sched_param param;
    
    if ( ::pthread_getschedparam( tid, &policy, &param ) != 0 ) {
      return false;
    }

    this->priority = param.sched_priority;
  }
  
  return this->priority;
}

inline
Thread Thread::self() throw () {
  return Thread( ::pthread_self() );
}

#ifdef CUTIL_TIMERFD_ENABLED


inline
bool Thread::enterPeriodic( long period_usec ) 
  throw ( ThreadException ) {
  
  std::map< pthread_t, int >::iterator
    it = timerfdMap().find( ::pthread_self() );
  
  if ( it != timerfdMap().end() ) {
    return false;
  }


  int timer_fd = ::timerfd_create( CLOCK_REALTIME, 0 );
  
  if ( timer_fd == -1 ) {
    throw ThreadException( "Fail to timerfd_create", __EXCEPTION_INFO__ );
  }
  
  timerfdMap().insert( std::pair< pthread_t, int >( ::pthread_self(), timer_fd ) );
  
  return setPeriod( period_usec );
}

inline
bool Thread::setPeriod( long period_usec )
  throw ( ThreadException ) {
  
  std::map< pthread_t, int >::iterator
    it = timerfdMap().find( ::pthread_self() );
  
  if ( it == timerfdMap().end() ) {
    return false;
  }
  

  itimerspec itval;
  
  itval.it_interval.tv_sec 
    = itval.it_value.tv_sec
    = period_usec / 1000000;

  itval.it_interval.tv_nsec
    = itval.it_value.tv_nsec
    = ( period_usec - itval.it_interval.tv_sec * 1000000 ) * 1000;
  
  if ( ::timerfd_settime( it->second, 0, &itval, NULL ) == -1 ) {
    throw ThreadException( "Fail to ::timer_settime", __EXCEPTION_INFO__ );
  }

  return true;
}

inline
int Thread::waitPeriod()
  throw ( ThreadException ) {

  std::map< pthread_t, int >::iterator
    it = timerfdMap().find( ::pthread_self() );
  
  if ( it == timerfdMap().end() ) {
    return -1;
  }
  
  uint64_t
    missed = 0,
    tmp = 0;
  

  //! save flag
  int flag = ::fcntl( it->second, F_GETFL, NULL );
  if ( flag < 0 ) {
    throw ThreadException( "::fcntl( GET ) error", __EXCEPTION_INFO__ );
  }

  //! make non-blocking
  if ( ::fcntl( it->second, F_SETFL, flag | O_NONBLOCK ) < 0 ) {
    throw ThreadException( "::fcntl( NONBLOCK ) error", __EXCEPTION_INFO__ );
  }
  
  //! non-blocking read and accumulate overrun
  int ret = ::read( it->second, &tmp, sizeof( tmp ) );

  if ( ret == -1 && errno != EAGAIN ) {
    throw ThreadException( "Fail to ::read", __EXCEPTION_INFO__ );
  }
  else if ( ret > 0 ) {
    missed += tmp;
  }
  
  //! make blocking
  if ( ::fcntl( it->second, F_SETFL, flag ) < 0 ) {
    throw ThreadException( "::fcntl( BLOCK ) error", __EXCEPTION_INFO__ );
  }
  
  //! blocking read and wait next period
  ret = ::read( it->second, &tmp, sizeof( tmp ) );
  
  if ( ret == -1 ) {
    throw ThreadException( "Fail to ::read", __EXCEPTION_INFO__ );
  }
  
  return ( missed + tmp ) - 1;
}

inline
bool Thread::exitPeriodic()
  throw ( ThreadException ) {

  std::map< pthread_t, int >::iterator
    it = timerfdMap().find( ::pthread_self() );
  
  if ( it == timerfdMap().end() ) {
    return false;
  }

  ::close( it->second );
  
  timerfdMap().erase( it );
  return true;
}

#endif

inline
LoopThreadBase::LoopThreadBase() throw ()
  : stopFlag( false ), suspendFlag( false ){}

inline
LoopThreadBase::~LoopThreadBase() {
  stop();
}

inline
void LoopThreadBase::operator()() {
  try {
    headFunc();
  }
  catch ( const Exception &e ) {
    exception = e;
    return;
  }
  
  while ( true ) {
    {
      Lock lock( suspendMutex );
        
      if ( stopFlag ) {
        break;
      }
      
      while ( suspendFlag && !stopFlag ) {
        suspendCondition.wait( suspendMutex );
      }
        
      if ( stopFlag ) {
        break;
      }
    }
    
    try {
      sleepFunc();
      eachLoop();
    }
    catch ( const Exception &e ) {
      exception = e;
      break;
    }
  }
  
  try {
    tailFunc();
  }
  catch ( const Exception &e ) {
    exception = e;
    return;
  }
}

inline
bool LoopThreadBase::start() 
  throw ( ThreadException ) {
  return Thread::start();
}

inline
bool LoopThreadBase::stop() throw () {
  if ( !isRunning() ) {
    return false;
  }
  Lock lock( suspendMutex );
  stopFlag = true;
  suspendCondition.notify();
  return true;
}

inline
bool LoopThreadBase::resume() throw () {
  if ( !isRunning() ) {
    return false;
  }
  Lock lock( suspendMutex );
  suspendFlag = false;
  suspendCondition.notify();
  return true;
}

inline
bool LoopThreadBase::suspend() throw () {
  if ( !isRunning() ) {
    return false;
  }
  Lock lock( suspendMutex );
  suspendFlag = true;
  suspendCondition.notify();
  return true;
}

inline
LoopThread::LoopThread() throw ()
  : interval( 0 ) {}

inline
LoopThread::~LoopThread() {}

inline
void LoopThread::sleepFunc() {
  ::usleep( interval );
}

inline
void LoopThread::setInterval( long usec ) throw () {
  interval = usec;
}

inline
long LoopThread::getInterval() const throw () {
  return interval;
}


#ifdef CUTIL_TIMERFD_ENABLED

inline
PeriodicThread::PeriodicThread( long period_usec ) throw ()
  : period_usec( period_usec ), overrun( 0 ) {}
  
inline
PeriodicThread::~PeriodicThread() {}

inline 
void PeriodicThread::operator()() {
  Thread::enterPeriodic( period_usec );
  LoopThreadBase::operator()();
  Thread::exitPeriodic();
}

inline 
void PeriodicThread::sleepFunc() {
  overrun = Thread::waitPeriod();
}

inline
int PeriodicThread::getOverrun() const throw () {
  return overrun;
}

#endif
   
}
  
