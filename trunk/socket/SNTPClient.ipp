/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <sys/time.h>
}


namespace cutil {


_INLINE_
timeval SNTPClient::getClock( const Address &ntpserver, int iteration, uint64_t timeout_usec ) 
  throw ( SocketException, SocketTimeoutException ) {
  
  timeval ret;
  ret.tv_sec = 0;
  ret.tv_usec = 0;
  
  Socket sock;
  sock.open( ntpserver.getFamily(), UDP );
  sock.bind();

  if ( !sock.connect( ntpserver ) ) {
    return ret;
  }
  
  uint32_t com[ 12 ];
  com[ 0 ] = htonl( 0x1b000000 );
  for ( int i = 1; i < 12; ++i ) {
    com[ i ] = 0;
  }
  
  const uint32_t TIME1970 = 2208988800UL;
  uint32_t data[ 12 ];
  timeval sent_tv, recv_tv;
  double delay = 0.0;
  
  if ( iteration < 1 ) {
    iteration = 1;
  }
  
  for ( int i = 0; i < iteration; ++i ){
    if ( !sock.isSendable( timeout_usec ) || 
	 !sock.sendAll( com, sizeof( com ) ) ||
         ::gettimeofday( &sent_tv, NULL ) !=  0 ||
	 !sock.isReceivable( timeout_usec ) || 
         !sock.recvAll( data, sizeof( data ) ) ||
         ::gettimeofday( &recv_tv, NULL ) != 0 ) {
      return ret;
    }

    //     printf( "sent: %u %u\n", sent_tv.tv_sec, sent_tv.tv_usec );
    //     printf( "    : %u %u\n", data[ 8 ], data[ 9 ] );
    //     printf( "recv: %u %u\n", recv_tv.tv_sec, recv_tv.tv_usec );
    //     printf( "    : %u %u\n", data[ 10 ], data[ 11 ] );
    
    double
      Td = (unsigned int)recv_tv.tv_sec + (double)recv_tv.tv_usec * 1e-6,
      Tc = ( ntohl( data[ 10 ] ) - TIME1970 ) + ( (double)ntohl( data[ 11 ] ) / 4294967296.0 ),
      Tb = ( ntohl( data[  8 ] ) - TIME1970 ) + ( (double)ntohl( data[  9 ] ) / 4294967296.0 ),
      Ta = (unsigned int)sent_tv.tv_sec + (double)sent_tv.tv_usec * 1e-6;
    
    //    printf( "%12.12f %12.12f %12.12f %12.12f\n", Ta, Tb, Tc, Td );
    
    delay += ( ( Td - Ta ) - ( Tc - Tb ) );
    ::usleep( 10000 );
  }
  
  delay /= ( 2.0 * iteration );
  
  if ( !sock.isSendable( timeout_usec ) ) {
    return ret;
  }
  sock.send( com, sizeof( com ) );


  if ( !sock.isReceivable( timeout_usec ) ) {
    return ret;
  }
  sock.recv( data, sizeof( data ) );
  
  ret.tv_sec = ( ntohl( data[ 10 ] ) - TIME1970 );
  ret.tv_usec = (long)( 1e6 * ( (double)ntohl( data[ 11 ] ) / 4294967296.0 + delay ) );
	
  return ret;
}

_INLINE_
bool SNTPClient::setClock( const Address &ntpserver, int iteration, uint64_t timeout_usec ) 
  throw ( SocketException, SocketTimeoutException ) {

  timeval tv = getClock( ntpserver, iteration, timeout_usec );
  if ( tv.tv_sec != 0 && tv.tv_usec != 0 ) {
    return ( ::settimeofday( &tv, NULL ) == 0 );
  }
  else {
    return false;
  }
}

}
