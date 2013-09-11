/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_SERIALPORTEXCEPTION_H__
#define __CUTIL_SERIALPORTEXCEPTION_H__

#include "Exception.h"

namespace cutil {

class SerialPortException: public Exception {
public:
  SerialPortException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : Exception( msg, file, line, func ) {}
  SerialPortException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : Exception( e, file, line, func ) {}
  SerialPortException( const Exception &e )
    : Exception( e ) {}
};


class SerialPortTimeoutException: public SerialPortException {
public:
  SerialPortTimeoutException( const std::string &msg, const std::string &file, int line, const std::string &func )
    : SerialPortException( msg, file, line, func ) {}
  SerialPortTimeoutException( const std::runtime_error &e, const std::string &file, int line, const std::string &func )
    : SerialPortException( e, file, line, func ) {}
  SerialPortTimeoutException( const Exception &e )
    : SerialPortException( e ) {}
};


}

#endif
