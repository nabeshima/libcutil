/**
 * $Id$
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_BYTE_CAST_H_
#define _CUTIL_BYTE_CAST_H_

#include "./type_traits.h"

namespace cutil {

template <typename VALUE>
void byte_cast(VALUE value, void *output, bool as_big_endian = false,
               typename enable_if<is_numeric<VALUE>::value>::type * = 0);

template <typename VALUE>
VALUE byte_cast(const void *input, bool is_big_endian = false,
                typename enable_if<is_numeric<VALUE>::value>::type * = 0);

}  // namespace cutil

#include "byte_cast.ipp"

#endif
