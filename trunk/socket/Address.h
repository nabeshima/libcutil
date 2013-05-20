/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_ADDRESS_H_
#define _CUTIL_ADDRESS_H_

#include <string>

#include "_socket_config.h"
#include "AddressException.h"

namespace cutil {

/*!
  IP address and port.
*/
class Address {
private:
  sockaddr_storage storage;
  
public:
  Address() throw ();
  Address( const Address &address ) throw ();
  Address( const in_addr &iaddr, uint16_t port ) throw ();
  Address( const in6_addr &iaddr6, uint16_t port ) throw ();
  
  Address& operator=( const Address &address ) throw ();
  
  void clear() throw ();

  void setAddress( const Address &address ) throw ();  
  void setAddress( const in_addr &iaddr, uint16_t port ) throw ();
  void setAddress( const in6_addr &iaddr6, uint16_t port ) throw ();
  
  INFamily getFamily() const throw ();

  bool isIPv4() const throw ();
  bool isIPv6() const throw ();
  
  sockaddr_in& asIPv4() throw ();
  sockaddr_in6& asIPv6() throw ();
  sockaddr_storage& asStorage() throw ();
  const sockaddr_in& asIPv4() const throw ();
  const sockaddr_in6& asIPv6() const throw ();
  const sockaddr_storage& asStorage() const throw ();
  
  void setPort( uint16_t port ) throw ();
  uint16_t getPort() const throw ();

  std::string toString() const throw ();    
  
  bool isMulticast() const throw ();

  bool operator==( const Address &address ) const throw ();
  bool operator!=( const Address &address ) const throw ();

  // resolve hostname
  template< template < typename, typename > class Sequence,
            typename Allocator >
  static void resolve( Sequence< Address, Allocator > &result, 
                       const std::string &host, 
                       bool numeric_host = true )
    throw ( ResolveHostnameException );
};

}


// #ifndef _NO_INLINE_
// #define _INLINE_ inline
#include "Address.ipp"
// #endif

#endif
