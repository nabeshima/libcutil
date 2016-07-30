/**
 * $Id$
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <algorithm>

namespace cutil {

template <typename VALUE>
inline void byte_cast(VALUE value, void *output, bool as_big_endian,
                      typename enable_if<is_numeric<VALUE>::value>::type *) {
  uint8_t *first = reinterpret_cast<uint8_t *>(&value);
  uint8_t *last = first + sizeof(VALUE);

#ifdef __BIG_ENDIAN__
  if (as_big_endian) {
    std::copy(first, last, reinterpret_cast<uint8_t *>(output));
  } else {
    std::reverse_copy(first, last, reinterpret_cast<uint8_t *>(output));
  }
#else
  if (as_big_endian) {
    std::reverse_copy(first, last, reinterpret_cast<uint8_t *>(output));
  } else {
    std::copy(first, last, reinterpret_cast<uint8_t *>(output));
  }
#endif
}

template <typename VALUE>
inline VALUE byte_cast(const void *input, bool is_big_endian,
                       typename enable_if<is_numeric<VALUE>::value>::type *) {
  VALUE value;
  const uint8_t *first = reinterpret_cast<const uint8_t *>(input);
  const uint8_t *last = first + sizeof(VALUE);

#ifdef __BIG_ENDIAN__
  if (is_big_endian) {
    std::copy(first, last, reinterpret_cast<uint8_t *>(&value));
  } else {
    std::reverse_copy(first, last, reinterpret_cast<uint8_t *>(&value));
  }
#else
  if (is_big_endian) {
    std::reverse_copy(first, last, reinterpret_cast<uint8_t *>(&value));
  } else {
    std::copy(first, last, reinterpret_cast<uint8_t *>(&value));
  }
#endif
  return value;
}
}
