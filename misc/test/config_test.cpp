/**
 * $Id: config_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <ConfigFileReader.h>
#include <iostream>
#include <map>
#include <string>

using namespace std;
using namespace cutil;

int main() {
  ConfigFileReader cfr;

  cfr.open("test.cfg");

  while (!cfr.isEmpty()) {
    cout << "SECTION <" << cfr.getSection() << ">" << endl;

    map<string, string> &kv = *cfr;

    map<string, string>::iterator it = kv.begin(), endIt = kv.end();
    while (it != endIt) {
      cout << "key: <" << it->first << ">, value: <" << it->second << ">"
           << endl;
      ++it;
    }

    cout << "px: " << kv["px"] << endl;
    cout << "py: " << kv["py"] << endl;
    cout << "cg: " << kv["cg"] << endl;

    ++cfr;
  }

  return 0;
}
