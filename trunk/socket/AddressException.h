/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_ADDRESSEXCEPTION_H_
#define _CUTIL_ADDRESSEXCEPTION_H_

#include "Exception.h"

namespace cutil {

//############################################################################
//                    Definition of socket exception classes

/*!
  Address Exception Base class.
  It holds source information.
*/
class AddressException: public Exception {
public:
  AddressException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : Exception( msg, file, line, func ) {}
  AddressException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : Exception( e, file, line, func ) {}
  AddressException( const Exception &e )
    : Exception( e ) {}
};


/*!
  Throw if hostname is unsolved.
*/
class ResolveHostnameException: public AddressException {
public:
  ResolveHostnameException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : AddressException( msg, file, line, func ) {}
  ResolveHostnameException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : AddressException( e, file, line, func ) {}
  ResolveHostnameException( const Exception &e )
    : AddressException( e ) {}
};


}


#endif
