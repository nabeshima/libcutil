/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_SNTPCLIENT_H_
#define _CUTIL_SNTPCLIENT_H_

#include "Socket.h"

namespace cutil {

class SNTPClient {
public:
  static timeval getClock( const Address &ntpserver, int iteration = 20, uint64_t timeout_usec = 0 )
    throw ( SocketException, SocketTimeoutException );
  static bool setClock( const Address &ntpserver, int iteration = 20, uint64_t timeout_usec = 0 )
    throw ( SocketException, SocketTimeoutException );
};

}

// #ifndef _NO_INLINE_
#define _INLINE_ inline
#include "SNTPClient.ipp"
// #endif

#endif
