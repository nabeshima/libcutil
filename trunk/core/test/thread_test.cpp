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

class MyThread : public Thread {
public:
  int i;
  
  MyThread()
    : i( 0 ) {}
  
  virtual void operator()() {
    cout << "MyThread. ()" << endl;
    while ( i < 3 ) {
      cout << "kib " << i++ << endl;
      sleep( 1 );
    }
  }
};


void thread_func() {
  int i = 0;
  while ( i < 3 ) {
    cout << "global " << ( i += 2 ) << endl;
    sleep( 1 );
  }
}

void thread_func2() {
  int i = 0;
  while ( i < 3 ) {
    cout << "global2 " << ( i += 2 ) << endl;
    throw Exception( "test", __EXCEPTION_INFO__ );
    sleep( 1 );
  }
}


class MyLoopThread : public LoopThread {
private:
  int i;
protected:
  virtual void headFunc() {
    i = 0;
    cout << "headFunc()" << endl;
  }
  virtual void eachLoop() {
    cout << "eachLoop() " << i << endl;
    i++;
    if ( i > 5 ) {
      //      stop();
      cout << " thrown " << endl;
      throw Exception( "In MyLoopThread", __EXCEPTION_INFO__ );
    }
  }
  virtual void tailFunc() {
    cout << "tailFunc()" << endl;
  }
};

int main() {
  if ( true ) {
    MyLoopThread thread;
    cout << thread.isRunning() << endl;
    thread.setInterval( 100000 );
    thread.start();
    
    sleep( 1 );
    
    thread.suspend();
    cout << "suspended." << endl;
        
    sleep( 1 );
    
    thread.resume();
    cout << "resumed." << endl;
    
    sleep( 1 );

    thread.stop();
    thread.join();
    cout << "joined." << endl;
    
    thread.caughtException().printStackTrace();
  }
  

  if ( false ) { 
    Thread thread;
    thread.start();
    sleep( 1 );
  }
  
  if ( false ) {
    Thread thread;
    thread.start( thread_func );
    sleep( 1 );
  }
  
  if ( false ) {
    MyThread thread;
    thread.start( thread );
    while ( thread.i < 200 ) {
      cout << ( thread.i += 100 ) << endl;
      sleep( 1 );
    }
    
    thread.join();
    cout << "joined" << endl;
  }

  if ( false ) {
    MyThread thread;
    Thread thread2;
    
    thread.setPriority( 10 );
    thread2.setPriority( 1 );

    thread.start( &thread );
    thread2.start( thread_func );
    
//     while ( thread.i < 100 ) {
//       cout << ( thread.i += 10 ) << endl;
//       sleep( 1 );
//     }
    
    thread.join();
    thread2.join();
    cout << "joined" << endl;
  }


  if ( false ) {
    MyThread thread;
    thread.start();
    int i = 0;
    while ( i < 200 ) {
      Thread t( Thread::self() );
      
      cout << ( i += 100 ) << endl;
      sleep( 1 );
    }
    
    thread.detach();
    cout << "join " << thread.join() << endl;
        
    sleep( 1 );
    cout << "join " << thread.join() << endl;
    sleep( 1 );
    cout << "end" << endl;
  }


  if ( false ) { 
    Thread thread;    
    thread.start( MyThread() );
    while ( true ) {
      cout << "isrunning " << thread.isRunning() << endl;
      sleep( 1 );
    }
  }

  if ( false ) {
    Thread thread;
    thread.start( thread_func2 );
    while ( true ) {
      bool isrun = thread.isRunning();
      cout << "isrunning " << isrun << endl;
      sleep( 1 );
      if ( !isrun ) {
        break;
      }
    }

    thread.caughtException().printStackTrace();
    throw thread.caughtException();
  }
  
  return 0;
}
