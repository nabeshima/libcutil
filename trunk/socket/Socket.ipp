/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cutil {



_INLINE_
Socket::Socket() throw ()
  : ref_counter( new int ), 
    sock( -1 ),
    family( IPv4 ),
    sock_type( UDP ) {
  (*ref_counter) = 1;
}


_INLINE_
Socket::Socket( const Socket &s )  throw ()
  : ref_counter( s.ref_counter ),
    sock( s.sock ),
    family( s.family ),
    sock_type( s.sock_type ),
    local_addr( s.local_addr ),
    to_addr( s.to_addr ),
    from_addr( s.from_addr ) {
  ++(*ref_counter);
}

_INLINE_
Socket::~Socket() throw () {
  --(*ref_counter);
  
  if ( (*ref_counter) == 0 ) {
    close();
    delete ref_counter;
  }
}


_INLINE_
Socket& Socket::operator=( const Socket &s ) throw () {
  --(*ref_counter);
  
  if ( (*ref_counter) == 0 ) {
    close();
    delete ref_counter;
  }
  
  ref_counter = s.ref_counter;
  ++(*ref_counter);
  
  sock = s.sock;
  family = s.family;
  sock_type = s.sock_type;
  local_addr = s.local_addr;
  to_addr = s.to_addr;
  from_addr = s.from_addr;

  return *this;
}

_INLINE_
INFamily Socket::getFamily() const throw () {
  return family;
}

_INLINE_
SocketType Socket::getType() const throw () {
  return sock_type;
}

_INLINE_
socket_t Socket::socket() const throw () {
  return sock;
}


_INLINE_
bool Socket::open( INFamily family, SocketType sock_type ) throw () {
  if ( isOpen() ) {
    return false;
  }
  
  // socket open
  switch ( sock_type ) {
  case TCP:
  case UDP:
    sock = ::socket( family, sock_type, 0 );
    break;
  case RAW:
    sock = ::socket( family, sock_type, IPPROTO_ICMP );
    break;
  }
  
  this->family = family;
  this->sock_type = sock_type;
  
  return isOpen();
}


_INLINE_
bool Socket::close() throw () {
  if( !isOpen() ){
    return false;
  }
  
#ifdef CUTIL_WIN32
  ::closesocket( sock );
#else
  ::close( sock );
#endif
  
  sock = -1;
  local_addr.clear();
  to_addr.clear();
  from_addr.clear();
  
  return true;
}

_INLINE_
bool Socket::isOpen() const throw () {
  return ( sock != -1 );
}


_INLINE_
bool Socket::connect( const Address &to_addr, uint64_t timeout_usec )
  throw ( SocketException, 
	  SocketTimeoutException, 
	  SocketFamilyUnmatchException ) {
  
  if ( !isOpen() ) {
    return false;
  }
  
  if ( getFamily() != to_addr.getFamily() ) {
    throw SocketFamilyUnmatchException( "socket and address are unmatched in INFamly; IPv4 and IPv6", __EXCEPTION_INFO__ );
  }
  
  if ( getType() != TCP ) {
    this->to_addr = to_addr;
    return true;
  }
  
  if ( timeout_usec == 0 ) {  // blocking
    if ( ::connect( sock, (const sockaddr*)&to_addr.asStorage(), to_addr.getAddressLength() ) < 0 ) {
      close();
      return false;
    }
  }
  else  { // non-blocking
    
    //! make socket non-blocking
    int flag = ::fcntl( sock, F_GETFL, NULL );
    if ( flag < 0 ) {
      close();
      throw SocketException( "::fcntl( GET ) error", __EXCEPTION_INFO__ );
    }
    
    if ( ::fcntl( sock, F_SETFL, flag | O_NONBLOCK ) < 0 ) {
      close();
      throw SocketException( "::fcntl( NONBLOCK ) error", __EXCEPTION_INFO__ );
    }
    
#if defined(__APPLE__) && !defined(__FreeBSD__)
    const int one = 1;
    if ( ::setsockopt( sock, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof( one ) ) == -1 ) {
      throw SocketException( "::setsockopt SO_NOSIGPIPE error", __EXCEPTION_INFO__ );
    }
#endif
    
    if ( ::connect( sock, (const sockaddr*)&to_addr.asStorage(), to_addr.getAddressLength() ) < 0 ) {
      if ( errno != EINPROGRESS ) {
	close();
	
	if ( ::fcntl( sock, F_SETFL, flag ) < 0 ) {
	  throw SocketException( "::connect and ::fcntl( END ) error", __EXCEPTION_INFO__ );
	}
	else {
	  throw SocketException( "::connect error", __EXCEPTION_INFO__ );
	}
      }
        
      fd_set wmask;
      FD_ZERO( &wmask );
      FD_SET( sock, &wmask );
      
      timeval waitval;
      waitval.tv_sec = timeout_usec / 1000000;
      waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;
      
      int rc = ::select( sock + 1, NULL, &wmask, NULL, &waitval );
      
      if ( rc == 0 ) {
	if ( ::fcntl( sock, F_SETFL, flag ) < 0 ) {
	  throw SocketTimeoutException( "timeout ::connect and ::fcntl( END ) error", __EXCEPTION_INFO__ );
	}
	throw SocketTimeoutException( "timeout ::connect", __EXCEPTION_INFO__ );
      }
      else if ( rc < 0 ) {
	if ( ::fcntl( sock, F_SETFL, flag ) < 0 ) {
	  close();
	  throw SocketException( "::select and ::fcntl( END ) error", __EXCEPTION_INFO__ );
	}
	close();
	throw SocketException( "::select error", __EXCEPTION_INFO__ );
      }
    }
    

    //! restore socket flag
    if ( ::fcntl( sock, F_SETFL, flag ) < 0 ) {
      close();
      throw SocketException( "::fcntl( END ) error", __EXCEPTION_INFO__ );
    }
  }
  
  // check actually connected server address.
  socklen_t len = sizeof( this->to_addr.asStorage() );
  if ( ::getpeername( sock, (sockaddr*)&this->to_addr.asStorage(), &len ) == -1 ) {
    close();
    return false;
  }
  
  return true;    
}

_INLINE_
bool Socket::setBroadcast( bool enable ) 
  throw ( SocketException,
	  SocketTypeUnmatchException ) {

  if ( !isOpen() ) {
    return false;
  }

  if ( getType() == TCP ) {
    throw SocketTypeUnmatchException( "socket type to connect() is not UDP nor RAW", __EXCEPTION_INFO__ );
  }
  
  const int one = ( enable )? 1: 0;
  if ( ::setsockopt( sock, SOL_SOCKET, SO_BROADCAST, &one, sizeof( one ) ) == -1 ) {
    throw SocketException( "::setsockopt SO_BROADCAST error", __EXCEPTION_INFO__ );
  }

  return true;
}

_INLINE_
bool Socket::joinMulticastGroup( const Address &group_addr )
  throw ( SocketException,
	  SocketFamilyUnmatchException, 
	  SocketTypeUnmatchException ) {

  if ( !isOpen() ||
       !group_addr.isMulticast() ) {
    return false;
  }
  
  if ( getFamily() != group_addr.getFamily() ) {
    throw SocketFamilyUnmatchException( "socket and address are unmatched in INFamily; IPv4 and IPv6", __EXCEPTION_INFO__ );
  }  
  
  if ( getType() != UDP ) {
    throw SocketTypeUnmatchException( "socket type to connect() is not UDP", __EXCEPTION_INFO__ );
  }
  
  bool ret = false;

  switch ( getFamily() ) {
  case IPv4:
    {
      const sockaddr_in& saddr = group_addr.asIPv4();

      //! prevent loopback
      //       const int zero = 0;
      //       ::setsockopt( sock, IPPROTO_IP, IP_MULTICAST_LOOP, &zero, sizeof( zero ) );
      
      //! choose interface
      in_addr iaddr;
      iaddr.s_addr = INADDR_ANY;
      if ( ::setsockopt( sock, IPPROTO_IP, IP_MULTICAST_IF, &iaddr, sizeof( iaddr ) ) == -1 ) {
	throw SocketException( "::setsockopt IP_MULTICAST_IF error", __EXCEPTION_INFO__ );
      }
      
      //! set TTL
      uint8_t lTTL = 2;
      if ( ::setsockopt( sock, IPPROTO_IP, IP_MULTICAST_TTL, &lTTL, sizeof( lTTL ) ) == -1 ) {
	throw SocketException( "::setsockopt IP_MULTICAST_TTL error", __EXCEPTION_INFO__ );
      }
      
      //! join multicast group
      ip_mreq mreq;
      mreq.imr_interface.s_addr = INADDR_ANY;
      mreq.imr_multiaddr = saddr.sin_addr;
      if ( ::setsockopt( sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof( mreq ) ) == -1 ) {
	throw SocketException( "::setsockopt IP_ADD_MEMBERSHIP error", __EXCEPTION_INFO__ );
      }
      
      ret = true;
    }
  case IPv6:
    {
      const sockaddr_in6& saddr6 = group_addr.asIPv6();
      
      ipv6_mreq mreq6;
      mreq6.ipv6mr_interface = 1;                  // if_nametoindex( "eth0" ) is applicable if needed. 0 causes error.
      mreq6.ipv6mr_multiaddr = saddr6.sin6_addr;
      
      //! choose interface
      if ( ::setsockopt( sock, IPPROTO_IPV6, IPV6_MULTICAST_IF, &mreq6.ipv6mr_interface, sizeof( mreq6.ipv6mr_interface ) ) == -1 ) {
	throw SocketException( "::setsockopt IPV6_MULTICAST_IF error", __EXCEPTION_INFO__ );
      }
    
      //! set HOPS
      uint32_t lhops = 2;
      if ( ::setsockopt( sock, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &lhops, sizeof( lhops ) ) == -1 ) {
	throw SocketException( "::setsockopt IPV6_MULTICAST_HOPS error", __EXCEPTION_INFO__ );
      }
      
      //! join multicast group
      if ( ::setsockopt( sock, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6, sizeof( mreq6 ) ) == -1 ) {
	throw SocketException( "::setsockopt IPV6_JOIN_GROUP error", __EXCEPTION_INFO__ );
      }
      
      ret = true;
    }
  }

  return ret;
}

_INLINE_
bool Socket::leaveMulticastGroup( const Address &group_addr ) 
  throw ( SocketException,
	  SocketFamilyUnmatchException, 
	  SocketTypeUnmatchException ) {
  
  if ( !isOpen() ||
       !group_addr.isMulticast() ) {
    return false;
  }
  
  if ( getFamily() != group_addr.getFamily() ) {
    throw SocketFamilyUnmatchException( "socket and address are unmatched in INFamily; IPv4 and IPv6", __EXCEPTION_INFO__ );
  }  
  
  if ( getType() != UDP ) {
    throw SocketTypeUnmatchException( "socket type to connect() is not UDP", __EXCEPTION_INFO__ );
  }
  
  bool ret = false;

  switch ( getFamily() ) {
  case IPv4:
    {
      const sockaddr_in& saddr = group_addr.asIPv4();
      ip_mreq mreq;
      mreq.imr_interface.s_addr = INADDR_ANY;
      mreq.imr_multiaddr = saddr.sin_addr;
      if ( ::setsockopt( sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq, sizeof( mreq ) ) == -1 ) {
	throw SocketException( "::setsockopt IP_DROP_MEMBERSHIP error", __EXCEPTION_INFO__ );
      }
      
      ret = true;
    }
  case IPv6:
    {
      const sockaddr_in6& saddr6 = group_addr.asIPv6();
      
      ipv6_mreq mreq6;
      mreq6.ipv6mr_interface = 1;                  // default interface. if_nametoindex( "eth0" ) is applicable if needed.
      mreq6.ipv6mr_multiaddr = saddr6.sin6_addr;
      if ( ::setsockopt( sock, IPPROTO_IPV6, IPV6_LEAVE_GROUP, &mreq6, sizeof( mreq6 ) ) == -1 ) {
	throw SocketException( "::setsockopt IPV6_LEAVE_GROUP error", __EXCEPTION_INFO__ );
      }
      
      ret = true;
    }
  }

  return ret;
}


_INLINE_
bool Socket::bind( uint16_t port, int backlog ) 
  throw ( SocketException ) {
  
  if ( !isOpen() || 
       isBound() ) {
    return false;
  }
  
  // port reusing
  const int one = 1;
  if ( ::setsockopt( sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof( one ) ) == -1 ) {
    throw SocketException( "::setsockopt SO_REUSEADDR error", __EXCEPTION_INFO__ );
  }
  
  // prepare address.
  sockaddr_in locv4;
  sockaddr_in6 locv6;
  
  sockaddr *loc;
  socklen_t len;
  
  if ( getFamily() == IPv4 ) {
    locv4.sin_addr.s_addr = INADDR_ANY;
    locv4.sin_port = cutil::hton( port );
    len = sizeof( locv4 );
    loc = (sockaddr*)&locv4;
  }
  else {
    ::memset( locv6.sin6_addr.s6_addr, 0, sizeof( locv6.sin6_addr.s6_addr ) );
    locv6.sin6_port = cutil::hton( port );
    len = sizeof( locv6 );
    loc = (sockaddr*)&locv6;
  }
  loc->sa_family = getFamily();
  
  if ( ::bind( sock, loc, len ) == -1 ) {
    close();
    return false;
  }

  // check actually assigned port (for port 0).
  len = sizeof( this->to_addr.asStorage() );
  if ( ::getsockname( sock, (sockaddr*)&this->local_addr.asStorage(), &len ) == -1 ) {
    close();
    return false;
  }
  
  if ( getType() == TCP &&
       ::listen( sock, backlog ) == -1 ) {
    throw SocketException( "::listen error", __EXCEPTION_INFO__ );
  }
  return true;
}


_INLINE_
bool Socket::isBound() const throw () {
  return ( local_addr != Address() );
}

_INLINE_
uint16_t Socket::boundPort() const throw () {
  return local_addr.getPort();
}

_INLINE_
const Address& Socket::localAddress() const throw () {
  return local_addr;
}

_INLINE_
const Address& Socket::toAddress() const throw () {
  return to_addr;
}

_INLINE_
const Address& Socket::fromAddress() const throw () {
  return from_addr;
}


_INLINE_
bool Socket::isAcceptable( uint64_t timeout_usec ) 
  throw ( SocketException, SocketTimeoutException, SocketTypeUnmatchException ) {
  if ( !isBound() ) {
    return false;
  }
  
  if ( getType() != TCP ) {
    throw SocketTypeUnmatchException( "socket type to isAcceptable() is not TCP", __EXCEPTION_INFO__ );
  }
  
  fd_set rmask;
  FD_ZERO( &rmask );
  FD_SET( sock, &rmask );
  
  timeval waitval;
  waitval.tv_sec = timeout_usec / 1000000;
  waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;
  
  int rc = ::select( sock + 1, &rmask, NULL, NULL, &waitval );
  
  if ( rc == 0 ) {
    throw SocketTimeoutException( "timeout ::select", __EXCEPTION_INFO__ );
  }
  else if ( rc < 0 ) {
    close();
    throw SocketException( "::select error", __EXCEPTION_INFO__ );
  }
  
  return ( FD_ISSET( sock, &rmask ) != 0 );
}


_INLINE_
Socket Socket::accept() 
  throw ( SocketException, SocketTypeUnmatchException ) {
  if ( !isBound() ) {
    return Socket();
  }
  
  if ( getType() != TCP ) {
    throw SocketTypeUnmatchException( "socket type to accept() is not TCP", __EXCEPTION_INFO__ );
  }

  
  sockaddr_storage saddrs;
  socklen_t len = sizeof( saddrs );
  
  Socket ret;    
  ret.sock = ::accept( sock, (sockaddr*)&ret.from_addr.asStorage(), &len );

  // to_addr is the same as from_addr because of connection.
  ret.to_addr = ret.from_addr;
  
  const int one = 1;
  if ( ::setsockopt( ret.sock, SOL_SOCKET, SO_KEEPALIVE, &one, sizeof( one ) ) < -1 ) {
    throw SocketException( "::setsockopt SO_KEEPALIVE error", __EXCEPTION_INFO__ );
  }
      
#if defined(__APPLE__) && !defined(__FreeBSD__)
  if ( ::setsockopt( ret.sock, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof( one ) ) < -1 ) {
    throw SocketException( "::setsockopt SO_NOSIGPIPE error", __EXCEPTION_INFO__ );
  }
#endif
  
  return ret;
}


_INLINE_
bool Socket::isSendable( uint64_t timeout_usec ) 
  throw ( SocketException, SocketTimeoutException ) {
  if ( !isOpen() ) {
    return false;
  }
  
  fd_set wmask;
  FD_ZERO( &wmask );
  FD_SET( sock, &wmask );
  
  timeval waitval;
  waitval.tv_sec = timeout_usec / 1000000;
  waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;
  
  int rc = ::select( sock + 1, NULL, &wmask, NULL, &waitval );
  
  if ( rc == 0 ) {
    throw SocketTimeoutException( "timeout ::select", __EXCEPTION_INFO__ );
  }
  else if ( rc < 0 ) {
    close();
    throw SocketException( "::select error", __EXCEPTION_INFO__ );
  }
  
  return ( FD_ISSET( sock, &wmask ) != 0 );
}


_INLINE_
int Socket::send( const void *data, int size, const Address &to_addr ) throw () {
  if ( !isOpen() ) {
    return -1;
  }
  
  int ret = -1;
  
  if ( getType() != TCP ) {
    if ( to_addr != Address() ) {
      ret = ::sendto( sock, data, size, 0, 
		      (sockaddr*)&to_addr.asStorage(), to_addr.getAddressLength() );
      this->to_addr = to_addr;
    }
    else if ( this->to_addr != Address() ) {
      ret = ::sendto( sock, data, size, 0, 
		      (sockaddr*)&this->to_addr.asStorage(), this->to_addr.getAddressLength() );
    }
  }
  else {
    
#if defined(__APPLE__) && !defined(__FreeBSD__)
    ret = ::send( sock, data, size, 0 );
#else
    ret = ::send( sock, data, size, MSG_NOSIGNAL );
#endif
    
  }
  
  if ( ret < 0 ) {
    close();
  }
  return ret;
}


// _INLINE_
// bool Socket::sendByte( char c ) 
//   throw ( SocketException, SocketTimeoutException ) {
//   return ( this->send( &c, 1 ) == 1 );
// }


// _INLINE_
// bool Socket::sendString( const std::string &str ) 
//   throw ( SocketException, SocketTimeoutException ) {
//   uint64_t size = cutil::hton( (uint64_t)str.size() );
//   return sendAll( &size, sizeof( size ) ) && sendAll( str.c_str(), str.size() );
// }


_INLINE_
bool Socket::sendAll( const void *data, int size ) 
  throw ( SocketException ) {
  const unsigned char *buf = (const unsigned char*)data;
  int sentBytes = 0;
  
  int sockBufNum;
  sock_len_t len = sizeof( sockBufNum );
  
  if ( ::getsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sockBufNum, &len ) == -1 ) {
    throw SocketException( "::getsockopt SO_SNDBUF error", __EXCEPTION_INFO__ );
  }
  
  while ( true ) {
    int bufnum = ( size - sentBytes > sockBufNum )? sockBufNum: size - sentBytes;
    
    int tmpSentBytes = this->send( buf + sentBytes, bufnum );

    if ( tmpSentBytes < 0 ) { 
      return false;
    }
    
    sentBytes += tmpSentBytes;
    
    if ( size == sentBytes ) {
      break;
    }
  }
  
  return true;
}


// _INLINE_
// bool Socket::sendFile( const std::string &filename ) 
//   throw ( SocketException, SocketTimeoutException, std::ios_base::failure ) {
  
//   uint64_t filesize;
//   {
//     std::ifstream istr;
//     istr.exceptions( std::ios_base::failbit | std::ios_base::eofbit | std::ios::badbit );
//     istr.open( filename.c_str() );
//     istr.exceptions( std::ios::goodbit );
    
//     istr.seekg( 0, std::ios::end );
//     filesize = istr.tellg();

//     istr.close();
//   }
  
//   if ( filesize == 0 ) {
//     return false;
//   }
  
//   uint64_t filesizeToSend = cutil::hton( filesize );
//   if ( !this->sendAll( &filesizeToSend, sizeof( filesizeToSend ) ) ) {
//     return false;
//   }
  
//   std::ifstream istr( filename.c_str(), std::ios::binary );
//   unsigned int readBytes = 0;  
  
//   unsigned int sockBufNum;
//   sock_len_t len = sizeof( sockBufNum );
  
//   int rc = ::getsockopt( sock, SOL_SOCKET, SO_SNDBUF, &sockBufNum, &len );

//   if ( rc != 0 ) {
//     return false;
//   }
  
//   while ( true ) {
//     char buf[ sockBufNum ];
//     int bufnum = ( filesize - readBytes > sockBufNum )? sockBufNum: filesize - readBytes; 
    
//     istr.read( buf, bufnum );
//     int tmpReadBytes = istr.gcount();
//     readBytes += tmpReadBytes;
    
//     if ( !this->sendAll( buf, tmpReadBytes ) ) {
//       istr.close();
//       return false;
//     }
    
//     if ( readBytes == filesize ) {
//       break;
//     }
//   }
  
//   istr.close();
//   return true;
// }


_INLINE_
bool Socket::isReceivable( uint64_t timeout_usec ) 
  throw ( SocketException, SocketTimeoutException ) {
  if ( !isOpen() ) {
    return false;
  }
  if ( ( getType() == UDP ) && !isBound() ) {
    return false;
  }
  
  fd_set rmask;
  FD_ZERO( &rmask );
  FD_SET( sock, &rmask );
  
  timeval waitval;
  waitval.tv_sec = timeout_usec / 1000000;
  waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;
  
  int rc = ::select( sock + 1, &rmask, NULL, NULL, &waitval );

  if ( rc == 0 ) {
    throw SocketTimeoutException( "timeout ::select", __EXCEPTION_INFO__ );
  }
  else if ( rc < 0 ) {
    close();
    throw SocketException( "::select error", __EXCEPTION_INFO__ );
  }
  
  return ( FD_ISSET( sock, &rmask ) != 0 );
}


_INLINE_
int Socket::recv( void *data, int size, bool peek_flag ) throw () {
  if ( !isOpen() ) {
    return -1;
  }
  
  if ( size < 0 ) {
    return -1;
  }
  
  int msg_flag = ( peek_flag )? MSG_PEEK: 0;
  
  int ret;
  
  switch ( getType() ) {
  case TCP:
    ret = ::recv( sock, data, size, msg_flag );
    //    ret = ::recv( socket(), (char*)data, size, MSG_WAITALL );
    break;
  case UDP:
  case RAW:
    {
      socklen_t len = sizeof( this->from_addr.asStorage() );
      ret = ::recvfrom( sock, data, size, msg_flag, 
			(sockaddr*)&from_addr.asStorage(), &len );
    }
    break;
  }
  
  if ( size > 0 && ret <= 0 ) {
    close();
  }
  return ret;
}


// _INLINE_
// bool Socket::recvByte( char &c )
//   throw ( SocketException, SocketTimeoutException ) {
//   return ( this->recv( &c, 1 ) == 1 );
// }


// _INLINE_
// bool Socket::recvString( std::string &str )
//   throw ( SocketException, SocketTimeoutException ) {

//   uint64_t size;
//   if ( !this->recvAll( &size, sizeof( size ) ) ) {
//     return false;
//   }
//   size = cutil::ntoh( size );
  
//   char buf[ size + 1 ];
//   if ( !this->recvAll( buf, size ) ) {
//     return false;
//   }
//   buf[ size ] = '\0';
//   str = buf;
  
//   return true;
// }


_INLINE_
bool Socket::recvAll( void *data, int size )
  throw ( SocketException ) {

  unsigned char *buf = (unsigned char*)data;
  int recvBytes = 0;
  
  int sockBufNum;
  sock_len_t len = sizeof( sockBufNum );
  
  if ( ::getsockopt( sock, SOL_SOCKET, SO_RCVBUF, &sockBufNum, &len ) == -1 ) {
    throw SocketException( "::getsockopt SO_RCVBUF error", __EXCEPTION_INFO__ );
  }
  
  while ( true ) {
    int bufnum = ( size - recvBytes > sockBufNum )? sockBufNum: size - recvBytes;
    
    int tmpRecvBytes = this->recv( buf + recvBytes , bufnum );
    
    if ( tmpRecvBytes <= 0 ) {
      return false;
    }
    
    recvBytes += tmpRecvBytes;
    
    if ( recvBytes == size ) {
      break;
    }
  }
  return true;
}


// _INLINE_
// bool Socket::recvFile( const std::string &filename )
//   throw ( SocketException, SocketTimeoutException, std::ios_base::failure ) {
  
//   uint64_t filesize;
//   if ( !this->recvAll( &filesize, sizeof( filesize ) ) ) {
//     return false;
//   }
//   filesize = cutil::ntoh( filesize );
  
//   if ( filesize == 0 ) {
//     return false;
//   }
  
//   std::ofstream ostr;
//   ostr.exceptions( std::ios_base::failbit | std::ios_base::eofbit | std::ios::badbit );
//   ostr.open( filename.c_str(), std::ios::binary );
//   ostr.exceptions( std::ios::goodbit );

//   unsigned int recvBytes = 0;

//   unsigned int sockBufNum;
//   sock_len_t len = sizeof( sockBufNum );
  
//   int rc = ::getsockopt( sock, SOL_SOCKET, SO_RCVBUF, &sockBufNum, &len );
  
//   if ( rc != 0 ) {
//     return false;
//   }

//   while ( true ) {
//     char buf[ sockBufNum ];
//     unsigned int bufnum = ( filesize - recvBytes > sockBufNum )? sockBufNum: filesize - recvBytes; 
    
//     if ( !this->recvAll( buf, bufnum ) ) {
//       ostr.close();
//       return false;
//     }
//     recvBytes += bufnum;

//     ostr.write( buf, bufnum );
//     if ( recvBytes == filesize ) {
//       break;
//     }
//   }

//   ostr.close(); 
//   return true;
// }




_INLINE_
int Socket::byteInSendQueue() 
  throw ( SocketException ) {
  if ( !isOpen() ) {
    return 0;
  }
  
  int value;

#if defined(__APPLE__)
  sock_len_t len = sizeof( value );
  int rc = ::getsockopt( sock, SOL_SOCKET, SO_NWRITE, &value, &len );
#else
  int rc = ::ioctl( sock, TIOCOUTQ, &value ); // SIOCOUTQ
#endif
  
  if ( rc != 0 ) {
    close();
    throw SocketException( "::getsockopt/::ioctl error", __EXCEPTION_INFO__ );
  }
  
  return value;
}



_INLINE_
int Socket::byteInRecvQueue() 
  throw ( SocketException ) {
  if ( !isOpen() ) {
    return 0;
  }
  
  int value;
  
#if defined(__APPLE__)
  sock_len_t len = sizeof( value );
  int rc = ::getsockopt( sock, SOL_SOCKET, SO_NREAD, &value, &len );
#else
  int rc = ::ioctl( sock, FIONREAD, &value ); // SIOCINQ
#endif
  
  if ( rc != 0 ) {
    close();
    throw SocketException( "::getsockopt/::ioctl error", __EXCEPTION_INFO__ );
  }
  
  return value;
}

}
