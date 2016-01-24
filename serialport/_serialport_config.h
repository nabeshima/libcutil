/**
 * $Id: _serialport_config.h 4 2013-09-11 05:53:28Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL__SERIALPORT_CONFIG_H_
#define _CUTIL__SERIALPORT_CONFIG_H_

extern "C" {
#include <fcntl.h>
#include <termios.h>
}

/*!
  Definition of constants
*/

namespace cutil {

enum DataBit { FIVE = CS5, SIX = CS6, SEVEN = CS7, EIGHT = CS8 };

enum ParityBit { NONE = 0, EVEN = PARENB, ODD = (PARENB | PARODD) };

enum StopBit { ONE = 0, TWO = CSTOPB };
}

#endif
