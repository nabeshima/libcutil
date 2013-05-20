/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_CRC32C_H_
#define _CUTIL_CRC32C_H_

extern "C" {
#include <stdint.h>
}

namespace cutil {

class CRC32C {
private:
  uint32_t crc32c;
  
public:
  CRC32C() throw ();
  
  void reset() throw ();
  void push( const void *msg, uint64_t msg_byte ) throw ();

  //! big endian
  void crc( void *result ) const throw ();
  static void crc( void *result, const void *msg, uint64_t msg_byte ) throw ();
};

}

#include "CRC32C.ipp"

#endif
