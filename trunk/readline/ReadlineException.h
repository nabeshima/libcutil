/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_READLINEEXCEPTION_H__
#define __CUTIL_READLINEEXCEPTION_H__

#include "Exception.h"

namespace cutil {

class ReadlineException: public Exception {
public:
  ReadlineException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : Exception( msg, file, line, func ) {}
  ReadlineException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : Exception( e, file, line, func ) {}
  ReadlineException( const Exception &e )
    : Exception( e ) {}
};


class ReadlineTimeoutException: public ReadlineException {
public:
  ReadlineTimeoutException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : ReadlineException( msg, file, line, func ) {}
  ReadlineTimeoutException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : ReadlineException( e, file, line, func ) {}
  ReadlineTimeoutException( const Exception &e )
    : ReadlineException( e ) {}
};


}

#endif
