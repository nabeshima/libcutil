/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_THREADEXCEPTION_H_
#define _CUTIL_THREADEXCEPTION_H_

#include "Exception.h"


namespace cutil {


class ThreadException: public Exception {
public:
  ThreadException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : Exception( msg, file, line, func ) {}
  ThreadException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : Exception( e, file, line, func ) {}
  ThreadException( const Exception &e )
    : Exception( e ) {}
};


}

#endif
