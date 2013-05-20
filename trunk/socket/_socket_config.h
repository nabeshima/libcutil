/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL__SOCKET_CONFIG_H_
#define _CUTIL__SOCKET_CONFIG_H_

/*!
  Definition of constants
*/


#if defined(_WIN32) || defined(__WIN32__) || defined(WIN32)
#  define CUTIL_WIN32
#endif

#ifdef CUTIL_WIN32
#  include <WinSock2.h>
#  pragma comment(lib,"ws2_32.lib")
#else
#  include <sys/socket.h>
#  include <sys/ioctl.h>
#  include <netinet/in.h>
#  include <netinet/in_systm.h>
#  include <netinet/ip.h>
#  include <netinet/ip_icmp.h>
#  include <net/ethernet.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <resolv.h>
#  include <unistd.h>
#  include <fcntl.h>
#  include <errno.h>
#endif


#include <algorithm>


namespace cutil{


#ifdef CUTIL_WIN32
  typedef SOCKET socket_t;
  typedef int sock_result_t;
  typedef int sock_len_t;
  typedef unsigned long sock_uint32_t;
#else
  typedef int socket_t;
  typedef ssize_t sock_result_t;
  typedef socklen_t sock_len_t;
  typedef u_int32_t sock_uint32_t;
#endif

//! initialize socket system
inline
bool startup_socket(){
#ifdef CUTIL_WIN32
  WSADATA wsaData;
  return WSAStartup( 0x0002, &wsaData ) == 0;
#else
  return true;
#endif
}

//! finalize socket system 
inline
void cleanup_socket(){
#ifdef CUTIL_WIN32
  WSACleanup();
#endif
}

enum INFamily { 
  IPv4 = AF_INET, 
  IPv6 = AF_INET6
};

//! Protocols for socket
enum SocketType { 
  RAW = SOCK_RAW,
  UDP = SOCK_DGRAM,
  TCP = SOCK_STREAM
};


//! endian conversion.
template< class VALUE >
inline 
VALUE reverse_endian( VALUE value ){
  char volatile* first = reinterpret_cast< char volatile* >( &value );
  char volatile* last = first + sizeof( VALUE );
  std::reverse( first, last );
  return value;
}

template< class VALUE >
inline 
VALUE ntoh( VALUE value ){
#ifdef  __BIG_ENDIAN__
  return value;
#else
  return reverse_endian( value );
#endif
}

template< class VALUE >
inline 
VALUE hton( VALUE value ){
#ifdef  __BIG_ENDIAN__
  return value;
#else
  return reverse_endian( value );
#endif
}

}

#endif
