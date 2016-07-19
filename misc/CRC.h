/**
 * $Id: CRC.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_CRC_H_
#define _CUTIL_CRC_H_

extern "C" {
#include <stdint.h>
}

namespace cutil {

enum CRC_TYPE { CRC_8_ATM, CRC_16_CCITT, CRC_32, CRC_32C };

template <CRC_TYPE CT>
class CRC {
 public:
  CRC();
  void reset();
  void push(const void *msg, uint64_t msg_byte);

  //! Stores CRC value in big Endian.
  void crc(void *result) const;

  //! Calculate and stores CRC
  static void crc(void *result, const void *msg, uint64_t msg_byte);

  //! Check the health of CRC value
  static bool check(const void *crc_value, const void *msg, uint64_t msg_byte);
};

}  // namespace cutil

#include "CRC.ipp"

#endif
