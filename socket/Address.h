/**
 * $Id: Address.h 5 2013-09-25 08:42:57Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_ADDRESS_H_
#define _CUTIL_ADDRESS_H_

#include <string>

#include "./_socket_config.h"

namespace cutil {

/*!
  IP address and port.
*/
class Address {
 private:
  sockaddr_storage storage;

 public:
  Address();
  Address(const Address &address);
  Address(const in_addr &iaddr, uint16_t port);
  Address(const in6_addr &iaddr6, uint16_t port);
  explicit Address(const std::string &socket_file_local_path);

  Address &operator=(const Address &address);

  void clear();

  void setAddress(const Address &address);
  void setAddress(const in_addr &iaddr, uint16_t port);
  void setAddress(const in6_addr &iaddr6, uint16_t port);
  void setAddress(const std::string &socket_file_local_path);

  INFamily getFamily() const;
  int getAddressLength() const;

  bool isIPv4() const;
  bool isIPv6() const;
  bool isLocal() const;

  sockaddr_in &asIPv4();
  sockaddr_in6 &asIPv6();
  sockaddr_un &asLocal();
  sockaddr_storage &asStorage();
  const sockaddr_in &asIPv4() const;
  const sockaddr_in6 &asIPv6() const;
  const sockaddr_un &asLocal() const;
  const sockaddr_storage &asStorage() const;

  void setPort(uint16_t port);
  uint16_t getPort() const;

  std::string toString() const;

  bool isMulticast() const;

  bool operator==(const Address &address) const;
  bool operator!=(const Address &address) const;

  // resolve hostname
  template <template <typename, typename> class Sequence, typename Allocator>
  static bool resolve(Sequence<Address, Allocator> *result,
                      const std::string &host, bool numeric_host = true);
};
}  // namespace cutil

#include "Address.ipp"

#endif
