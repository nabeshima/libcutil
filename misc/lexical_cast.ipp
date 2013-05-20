/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <sstream>

namespace cutil {

template< typename AFTER, typename BEFORE > inline
AFTER lexical_cast( const BEFORE &val ) {
  AFTER ret;
  std::stringstream strm;
  strm << val;
  strm >> ret;
  return ret;
}

}
