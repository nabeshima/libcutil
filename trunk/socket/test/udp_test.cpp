/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Socket.h"
#include "Thread.h"

#include <sstream>
#include <vector>

using namespace std;
using namespace cutil;

void thread_func_body( int mode = 0 ) {
  Socket socket;
  
  socket.open( IPv6, UDP );
  cout << "bind: " << socket.bind( 55725 ) << endl;

  // if only to receive, joinMulticastGroup is not needed.
  //   if ( mode == 1 ) {
  //     vector< Address > res;
  //     //    Address::resolve( "239.255.255.255", res );
  //     Address::resolve( res, "ff02::666" );
  //     for ( unsigned int i = 0; i < res.size(); ++i ) {
  //       if ( res[ i ].getFamily() == IPv6 ) {
  // 	try {
  // 	  socket.joinMulticastGroup( res[ i ] );   // multicast
  // 	}
  // 	catch ( Exception &e ){
  // 	  cout << e.what() << endl;
  // 	}
  // 	break;
  //       }
  //     }
  //   }
  
  int timeout_count = 0;
  
  long prev_l = -1;
  while ( true ) {
    long l;
    try {
      cout << "INQ 1: " << socket.byteInRecvQueue() << endl;
      if ( socket.isReceivable( 1000000 ) )
	socket.recv( &l, sizeof( l ), true );
      cout << "RECV peek : " << l << " from " << socket.fromAddress().toString() << endl;
      cout << "INQ 2: " << socket.byteInRecvQueue() << endl;
      if ( socket.isReceivable( 1000000 ) )
	socket.recv( &l, sizeof( l ) );
      cout << "RECV      : " << l << " from " << socket.fromAddress().toString() << endl;
      cout << "INQ 3: " << socket.byteInRecvQueue() << endl;
      if ( l == prev_l ) {
	break;
      }
      prev_l = l;
      
      usleep( 1000000 );
    }
    catch ( const SocketTimeoutException e ) {
      cout << e.what() << " from " << e.where() << endl;
      ++timeout_count;
      if ( timeout_count > 3 ) {
        break;
      }
    }
  }
}

void thread_func0() {
  thread_func_body( 0 );
}

void thread_func1() {
  thread_func_body( 1 );
}

void thread_func2() {
  thread_func_body( 2 );
}

int main( int argc, char **argv ) {
  
  {
    Thread thread;
    thread.start( thread_func0 );

    Socket socket;
    //    socket.open( UDP );
    socket.open( IPv6, UDP );
    
    vector< Address > res;
    
    try {
      //      Address::resolve( "127.0.0.1", res );
      Address::resolve( res, "::1" );
    }
    catch ( Exception &e ) {
      e.printStackTrace();
      throw e;
    }
    
    for ( unsigned int i = 0; i < res.size(); ++i ) {
      cout << "Chost " << res[ i ].toString() << endl;
    }
    res[ 0 ].setPort( 55725 );
    cout << "connect " <<  socket.connect( res[ 0 ] ) << endl;         // unicast

    cout << "###" << endl;
    cout << "multicast: " << socket.toAddress().isMulticast() << endl;
    cout << "broadcast?: " << socket.toAddress().toString() << endl;
    
    long l = 0;
    while ( true ) {
      l += 0x000000fa;
      cout << "OUTQ 0 : " << socket.byteInSendQueue() << endl;
      cout << "SEND: " << l << endl;
      socket.send( &l, sizeof( l ) );
      cout << "OUTQ 1 : " << socket.byteInSendQueue() << endl;
      
      if ( l > 1000 ) {
        socket.send( &l, sizeof( l ) );
        break;
      }
      usleep( 1000000 );
    }
    thread.join();
  }

  {
    Thread thread;
    thread.start( thread_func2 );
    
    Socket socket;
    socket.open( IPv4, UDP );
    
    vector< Address > res;
    try {
      Address::resolve( res, "255.255.255.255" );
    }
    catch ( Exception &e ) {
      e.printStackTrace();
    }

    for ( unsigned int i = 0; i < res.size(); ++i ) {
      cout << "Chost " << res[ i ].toString() << endl;
    }
    
    res[ 0 ].setPort( 55725 );
    socket.connect( res[ 0 ] );   // broadcast
    socket.setBroadcast( true );

    cout << "###" << endl;
    cout << "multicast: " << socket.toAddress().isMulticast() << endl;
    cout << "broadcast?: " << socket.toAddress().toString() << endl;
    
    long l = 0;
    while ( true ) {
      l += 0x000000fa;
      cout << "OUTQ 0 : " << socket.byteInSendQueue() << endl;
      cout << "SEND: " << l << endl;
      socket.send( &l, sizeof( l ) );
      cout << "OUTQ 1 : " << socket.byteInSendQueue() << endl;
      
      if ( l > 1000 ) {
        socket.send( &l, sizeof( l ) );
        break;
      }
      usleep( 1000000 );
    }
    thread.join();
  }
  
  {
    Thread thread;
    thread.start( thread_func1 );
    
    Socket socket;
    //    socket.open( UDP );
    socket.open( IPv6, UDP );

    vector< Address > res;
    //    Address::resolve( "239.255.255.255", res );
    Address::resolve( res, "ff02::666" );


    for ( unsigned int i = 0; i < res.size(); ++i ) {
      cout << "Chost " << res[ i ].toString() << endl;
    }
    
    res[ 0 ].setPort( 55725 );
    
    cout << "connect: " << socket.connect( res[ 0 ] ) << endl;   // multicast
    
    cout << "###" << endl;
    cout << "multicast: " << socket.toAddress().isMulticast() << endl;
    cout << "broadcast?: " << socket.toAddress().toString() << endl;

    socket.joinMulticastGroup( res[ 0 ] );   // multicast    

    long l = 0;
    while ( true ) {
      l += 0x000000fa;
      cout << "OUTQ 0 : " << socket.byteInSendQueue() << endl;
      cout << "SEND: " << l << endl;
      socket.send( &l, sizeof( l ) );
      cout << "OUTQ 1 : " << socket.byteInSendQueue() << endl;
      
      if ( l > 1000 ) {
        socket.send( &l, sizeof( l ) );
        break;
      }
      usleep( 1000000 );
    }
    thread.join();
  }
 
  return 0;
}
