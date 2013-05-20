/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_LEXICAL_CAST_H_
#define _CUTIL_LEXICAL_CAST_H_

namespace cutil {

template< typename AFTER, typename BEFORE >
AFTER lexical_cast( const BEFORE &val );

}

#include "lexical_cast.ipp"

#endif
