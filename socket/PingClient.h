/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_PINGCLIENT_H_
#define _CUTIL_PINGCLIENT_H_

#include "Socket.h"


namespace cutil {

class PingClient {
public:
  static unsigned short checksum( unsigned short *buf, int size ) throw ();
  static bool ping( const Address &peer, uint64_t timeout_usec = 0 )
    throw ( SocketException, SocketTimeoutException );
};

}

// #ifndef _NO_INLINE_
#define _INLINE_ inline
#include "PingClient.ipp"
// #endif

#endif
