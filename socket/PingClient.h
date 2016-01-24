/**
 * $Id: PingClient.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_PINGCLIENT_H_
#define _CUTIL_PINGCLIENT_H_

#include "Socket.h"

namespace cutil {

class PingClient {
 public:
  static uint16_t checksum(uint16_t *buf, int size);
  static bool ping(const Address &peer, uint64_t timeout_usec = 0);
};

}  // namespace cutil

#include "PingClient.ipp"

#endif
