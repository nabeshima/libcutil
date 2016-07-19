/**
 * $Id: Socket.ipp 8 2013-09-30 09:17:20Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cutil {

inline Socket::Socket()
    : ref_counter(new int), sock(-1), family(IPv4), sock_type(UDP) {
  (*ref_counter) = 1;
}

inline Socket::Socket(const Socket &s)
    : ref_counter(s.ref_counter),
      sock(s.sock),
      family(s.family),
      sock_type(s.sock_type),
      local_addr(s.local_addr),
      to_addr(s.to_addr),
      from_addr(s.from_addr) {
  ++(*ref_counter);
}

inline Socket::~Socket() {
  --(*ref_counter);

  if ((*ref_counter) == 0) {
    close();
    delete ref_counter;
  }
}

inline Socket &Socket::operator=(const Socket &s) {
  --(*ref_counter);

  if ((*ref_counter) == 0) {
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

inline INFamily Socket::getFamily() const { return family; }

inline SocketType Socket::getType() const { return sock_type; }

inline socket_t Socket::socket() const { return sock; }

inline bool Socket::open(INFamily family, SocketType sock_type) {
  if (isOpen()) {
    return false;
  }

  // socket open
  switch (sock_type) {
    case TCP:
    case UDP:
      sock = ::socket(family, sock_type, 0);
      break;
    case RAW:
      sock = ::socket(family, sock_type, IPPROTO_ICMP);
      break;
  }

  this->family = family;
  this->sock_type = sock_type;

  return isOpen();
}

inline bool Socket::close() {
  if (!isOpen()) {
    return false;
  }

#ifdef CUTIL_WIN32
  ::closesocket(sock);
#else
  ::close(sock);
#endif

  sock = -1;
  local_addr.clear();
  to_addr.clear();
  from_addr.clear();

  return true;
}

inline bool Socket::isOpen() const { return (sock != -1); }

inline bool Socket::connect(const Address &to_addr, uint64_t timeout_usec) {
  if (!isOpen()) {
    return false;
  }

  if (getFamily() != to_addr.getFamily()) {
    return false;
  }

  if (getType() != TCP) {
    this->to_addr = to_addr;
    return true;
  }

  if (timeout_usec == 0) {  // blocking
    if (::connect(sock, (const sockaddr *)&to_addr.asStorage(),
                  to_addr.getAddressLength()) < 0) {
      close();
      return false;
    }
  } else {  // non-blocking

    //! make socket non-blocking
    int flag = ::fcntl(sock, F_GETFL, NULL);
    if (flag < 0) {
      close();
      return false;
    }

    if (::fcntl(sock, F_SETFL, flag | O_NONBLOCK) < 0) {
      close();
      return false;
    }

#if defined(__APPLE__) && !defined(__FreeBSD__)
    const int one = 1;
    if (::setsockopt(sock, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one)) == -1) {
      return false;
    }
#endif

    if (::connect(sock, (const sockaddr *)&to_addr.asStorage(),
                  to_addr.getAddressLength()) < 0) {
      if (errno != EINPROGRESS) {
        close();

        if (::fcntl(sock, F_SETFL, flag) < 0) {
          return false;
        } else {
          return false;
        }
      }

      fd_set wmask;
      FD_ZERO(&wmask);
      FD_SET(sock, &wmask);

      timeval waitval;
      waitval.tv_sec = timeout_usec / 1000000;
      waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;

      int rc = ::select(sock + 1, NULL, &wmask, NULL, &waitval);

      if (rc == 0) {
        if (::fcntl(sock, F_SETFL, flag) < 0) {
          return false;
        }
        return false;
      } else if (rc < 0) {
        if (::fcntl(sock, F_SETFL, flag) < 0) {
          close();
          return false;
        }
        close();
        return false;
      }
    }

    //! restore socket flag
    if (::fcntl(sock, F_SETFL, flag) < 0) {
      close();
      return false;
    }
  }

  // check actually connected server address.
  socklen_t len = sizeof(this->to_addr.asStorage());
  if (::getpeername(sock, (sockaddr *)&this->to_addr.asStorage(), &len) == -1) {
    close();
    return false;
  }

  return true;
}

inline bool Socket::setBroadcast(bool enable) {
  if (!isOpen()) {
    return false;
  }

  if (getType() == TCP) {
    return false;
  }

  const int one = (enable) ? 1 : 0;
  if (::setsockopt(sock, SOL_SOCKET, SO_BROADCAST, &one, sizeof(one)) == -1) {
    return false;
  }

  return true;
}

inline bool Socket::joinMulticastGroup(const Address &group_addr) {
  if (!isOpen() || !group_addr.isMulticast()) {
    return false;
  }

  if (getFamily() != group_addr.getFamily()) {
    return false;
  }

  if (getType() != UDP) {
    return false;
  }

  switch (getFamily()) {
    case IPv4: {
      const sockaddr_in &saddr = group_addr.asIPv4();

      //! prevent loopback
      //       const int zero = 0;
      //       ::setsockopt( sock, IPPROTO_IP, IP_MULTICAST_LOOP, &zero, sizeof(
      //       zero ) );

      //! choose interface
      in_addr iaddr;
      iaddr.s_addr = INADDR_ANY;
      if (::setsockopt(sock, IPPROTO_IP, IP_MULTICAST_IF, &iaddr,
                       sizeof(iaddr)) == -1) {
        return false;
      }

      //! set TTL
      uint8_t lTTL = 2;
      if (::setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, &lTTL,
                       sizeof(lTTL)) == -1) {
        return false;
      }

      //! join multicast group
      ip_mreq mreq;
      mreq.imr_interface.s_addr = INADDR_ANY;
      mreq.imr_multiaddr = saddr.sin_addr;
      if (::setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq,
                       sizeof(mreq)) == -1) {
        return false;
      }

      return true;
    }
    case IPv6: {
      const sockaddr_in6 &saddr6 = group_addr.asIPv6();

      ipv6_mreq mreq6;
      mreq6.ipv6mr_interface = 1;  // if_nametoindex( "eth0" ) is applicable if
                                   // needed. 0 causes error.
      mreq6.ipv6mr_multiaddr = saddr6.sin6_addr;

      //! choose interface
      if (::setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_IF,
                       &mreq6.ipv6mr_interface,
                       sizeof(mreq6.ipv6mr_interface)) == -1) {
        return false;
      }

      //! set HOPS
      uint32_t lhops = 2;
      if (::setsockopt(sock, IPPROTO_IPV6, IPV6_MULTICAST_HOPS, &lhops,
                       sizeof(lhops)) == -1) {
        return false;
      }

      //! join multicast group
      if (::setsockopt(sock, IPPROTO_IPV6, IPV6_JOIN_GROUP, &mreq6,
                       sizeof(mreq6)) == -1) {
        return false;
      }

      return true;
    }
    default:
      return false;
  }
}

inline bool Socket::leaveMulticastGroup(const Address &group_addr) {
  if (!isOpen() || !group_addr.isMulticast()) {
    return false;
  }

  if (getFamily() != group_addr.getFamily()) {
    return false;
  }

  if (getType() != UDP) {
    return false;
  }

  switch (getFamily()) {
    case IPv4: {
      const sockaddr_in &saddr = group_addr.asIPv4();
      ip_mreq mreq;
      mreq.imr_interface.s_addr = INADDR_ANY;
      mreq.imr_multiaddr = saddr.sin_addr;
      if (::setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, &mreq,
                       sizeof(mreq)) == -1) {
        return false;
      }

      return true;
    }
    case IPv6: {
      const sockaddr_in6 &saddr6 = group_addr.asIPv6();

      ipv6_mreq mreq6;
      mreq6.ipv6mr_interface = 1;  // default interface. if_nametoindex( "eth0"
                                   // ) is applicable if needed.
      mreq6.ipv6mr_multiaddr = saddr6.sin6_addr;
      if (::setsockopt(sock, IPPROTO_IPV6, IPV6_LEAVE_GROUP, &mreq6,
                       sizeof(mreq6)) == -1) {
        return false;
      }

      return true;
    }
    default:
      return false;
  }
}

inline bool Socket::bind(uint16_t port, int backlog) {
  if (!isOpen() || isBound()) {
    return false;
  }

  // port reusing
  const int one = 1;
  if (::setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &one, sizeof(one)) == -1) {
    return false;
  }

  // prepare address.
  sockaddr_in locv4;
  sockaddr_in6 locv6;

  sockaddr *loc;
  socklen_t len;

  if (getFamily() == IPv4) {
    locv4.sin_addr.s_addr = INADDR_ANY;
    locv4.sin_port = cutil::hton(port);
    len = sizeof(locv4);
    loc = (sockaddr *)&locv4;
  } else {
    ::memset(locv6.sin6_addr.s6_addr, 0, sizeof(locv6.sin6_addr.s6_addr));
    locv6.sin6_port = cutil::hton(port);
    len = sizeof(locv6);
    loc = (sockaddr *)&locv6;
  }
  loc->sa_family = getFamily();

  if (::bind(sock, loc, len) == -1) {
    close();
    return false;
  }

  // check actually assigned port (for port 0).
  len = sizeof(this->to_addr.asStorage());
  if (::getsockname(sock, (sockaddr *)&this->local_addr.asStorage(), &len) ==
      -1) {
    close();
    return false;
  }

  if (getType() == TCP && ::listen(sock, backlog) == -1) {
    return false;
  }
  return true;
}

inline bool Socket::bind(const Address &local_socket_path, int backlog) {
  if (!isOpen() || isBound() || getFamily() != Local ||
      local_socket_path.getFamily() != Local) {
    return false;
  }

  // local socket path reusing
  unlink(local_socket_path.toString().c_str());

  sockaddr_un loc = local_socket_path.asLocal();
  socklen_t len = sizeof(sockaddr_un);

  if (::bind(sock, (sockaddr *)&loc, len) == -1) {
    close();
    return false;
  }

  local_addr = local_socket_path;

  if (getType() == TCP && ::listen(sock, backlog) == -1) {
    return false;
  }
  return true;
}

inline bool Socket::isBound() const { return (local_addr != Address()); }

inline uint16_t Socket::boundPort() const { return local_addr.getPort(); }

inline const Address &Socket::localAddress() const { return local_addr; }

inline const Address &Socket::toAddress() const { return to_addr; }

inline const Address &Socket::fromAddress() const { return from_addr; }

inline bool Socket::isAcceptable(uint64_t timeout_usec) {
  if (!isBound()) {
    return false;
  }

  if (getType() != TCP) {
    return false;
  }

  fd_set rmask;
  FD_ZERO(&rmask);
  FD_SET(sock, &rmask);

  timeval waitval;
  waitval.tv_sec = timeout_usec / 1000000;
  waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;

  int rc = ::select(sock + 1, &rmask, NULL, NULL, &waitval);

  if (rc == 0) {
    return false;
  } else if (rc < 0) {
    close();
    return false;
  }

  return (FD_ISSET(sock, &rmask) != 0);
}

inline Socket Socket::accept() {
  if (!isBound()) {
    return Socket();
  }

  if (getType() != TCP) {
    return Socket();
  }

  sockaddr_storage saddrs;
  socklen_t len = sizeof(saddrs);

  Socket ret;
  ret.sock = ::accept(sock, (sockaddr *)&ret.from_addr.asStorage(), &len);

  // to_addr is the same as from_addr because of connection.
  ret.to_addr = ret.from_addr;

  ret.family = Address(ret.to_addr).getFamily();
  ret.sock_type = TCP;

  const int one = 1;
  if (::setsockopt(ret.sock, SOL_SOCKET, SO_KEEPALIVE, &one, sizeof(one)) <
      -1) {
    return Socket();
  }

#if defined(__APPLE__) && !defined(__FreeBSD__)
  if (::setsockopt(ret.sock, SOL_SOCKET, SO_NOSIGPIPE, &one, sizeof(one)) <
      -1) {
    return Socket();
  }
#endif

  return ret;
}

inline bool Socket::isSendable(uint64_t timeout_usec) {
  if (!isOpen()) {
    return false;
  }

  fd_set wmask;
  FD_ZERO(&wmask);
  FD_SET(sock, &wmask);

  timeval waitval;
  waitval.tv_sec = timeout_usec / 1000000;
  waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;

  int rc = ::select(sock + 1, NULL, &wmask, NULL, &waitval);

  if (rc == 0) {
    return false;
  } else if (rc < 0) {
    close();
    return false;
  }

  return (FD_ISSET(sock, &wmask) != 0);
}

inline int Socket::send(const void *data, int size, const Address &to_addr) {
  if (!isOpen()) {
    return -1;
  }

  int ret = -1;

  if (getType() != TCP) {
    if (to_addr != Address()) {
      ret = ::sendto(sock, data, size, 0, (sockaddr *)&to_addr.asStorage(),
                     to_addr.getAddressLength());
      this->to_addr = to_addr;
    } else if (this->to_addr != Address()) {
      ret =
          ::sendto(sock, data, size, 0, (sockaddr *)&this->to_addr.asStorage(),
                   this->to_addr.getAddressLength());
    }
  } else {
#if defined(__APPLE__) && !defined(__FreeBSD__)
    ret = ::send(sock, data, size, 0);
#else
    ret = ::send(sock, data, size, MSG_NOSIGNAL);
#endif
  }

  if (ret < 0) {
    close();
  }
  return ret;
}

inline bool Socket::sendAll(const void *data, int size) {
  const unsigned char *buf = (const unsigned char *)data;
  int sentBytes = 0;

  int sockBufNum;
  sock_len_t len = sizeof(sockBufNum);

  if (::getsockopt(sock, SOL_SOCKET, SO_SNDBUF, &sockBufNum, &len) == -1) {
    return false;
  }

  while (true) {
    int bufnum =
        (size - sentBytes > sockBufNum) ? sockBufNum : size - sentBytes;

    int tmpSentBytes = this->send(buf + sentBytes, bufnum);

    if (tmpSentBytes < 0) {
      return false;
    }

    sentBytes += tmpSentBytes;

    if (size == sentBytes) {
      break;
    }
  }

  return true;
}

inline bool Socket::isReceivable(uint64_t timeout_usec) {
  if (!isOpen()) {
    return false;
  }
  if ((getType() == UDP) && !isBound()) {
    return false;
  }

  fd_set rmask;
  FD_ZERO(&rmask);
  FD_SET(sock, &rmask);

  timeval waitval;
  waitval.tv_sec = timeout_usec / 1000000;
  waitval.tv_usec = timeout_usec - waitval.tv_sec * 1000000;

  int rc = ::select(sock + 1, &rmask, NULL, NULL, &waitval);

  if (rc == 0) {
    return false;
  } else if (rc < 0) {
    close();
    return false;
  }

  return (FD_ISSET(sock, &rmask) != 0);
}

inline int Socket::recv(void *data, int size, bool peek_flag) {
  if (!isOpen()) {
    return -1;
  }

  if (size < 0) {
    return -1;
  }

  int msg_flag = (peek_flag) ? MSG_PEEK : 0;

  int ret;

  switch (getType()) {
    case TCP:
      ret = ::recv(sock, data, size, msg_flag);
      //    ret = ::recv( socket(), (char*)data, size, MSG_WAITALL );
      break;
    case UDP:
    case RAW: {
      socklen_t len = sizeof(from_addr.asStorage());
      ret = ::recvfrom(sock, data, size, msg_flag,
                       (sockaddr *)&from_addr.asStorage(), &len);
    } break;
  }

  if (size > 0 && ret <= 0) {
    close();
  }
  return ret;
}

inline bool Socket::recvAll(void *data, int size) {
  unsigned char *buf = (unsigned char *)data;
  int recvBytes = 0;

  int sockBufNum;
  sock_len_t len = sizeof(sockBufNum);

  if (::getsockopt(sock, SOL_SOCKET, SO_RCVBUF, &sockBufNum, &len) == -1) {
    return false;
  }

  while (true) {
    int bufnum =
        (size - recvBytes > sockBufNum) ? sockBufNum : size - recvBytes;

    int tmpRecvBytes = this->recv(buf + recvBytes, bufnum);

    if (tmpRecvBytes <= 0) {
      return false;
    }

    recvBytes += tmpRecvBytes;

    if (recvBytes == size) {
      break;
    }
  }
  return true;
}

inline int Socket::byteInSendQueue() {
  if (!isOpen()) {
    return 0;
  }

  int value;

#if defined(__APPLE__)
  sock_len_t len = sizeof(value);
  int rc = ::getsockopt(sock, SOL_SOCKET, SO_NWRITE, &value, &len);
#else
  int rc = ::ioctl(sock, TIOCOUTQ, &value);  // SIOCOUTQ
#endif

  if (rc != 0) {
    close();
    return 0;
  }
  return value;
}

inline int Socket::byteInRecvQueue() {
  if (!isOpen()) {
    return 0;
  }

  int value;

#if defined(__APPLE__)
  sock_len_t len = sizeof(value);
  int rc = ::getsockopt(sock, SOL_SOCKET, SO_NREAD, &value, &len);
#else
  int rc = ::ioctl(sock, FIONREAD, &value);  // SIOCINQ
#endif

  if (rc != 0) {
    close();
    return 0;
  }

  return value;
}
}
