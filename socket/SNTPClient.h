/**
 * $Id: SNTPClient.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_SNTPCLIENT_H_
#define _CUTIL_SNTPCLIENT_H_

#include "Socket.h"

namespace cutil {

class SNTPClient {
 public:
  static timeval getClock(const Address &ntpserver, int iteration = 20,
                          uint64_t timeout_usec = 0);
  static bool setClock(const Address &ntpserver, int iteration = 20,
                       uint64_t timeout_usec = 0);
};

}  // namespace cutil

#include "SNTPClient.ipp"

#endif
