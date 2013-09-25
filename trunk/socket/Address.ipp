/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <memory.h>
}

namespace cutil {

inline // _INLINE_
Address::Address() throw () {
  clear();
}

inline // _INLINE_
Address::Address( const Address &address ) throw () {
  setAddress( address );
}

inline // _INLINE_
Address::Address( const in_addr &iaddr, uint16_t port ) throw () {
  setAddress( iaddr, port );
}

inline // _INLINE_
Address::Address( const in6_addr &iaddr6, uint16_t port ) throw () {
  setAddress( iaddr6, port );
}

inline // _INLINE_
Address& Address::operator=( const Address &address ) throw () {
  setAddress( address );
  return *this;
}

inline // _INLINE_
void Address::clear() throw () {
  ::memset( &storage, 0, sizeof( storage ) );
  storage.ss_family = IPv4;
}

inline // _INLINE_
void Address::setAddress( const Address &address ) throw () {
  storage = address.storage;
}

inline // _INLINE_
void Address::setAddress( const in_addr &iaddr, uint16_t port ) throw () {
  clear();
  sockaddr_in &saddr = asIPv4();
#if defined(__APPLE__) && !defined(__FreeBSD__)
  saddr.sin_len = sizeof( sockaddr_in );
#endif
  saddr.sin_family = IPv4;
  saddr.sin_addr = iaddr;
  saddr.sin_port = htons( port );
}

inline // _INLINE_
void Address::setAddress( const in6_addr &iaddr6, uint16_t port ) throw () {
  clear();
  sockaddr_in6 &saddr = asIPv6();
#if defined(__APPLE__) && !defined(__FreeBSD__)
  saddr.sin6_len = sizeof( sockaddr_in6 );
#endif
  saddr.sin6_family = IPv6;
  saddr.sin6_addr = iaddr6;
  saddr.sin6_port = htons( port );
}

inline // _INLINE_
INFamily Address::getFamily() const throw () {
  return (INFamily)storage.ss_family;
}

// to support no ss_len environment
inline // _INLINE_
int Address::getAddressLength() const throw () {
  return ( isIPv4()? sizeof( sockaddr_in ): sizeof( sockaddr_in6 ) );
}

inline // _INLINE_
bool Address::isIPv4() const throw () {
  return ( getFamily() == IPv4 );
}

inline // _INLINE_
bool Address::isIPv6() const throw () {
  return ( getFamily() == IPv6 );
}

inline // _INLINE_
sockaddr_in& Address::asIPv4() throw () {
  return *(sockaddr_in*)&storage;
}

inline // _INLINE_
sockaddr_in6& Address::asIPv6() throw () {
  return *(sockaddr_in6*)&storage;
}

inline // _INLINE_
sockaddr_storage& Address::asStorage() throw () {
  return storage;
}

inline // _INLINE_
const sockaddr_in& Address::asIPv4() const throw () {
  return *(sockaddr_in*)&storage;
}

inline // _INLINE_
const sockaddr_in6& Address::asIPv6() const throw () {
  return *(sockaddr_in6*)&storage;
}

inline // _INLINE_
const sockaddr_storage& Address::asStorage() const throw () {
  return storage;
}

inline // _INLINE_
void Address::setPort( uint16_t port ) throw () {
  switch ( getFamily() ) {
  case IPv4:
    asIPv4().sin_port = htons( port );
    break;
  case IPv6:
    asIPv6().sin6_port = htons( port );
    break;
  }
}

inline // _INLINE_
uint16_t Address::getPort() const throw () {
  switch ( getFamily() ) {
  case IPv4:
    return ntohs( asIPv4().sin_port );
  case IPv6:
    return ntohs( asIPv6().sin6_port );
  default:
    return 0;
  }
}

inline // _INLINE_
std::string Address::toString() const throw () {
  char strbuf[ INET6_ADDRSTRLEN ];
  
  switch ( getFamily() ) {
  case IPv4:
    ::inet_ntop( IPv4, &asIPv4().sin_addr, strbuf, sizeof( strbuf ) );
    break;
  case IPv6:
    ::inet_ntop( IPv6, &asIPv6().sin6_addr, strbuf, sizeof( strbuf ) );
    break;
  }
  
  return std::string( strbuf );
}

inline // _INLINE_
bool Address::isMulticast() const throw () {
  return 
    ( isIPv4() && ( ( asIPv4().sin_addr.s_addr & 0xf0 ) == 0xe0 ) ) || // little endian, MSB is 0xe0 or not.
    ( isIPv6() && ( asIPv6().sin6_addr.s6_addr[ 0 ] == 0xff ) );       // MSB is 0xff
}

inline // _INLINE_
bool Address::operator==( const Address &address ) const throw () {
  if ( getFamily() != address.getFamily() ) {
    return false;
  }
  
  switch ( getFamily() ) {
  case IPv4:
    return 
      ::memcmp( &asIPv4().sin_addr, &address.asIPv4().sin_addr, sizeof( in_addr ) ) == 0 &&
      asIPv4().sin_port == address.asIPv4().sin_port;
  case IPv6:
    return
      ::memcmp( &asIPv6().sin6_addr, &address.asIPv6().sin6_addr, sizeof( in6_addr ) ) == 0 &&
      asIPv6().sin6_port == address.asIPv6().sin6_port;
  default:
    return false;
  }
}

inline // _INLINE_
bool Address::operator!=( const Address &address ) const throw () {
  return !( *this == address );
}

template< template < typename, typename > class Sequence,
          typename Allocator >
inline // _INLINE_
void Address::resolve( Sequence< Address, Allocator > &result, 
                       const std::string &host,
                       bool numeric_host )
  throw ( ResolveHostnameException ) {

  result.clear();

  {  
    res_state resp = &_res;
    resp->retrans  = 1;      //! DNS request interval seconds >= 1
    resp->retry    = 1;      //! DNS request trials >= 1
  }
  
  addrinfo* res = NULL;
  addrinfo hints;
  ::memset( &hints, 0, sizeof( hints ) );
  
  hints.ai_family = AF_UNSPEC;
  //  hints.ai_protocol = TCP;
  //  hints.ai_flags = AI_NUMERICSERV;  // comment out to avoid Mac OS X bug
  if ( numeric_host ) {
    hints.ai_flags |= AI_NUMERICHOST;
  }
  
  int err = ::getaddrinfo( host.c_str(), 
                           "0",          // port is not assigned
                           &hints,
                           &res );       // any NIC and any protocol
  
  if ( err != 0 ) {
    ::freeaddrinfo( res );  
    throw ResolveHostnameException( std::string( "Resolving hostname " ) + 
                                    host + " is failed: " +
                                    ::gai_strerror( err ), __EXCEPTION_INFO__ );
  }
  
  for ( addrinfo *i = res; i != NULL; i = i->ai_next ) {
    
    Address tmp;

    switch ( i->ai_family ) {
    case IPv4:
      tmp.setAddress( ( (sockaddr_in*)i->ai_addr )->sin_addr, 0 );
      break;
    case IPv6:
      tmp.setAddress( ( (sockaddr_in6*)i->ai_addr )->sin6_addr, 0 );
      break;
    }

    if ( result.size() == 0 || 
         tmp != result.back() ) {
      result.push_back( tmp );
    }
  }
  
  ::freeaddrinfo( res );  
}

}
