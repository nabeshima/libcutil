/**
 * $Id: lexical_cast.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <sstream>

namespace cutil {

template <typename AFTER, typename BEFORE>
inline AFTER lexical_cast(
    const BEFORE &val, bool *is_fail,
    typename enable_if<
        ((is_numeric<BEFORE>::value && is_string<AFTER>::value) ||
         (is_literal<BEFORE>::value && is_numeric<AFTER>::value))>::type *) {
  AFTER ret;
  std::stringstream strm;
  strm << val;
  strm >> ret;
  if (is_fail != NULL) {
    *is_fail = strm.fail();
  }
  return ret;
}
}
