/**
 * $Id: SHA256.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
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
  uint32_t H[8];  //! hash

  uint64_t count;
  uint32_t dnum;
  unsigned char data[64];

  void calc();
  void end();

 public:
  SHA256();
  void reset();
  void push(const void *msg, uint64_t msg_byte);

  //! big endian
  void hash(void *result) const;
  static void hash(void *result, const void *msg, uint64_t msg_byte);
};
}  // namespace cutil

#include "SHA256.ipp"

#endif
