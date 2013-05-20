/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_SOCKETEXCEPTION_H_
#define _CUTIL_SOCKETEXCEPTION_H_

#include "Exception.h"

namespace cutil {

//############################################################################
//                    Definition of socket exception classes

/*!
  Socket Exception Base class.
  It holds source information.
*/
class SocketException: public Exception {
public:
  SocketException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : Exception( msg, file, line, func ) {}
  SocketException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : Exception( e, file, line, func ) {}
  SocketException( const Exception &e )
    : Exception( e ) {}
};


/*!
  Throw if socket communication is timeout.
*/
class SocketTimeoutException: public SocketException {
public:
  SocketTimeoutException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : SocketException( msg, file, line, func ) {}
  SocketTimeoutException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : SocketException( e, file, line, func ) {}
  SocketTimeoutException( const Exception &e )
    : SocketException( e ) {}
};

class SocketFamilyUnmatchException: public SocketException {
public:
  SocketFamilyUnmatchException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : SocketException( msg, file, line, func ) {}
  SocketFamilyUnmatchException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : SocketException( e, file, line, func ) {}
  SocketFamilyUnmatchException( const Exception &e )
    : SocketException( e ) {}
};

class SocketTypeUnmatchException: public SocketException {
public:
  SocketTypeUnmatchException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : SocketException( msg, file, line, func ) {}
  SocketTypeUnmatchException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : SocketException( e, file, line, func ) {}
  SocketTypeUnmatchException( const Exception &e )
    : SocketException( e ) {}
};


}

#endif
