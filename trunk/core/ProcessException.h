/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_PROCESSEXCEPTION_H_
#define _CUTIL_PROCESSEXCEPTION_H_

#include "Exception.h"


namespace cutil {


class ProcessException: public Exception {
public:
  ProcessException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : Exception( msg, file, line, func ) {}
  ProcessException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : Exception( e, file, line, func ) {}
  ProcessException( const Exception &e )
    : Exception( e ) {}
};

class ProcessTimeoutException: public Exception {
public:
  ProcessTimeoutException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : Exception( msg, file, line, func ) {}
  ProcessTimeoutException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : Exception( e, file, line, func ) {}
  ProcessTimeoutException( const Exception &e )
    : Exception( e ) {}
};


}

#endif
