/**
 * $Id: SerialPort.h 9 2013-10-10 02:13:07Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_SERIALPORT_H_
#define _CUTIL_SERIALPORT_H_

extern "C" {
#include <stdint.h>
}

#include <string>

#include "./_serialport_config.h"

namespace cutil {

/*!
  POSIX serial communication wrapper
*/

class SerialPort {
 private:
  int fd;
  termios pre_term_attr;

  void _close();

 public:
  SerialPort();
  ~SerialPort();

  bool open(const std::string &portname, int baudrate, DataBit databit = EIGHT,
            ParityBit paritybit = NONE, StopBit stopbit = ONE,
            bool hardware_control = false);

  bool close();

  bool isOpen() const;

  bool isReadable(uint64_t timeout_usec = 0);

  int read(void *data, int size);

  bool isWritable(uint64_t timeout_usec = 0);

  int write(const void *data, int size);

  int bytesInReadBuffer();

  void flushReadBuffer();
  void flushWriteBuffer();
};
}  // namespace cutil

#include "SerialPort.ipp"

#endif
