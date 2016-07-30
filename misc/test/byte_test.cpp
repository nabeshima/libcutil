/**
 * $Id$
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "./byte_cast.h"

#include <iostream>
#include <string>

using namespace std;
using namespace cutil;

int main() {
  {
    uint8_t data[] = {0x01, 0x02, 0x03, 0x80};

    int32_t ret = byte_cast<int32_t>(data);

    cout << ret << endl;

    ret = byte_cast<int32_t>(data, true);

    cout << ret << endl;

    byte_cast(ret, data);
    ret = byte_cast<int32_t>(data);

    cout << ret << endl;

    ret = byte_cast<int32_t>(data, true);

    cout << ret << endl;
  }

  {
    uint8_t data[] = {0x01, 0x02, 0x03, 0x80, 0x01, 0x02, 0x03, 0x80};

    double ret = byte_cast<double>(data);

    cout << ret << endl;

    ret = byte_cast<double>(data, true);

    cout << ret << endl;

    byte_cast(ret, data);
    ret = byte_cast<double>(data);

    cout << ret << endl;

    ret = byte_cast<double>(data, true);

    cout << ret << endl;
  }

  return 0;
}
