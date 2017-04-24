/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>
#include <string>

#include "CLog.h"

using namespace std;
using namespace cutil;

int main() {
  CLog clog;
  clog.open("/tmp/log", 3000, true);

  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::INFO, "info %d", i);
  }
  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::ALERT, "alert %d", i);
  }

  clog.setLogLevel(CLog::NOTICE);

  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::INFO, "info2 %d", i);  // to be neglected;
  }
  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::NOTICE, "notice2 %d", i);
  }
  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::ERROR, "error2 %d", i);
  }

  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::EMERG, "emerg2 %d", i);
  }

  clog.close();

  clog.setLogLevel(CLog::INFO);

  clog.open("/tmp/log", 1200, true);

  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::INFO, "info %d", i);
  }
  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::ALERT, "alert %d", i);
  }

  clog.setLogLevel(CLog::NOTICE);

  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::INFO, "info2 %d", i);  // to be neglected;
  }
  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::NOTICE, "notice2 %d", i);
  }
  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::ERROR, "error2 %d", i);
  }

  for (int i = 0; i < 10; ++i) {
    clog.printf(CLog::EMERG, "emerg2 %d", i);
  }

  clog.close();

  return 0;
}
