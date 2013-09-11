/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_SERIALPORT_H_
#define _CUTIL_SERIALPORT_H_

#include <string>

#include "_serialport_config.h"
#include "SerialPortException.h"

namespace cutil {

/*!
  POSIX serial communication wrapper
*/

class SerialPort {
private:
  int fd;
  termios pre_term_attr;

public:
  SerialPort() throw ();
  ~SerialPort() throw ();
  
  bool open( const std::string& portname,
	     int baudrate,
	     DataBit databit = EIGHT,
	     ParityBit paritybit = NONE,
	     StopBit stopbit = ONE,
	     bool hardware_control = false )
    throw ( SerialPortException );
  
  void close() 
    throw ( SerialPortException );
  
  bool isOpen() const throw ();
  
  
  bool isReadable( uint64_t timeout_usec = 0 )
    throw ( SerialPortException, SerialPortTimeoutException );  

  int read( void *data, int size ) throw ();  
  
  bool isWritable( uint64_t timeout_usec = 0 )
    throw ( SerialPortException, SerialPortTimeoutException );

  int write( const void *data, int size ) throw ();
};

}

#include "SerialPort.ipp"

#endif

