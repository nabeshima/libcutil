/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Socket.h"
#include "Thread.h"

#include <sstream>

using namespace std;
using namespace cutil;

void thread_func() {
  Socket socket_listen;
  socket_listen.open( IPv4, TCP );
  socket_listen.bind( 15433 );
  cout << "bound " << socket_listen.boundPort() << endl;
  Socket socket = socket_listen.accept();
  
  cout << "bound " << socket_listen.boundPort() << " " << socket.isOpen() << endl;
  
  
  while ( true ) {
    long c;
    try {
      cout << "INQ 1 : " << socket.byteInRecvQueue() << endl;
      
      socket.isReceivable( 1000000 );
      if ( socket.recv( &c, sizeof( c ) ) == 0 ) {
        cout << "connection is down (recv)" << endl;
        break;
      }
      
      cout << "RECV: " << c << " from " << socket.fromAddress().toString() << endl;
      cout << "INQ 2 : " << socket.byteInRecvQueue() << endl;
      
      if ( c == 1000000 ) {
        break;
      }
      sleep( 1 );
    }
    catch ( const SocketTimeoutException &e ) {
      cout << e.what() << " from " << e.where() << endl;
    }
  }
}

int main( int argc, char **argv ) {
  Thread thread;
  thread.start( thread_func );

  Socket socket;


  vector< Address > res;
  Address::resolve( res, "127.0.0.1" );
  Address peer;
  
  for ( unsigned int i = 0; i < res.size(); ++i ) {
    if ( res[ i ].getFamily() == IPv4 ) {
      res[ i ].setPort( 15433 );
      peer = res[ i ];
    }
  }

  
  while ( true ) {    
    try {
      socket.open( peer.getFamily(), TCP );
      
      bool ret = socket.connect( peer );
      if ( !ret ) {
        cout << "connection failure" << endl;
        sleep( 1 );
        continue;
      }
      if ( ret ) {
        cout << "connection success" << endl;
        break;
      }
    }
    catch ( const Exception &e ) {
      cout << e.what() << " from " << e.where() << endl;
    }
  }

  usleep( 1000000 );
  
  long l = 0;
  while ( true ) {
    l += 0x000000fa;
    cout << "SEND: " << l << endl;
    
//     stringstream sstrm;
//     sstrm << l;
    
    cout << "OUTQ 0 : " << socket.byteInSendQueue() << endl;    
    //    if ( !socket.sendString( sstrm.str() ) ) {
    if ( !socket.send( &l, sizeof( l ) ) ) {
      cout << "connection is down (send)" << endl;
      break;
    }
    cout << "OUTQ 1 : " << socket.byteInSendQueue() << endl;    
    
    if ( l > 1000 ) {
      l = 1000000;
      socket.send( &l, sizeof( l ) );
      break;
    }
    usleep( 1000000 );
  }

  thread.join();

  return 0;
}
