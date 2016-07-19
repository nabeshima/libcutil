/**
 * $Id: lexical_cast.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_LEXICAL_CAST_H_
#define _CUTIL_LEXICAL_CAST_H_

#include <cstddef>

#include "./type_traits.h"

namespace cutil {

template <typename AFTER, typename BEFORE>
AFTER lexical_cast(
    const BEFORE &val, bool *is_fail = NULL,
    typename enable_if<
        ((is_numeric<BEFORE>::value && is_string<AFTER>::value) ||
         (is_literal<BEFORE>::value && is_numeric<AFTER>::value))>::type * = 0);
}

#include "lexical_cast.ipp"

#endif
