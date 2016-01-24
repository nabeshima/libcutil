/**
 * $Id: Socket.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_SOCKET_H_
#define _CUTIL_SOCKET_H_

#include "./_socket_config.h"
#include "Address.h"

namespace cutil {

/*!
  BSD socket wrapper
*/
class Socket {
 private:
  int *ref_counter;  //! reference counter for delayed close()
  socket_t sock;     //! socket handler
  INFamily family;
  SocketType sock_type;  //! RAW, UDP, TCP

  Address local_addr;  //! local address to bind
  Address to_addr;     //! opponent's address to which this socket connects and
                       //! send data
  Address
      from_addr;  //! opponent's address which connects to or send data to this

 public:
  Socket();
  Socket(const Socket &s);
  ~Socket();

  Socket &operator=(const Socket &s);

  INFamily getFamily() const;
  SocketType getType() const;

  //! allow user to handle the socket out of this class.
  socket_t socket() const;

  bool open(INFamily family, SocketType sock_type);

  bool close();

  bool isOpen() const;

  //! TCP: connect to opponent; UDP: address to send is set
  //! no blocking nor no waiting when timeout_usec == 0.
  bool connect(const Address &to_addr, uint64_t timeout_usec = 0);

  //! make the socket to send/recv broadcast packet
  bool setBroadcast(bool enable);

  //! join multicast group to recv multicast packet and to specify interface to
  //! send multicast packet. return false if group_addr is not multicast addres;
  bool joinMulticastGroup(const Address &group_addr);

  bool leaveMulticastGroup(const Address &group_addr);

  //! Only for IPv4 and IPv6. Set Address to TCP accept / set Address to UDP
  //! recv. If port == 0, random port is bound.
  bool bind(uint16_t port = 0, int backlog = 5);

  bool isBound() const;
  uint16_t boundPort() const;

  const Address &localAddress() const;
  const Address &toAddress() const;
  const Address &fromAddress() const;

  // For TCP
  //! no blocking nor no waiting when timeout_usec == 0.
  bool isAcceptable(uint64_t timeout_usec = 0);  //! return true if TCP is bound
                                                 //! and a connection is
                                                 //! requested from client

  Socket accept();  //! accept connection from TCP client. blocking

  //! no blocking nor no waiting when timeout_usec == 0.
  bool isSendable(uint64_t timeout_usec = 0);
  int send(const void *data, int size,
           const Address &to_addr = Address());  //! return sent byte
  bool sendAll(const void *data, int size);

  //! no blocking nor no waiting when timeout_usec == 0.
  bool isReceivable(uint64_t timeout_usec = 0);
  int recv(void *data, int size, bool peek_flag = false);  //! return recv byte
  bool recvAll(void *data, int size);

  // For monitoring of network interface status.
  int byteInSendQueue();  //! returns byte in queue (different from specified by
                          //! send())

  int byteInRecvQueue();
};
}  // namespace cutil

#include "Socket.ipp"

#endif
