/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cstring>

namespace cutil {

unsigned short PingClient::checksum( unsigned short *buf, int size ) throw () {
  unsigned long sum = 0;
  
  while ( size > 1 ) {
    sum += *buf;
    ++buf;
    size -= 2;
  }

  if ( size == 1 ) {
    sum += *(unsigned char *)buf;
  }
  
  sum = ( sum & 0xffff ) + ( sum >> 16 );
  sum = ( sum & 0xffff ) + ( sum >> 16 );
  
  return ~sum;
}


bool PingClient::ping( const Address &peer, uint64_t timeout_usec )
  throw ( SocketException, SocketTimeoutException ) {

  Socket socket;
  if ( !socket.open( peer.getFamily(), RAW ) ) {
    throw SocketException( "fail to create raw socket", __EXCEPTION_INFO__ );
  }
  
  socket.connect( peer );
  
  {
    icmp hdr;
    ::memset( &hdr, 0, sizeof( hdr ) );
    hdr.icmp_type = ICMP_ECHO;
    hdr.icmp_code = 0;
    hdr.icmp_cksum = 0;
    hdr.icmp_hun.ih_idseq.icd_id = 0;
    hdr.icmp_hun.ih_idseq.icd_seq = htons( 10 );
    hdr.icmp_cksum = checksum( (unsigned short*)&hdr, sizeof( hdr ) );
    
    if ( socket.send( &hdr, sizeof( hdr ) ) < 1 ) {
      return false;
    }
  }
  {
    char buf[ 2000 ];
    memset( buf, 0, sizeof( buf ) );
    
    if ( !socket.isReceivable( timeout_usec ) || 
	 socket.recv( buf, sizeof( buf ) ) < 1 ) {
      return false;
    }
    
    ip *iphdrptr = (ip*)buf;
    icmp *icmphdrptr = (icmp*)( buf + ( iphdrptr->ip_hl * 4 ) );

    if ( icmphdrptr->icmp_type == ICMP_ECHOREPLY ) {
      return true;
    } 
    else {
      return false;
    }
  }  
}

}
