/**
 * $Id: CRC32C.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
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
  CRC32C();

  void reset();
  void push(const void *msg, uint64_t msg_byte);

  //! big endian
  void crc(void *result) const;
  static void crc(void *result, const void *msg, uint64_t msg_byte);
};
}  // namespace cutil

#include "CRC32C.ipp"

#endif
