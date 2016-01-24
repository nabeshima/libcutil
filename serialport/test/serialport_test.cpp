/**
 * $Id$
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "SerialPort.h"

#include <iostream>

using namespace std;
using namespace cutil;

int main() {
  SerialPort sport;

  // to secify baudrate by using B460800 if available
  bool ret = sport.open("/dev/ttyACM0", 460800, EIGHT, NONE, ONE);
  cout << ret << endl;

  while (sport.isReadable(20000)) {
    int64_t l = 0;
    int rc = sport.read(&l, sizeof(l));

    cout << rc << " " << l << endl;
  }

  return 0;
}
