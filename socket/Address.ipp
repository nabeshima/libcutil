/**
 * $Id: Address.ipp 5 2013-09-25 08:42:57Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <memory.h>
}

namespace cutil {

inline Address::Address() { clear(); }

inline Address::Address(const Address &address) { setAddress(address); }

inline Address::Address(const in_addr &iaddr, uint16_t port) {
  setAddress(iaddr, port);
}

inline Address::Address(const in6_addr &iaddr6, uint16_t port) {
  setAddress(iaddr6, port);
}

inline Address::Address(const std::string &socket_file_local_path) {
  setAddress(socket_file_local_path);
}

inline Address &Address::operator=(const Address &address) {
  setAddress(address);
  return *this;
}

inline void Address::clear() {
  ::memset(&storage, 0, sizeof(storage));
  storage.ss_family = IPv4;
}

inline void Address::setAddress(const Address &address) {
  storage = address.storage;
}

inline void Address::setAddress(const in_addr &iaddr, uint16_t port) {
  clear();
  sockaddr_in &saddr = asIPv4();
#if defined(__APPLE__) && !defined(__FreeBSD__)
  saddr.sin_len = sizeof(sockaddr_in);
#endif
  saddr.sin_family = IPv4;
  saddr.sin_addr = iaddr;
  saddr.sin_port = htons(port);
}

inline void Address::setAddress(const in6_addr &iaddr6, uint16_t port) {
  clear();
  sockaddr_in6 &saddr = asIPv6();
#if defined(__APPLE__) && !defined(__FreeBSD__)
  saddr.sin6_len = sizeof(sockaddr_in6);
#endif
  saddr.sin6_family = IPv6;
  saddr.sin6_addr = iaddr6;
  saddr.sin6_port = htons(port);
}

inline void Address::setAddress(const std::string &socket_file_local_path) {
  clear();
  sockaddr_un &saddr = asLocal();
  saddr.sun_family = Local;

  size_t len = socket_file_local_path.size();
  if (103 < len) {
    len = 103;
  }
  ::strncpy(saddr.sun_path, socket_file_local_path.c_str(), len);
  saddr.sun_path[len] = '\0';
#if defined(__APPLE__) && !defined(__FreeBSD__)
  saddr.sun_len = len + 1;
#endif
}

inline INFamily Address::getFamily() const {
  return (INFamily)storage.ss_family;
}

// to support no ss_len environment
inline int Address::getAddressLength() const {
  switch (getFamily()) {
    case IPv4:
      return sizeof(sockaddr_in);
    case IPv6:
      return sizeof(sockaddr_in6);
    case Local:
      return sizeof(sockaddr_un);
    default:
      return 0;
  }
}

inline bool Address::isIPv4() const { return (getFamily() == IPv4); }

inline bool Address::isIPv6() const { return (getFamily() == IPv6); }

inline bool Address::isLocal() const { return (getFamily() == Local); }

inline sockaddr_in &Address::asIPv4() { return *(sockaddr_in *)&storage; }

inline sockaddr_in6 &Address::asIPv6() { return *(sockaddr_in6 *)&storage; }

inline sockaddr_un &Address::asLocal() { return *(sockaddr_un *)&storage; }

inline sockaddr_storage &Address::asStorage() { return storage; }

inline const sockaddr_in &Address::asIPv4() const {
  return *(sockaddr_in *)&storage;
}

inline const sockaddr_in6 &Address::asIPv6() const {
  return *(sockaddr_in6 *)&storage;
}

inline const sockaddr_un &Address::asLocal() const {
  return *(sockaddr_un *)&storage;
}

inline const sockaddr_storage &Address::asStorage() const { return storage; }

inline void Address::setPort(uint16_t port) {
  switch (getFamily()) {
    case IPv4:
      asIPv4().sin_port = htons(port);
      break;
    case IPv6:
      asIPv6().sin6_port = htons(port);
      break;
    default:
      break;
  }
}

inline uint16_t Address::getPort() const {
  switch (getFamily()) {
    case IPv4:
      return ntohs(asIPv4().sin_port);
    case IPv6:
      return ntohs(asIPv6().sin6_port);
    default:
      return 0;
  }
}

inline std::string Address::toString() const {
  char strbuf[128];

  switch (getFamily()) {
    case IPv4:
      ::inet_ntop(IPv4, &asIPv4().sin_addr, strbuf, sizeof(strbuf));
      break;
    case IPv6:
      ::inet_ntop(IPv6, &asIPv6().sin6_addr, strbuf, sizeof(strbuf));
      break;
    case Local:
      ::strncpy(strbuf, asLocal().sun_path, sizeof(strbuf));
  }

  return std::string(strbuf);
}

inline bool Address::isMulticast() const {
  return (isIPv4() && ((asIPv4().sin_addr.s_addr & 0xf0) ==
                       0xe0)) ||  // little endian, MSB is 0xe0 or not.
         (isIPv6() && (asIPv6().sin6_addr.s6_addr[0] == 0xff));  // MSB is 0xff
}

inline bool Address::operator==(const Address &address) const {
  if (getFamily() != address.getFamily()) {
    return false;
  }

  switch (getFamily()) {
    case IPv4:
      return ::memcmp(&asIPv4().sin_addr, &address.asIPv4().sin_addr,
                      sizeof(in_addr)) == 0 &&
             asIPv4().sin_port == address.asIPv4().sin_port;
    case IPv6:
      return ::memcmp(&asIPv6().sin6_addr, &address.asIPv6().sin6_addr,
                      sizeof(in6_addr)) == 0 &&
             asIPv6().sin6_port == address.asIPv6().sin6_port;
    default:
      return false;
  }
}

inline bool Address::operator!=(const Address &address) const {
  return !(*this == address);
}

template <template <typename, typename> class Sequence, typename Allocator>
inline bool Address::resolve(Sequence<Address, Allocator> *result,
                             const std::string &host, bool numeric_host) {
  result->clear();

  {
    res_state resp = &_res;
    resp->retrans = 1;  //! DNS request interval seconds >= 1
    resp->retry = 1;    //! DNS request trials >= 1
  }

  addrinfo *res = NULL;
  addrinfo hints;
  ::memset(&hints, 0, sizeof(hints));

  hints.ai_family = AF_UNSPEC;
  //  hints.ai_protocol = TCP;
  //  hints.ai_flags = AI_NUMERICSERV;  // comment out to avoid Mac OS X bug
  if (numeric_host) {
    hints.ai_flags |= AI_NUMERICHOST;
  }

  int err = ::getaddrinfo(host.c_str(),
                          "0",  // port is not assigned
                          &hints,
                          &res);  // any NIC and any protocol

  if (err != 0) {
    return false;
  }

  for (addrinfo *i = res; i != NULL; i = i->ai_next) {
    Address tmp;

    switch (i->ai_family) {
      case IPv4:
        tmp.setAddress(((sockaddr_in *)i->ai_addr)->sin_addr, 0);
        break;
      case IPv6:
        tmp.setAddress(((sockaddr_in6 *)i->ai_addr)->sin6_addr, 0);
        break;
    }

    if (result->size() == 0 || tmp != result->back()) {
      result->push_back(tmp);
    }
  }

  if (res != NULL) {
    ::freeaddrinfo(res);
  }

  return (result->size() > 0);
}
}
