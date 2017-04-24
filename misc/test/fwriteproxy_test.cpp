/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>
#include <string>

#include "./FileWriteProxy.h"

using namespace std;
using namespace cutil;

int main() {
  FileWriteProxy fwp;
  fwp.setPriority(Thread::self().getPriority() - 1);
  fwp.open("/tmp/tmp.txt", FileWriteProxy::WRITE);

  FileWriteProxy fwp2;
  fwp2.open("/tmp/tmp2.txt", FileWriteProxy::APPEND);
  fwp.setPriority(Thread::self().getPriority() + 1);

  fwp.printf("tgst %02d, %s\n", 952, "TEST");

  fwp.printf("ggg %02d, %s\n", 12, "TEST");

  fwp2.printf("22 ggg %02d, %s\n", 12, "TEST");
  for (int i = 0; i < 100; ++i) {
    fwp2.printf("%d ", i, "awab");
  }

  fwp.ostream() << "stream " << 512 << "," << std::showbase << std::hex << 512
                << "\n";

  fwp2.ostream() << 82.1199 << "::: ";

  fwp.close();
  fwp2.close();

  return 0;
}
