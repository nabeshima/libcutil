/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_SOCKET_H_
#define _CUTIL_SOCKET_H_


#include "_socket_config.h"
#include "Address.h"
#include "SocketException.h"


namespace cutil {


/*!
  BSD socket wrapper
*/
class Socket {
private:
  int *ref_counter;    //! reference counter for delayed close()
  socket_t sock;       //! socket handler
  INFamily family;
  SocketType sock_type;  //! RAW, UDP, TCP
  
  Address local_addr;    //! local address to bind
  Address to_addr;       //! opponent's address to which this socket connects and send data
  Address from_addr;     //! opponent's address which connects to or send data to this 
  
public:
  Socket() throw ();
  Socket( const Socket &s ) throw ();
  ~Socket() throw ();
  
  Socket& operator=( const Socket &s ) throw ();
  
  INFamily getFamily() const throw ();
  SocketType getType() const throw ();
  
  //! allow user to handle the socket out of this class.
  socket_t socket() const throw ();
  
  
  bool open( INFamily family, SocketType sock_type ) throw ();
  
  bool close() throw ();

  bool isOpen() const throw ();
    
  //! TCP: connect to opponent; UDP: addres to send is set
  bool connect( const Address &to_addr, uint64_t timeout_usec = 0 )
    throw ( SocketException, 
	    SocketTimeoutException, 
	    SocketFamilyUnmatchException );
  
  //! make the socket to send/recv broadcast packet
  bool setBroadcast( bool enable ) 
    throw ( SocketException,
	    SocketTypeUnmatchException );
  
  //! join multicast group to recv multicast packet and to specify interface to send multicast packet. return false if group_addr is not multicast addres;
  bool joinMulticastGroup( const Address &group_addr )
    throw ( SocketException,
	    SocketFamilyUnmatchException, 
	    SocketTypeUnmatchException );
  
  bool leaveMulticastGroup( const Address &group_addr ) 
    throw ( SocketException,
	    SocketFamilyUnmatchException, 
	    SocketTypeUnmatchException );
  
  //! set Address to TCP accept / set Address to UDP recv. If port == 0, random port is bound.
  bool bind( uint16_t port = 0, int backlog = 5 )
    throw ( SocketException );
  
  bool isBound() const throw ();
  uint16_t boundPort() const throw ();
  
  const Address& localAddress() const throw ();
  const Address& toAddress() const throw ();
  const Address& fromAddress() const throw ();
  
  // For TCP
  bool isAcceptable( uint64_t timeout_usec = 0 )   //! return true if TCP is bound and a connection is requested from client
    throw ( SocketException, SocketTimeoutException, SocketTypeUnmatchException );
  Socket accept()         //! accept connection from TCP client. blocking
    throw ( SocketException, SocketTypeUnmatchException );
  
  bool isSendable( uint64_t timeout_usec = 0 )
    throw ( SocketException, SocketTimeoutException );
  int send( const void *data, int size, const Address& to_addr = Address() ) throw ();  //! return sent byte
  //   bool sendByte( char c )
  //     throw ( SocketException, SocketTimeoutException );
  //   bool sendString( const std::string &str )
  //     throw ( SocketException, SocketTimeoutException );
  bool sendAll( const void *data, int size ) 
    throw ( SocketException );
  //   bool sendFile( const std::string &filename ) 
  //     throw ( SocketException, SocketTimeoutException, std::ios_base::failure );
  
  bool isReceivable( uint64_t timeout_usec = 0 )
    throw ( SocketException, SocketTimeoutException );
  int recv( void *data, int size, bool peek_flag = false ) throw (); //! return recv byte
  //   bool recvByte( char &c )
  //     throw ( SocketException, SocketTimeoutException );
  //   bool recvString( std::string &str )
  //     throw ( SocketException, SocketTimeoutException );
  bool recvAll( void *data, int size ) 
    throw ( SocketException );
  //   bool recvFile( const std::string &filename )
  //     throw ( SocketException, SocketTimeoutException, std::ios_base::failure );
  
  
  // For monitoring of network interface status.
  int byteInSendQueue()        //! returns byte in queue (different from specified by send())
    throw ( SocketException );
  int byteInRecvQueue()
    throw ( SocketException );
};


}

// #ifndef _NO_INLINE_
#define _INLINE_ inline
#include "Socket.ipp"
// #endif

#endif

