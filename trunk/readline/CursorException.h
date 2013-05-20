/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_CURSOREXCEPTION_H__
#define __CUTIL_CURSOREXCEPTION_H__

#include "Exception.h"


namespace cutil {


class CursorException: public Exception {
public:
  CursorException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : Exception( msg, file, line, func ) {}
  CursorException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : Exception( e, file, line, func ) {}
  CursorException( const Exception &e )
    : Exception( e ) {}
};


}

#endif
