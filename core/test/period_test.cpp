/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>

#include "Thread.h"

using namespace std;
using namespace cutil;

#ifdef linux
#include <linux/version.h>
#if LINUX_VERSION_CODE >= KERNEL_VERSION(2, 6, 25)
// #define CUTIL_TIMERFD_ENABLED
#endif
#endif

#ifndef CUTIL_TIMERFD_ENABLED
int main() {
  return 0;
}
#else

class MyPeriodicThread : public PeriodicThread {
private:
  int i;
  timeval tv, tv_p;
protected:
  virtual void headFunc() {
    i = 0;
    cout << "headFunc()" << endl;
   gettimeofday( &tv_p, NULL );
  }
  virtual void eachLoop() {
    
    gettimeofday( &tv, NULL );
    double t = ( tv.tv_sec - tv_p.tv_sec ) * 1e6 + ( tv.tv_usec - tv_p.tv_usec );
    
    cout << t << endl;
    tv_p = tv;

    cout << "eachLoop() " << i << ", overrun " << getOverrun() << endl;
    i++;
    if ( i > 5 ) {
      cout << " thrown " << endl;
      throw Exception( "In MyLoopThread", __EXCEPTION_INFO__ );
    }
    usleep( 600000 );
  }
  virtual void tailFunc() {
    cout << "tailFunc()" << endl;
  }
public:
  MyPeriodicThread( long period_usec ) 
    : PeriodicThread( period_usec ) {}
};


int main() {
  try {

  cout << "make periodic " << endl;
  Thread::enterPeriodic( 1000 );
  
  
  int i = 0;
  timeval tv, tv_p;
  gettimeofday( &tv_p, NULL );

  while ( i < 7 ) {
    int overrun = Thread::waitPeriod();
    
    gettimeofday( &tv, NULL );
    double t = ( tv.tv_sec - tv_p.tv_sec ) * 1e6 + ( tv.tv_usec - tv_p.tv_usec );
    
    cout << t << endl;
    
    tv_p = tv;
    
    cout << "period overrun " << overrun << endl;
    ++i;
  }

  MyPeriodicThread pth( 500000 );
  pth.start();
  sleep( 4 );
  }
  catch ( const Exception &e ) {
    e.printStackTrace();
  }
  
  return 0;
}

#endif
