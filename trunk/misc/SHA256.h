/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_SHA256_H_
#define _CUTIL_SHA256_H_

extern "C" {
#include <stdint.h>
}

#include <cstddef>

namespace cutil {

class SHA256 {
private:
  uint32_t H[ 8 ]; //! hash
  
  uint64_t count;
  uint32_t dnum;
  unsigned char data[ 64 ];
  
  void calc() throw ();
  void end() throw ();
  
public:
  SHA256() throw ();
  void reset() throw ();
  void push( const void *msg, uint64_t msg_byte ) throw ();
  
  //! big endian
  void hash( void *result ) const throw ();
  static void hash( void *result,
                    const void *msg, 
                    uint64_t msg_byte ) throw ();
};

}

#include "SHA256.ipp"

#endif
