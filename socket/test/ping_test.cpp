/**
 * $Id: ping_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "PingClient.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace cutil;

int main() {
  vector<Address> res;
  //    Address::resolve( &res, "127.0.0.1" );
  Address::resolve(&res, "::1");

  for (unsigned int i = 0; i < res.size(); ++i) {
    cout << res[i].isIPv4() << " " << res[i].isIPv6() << ", "
         << res[i].toString() << "\t" << res[i].getPort() << endl;
  }

  cout << PingClient::ping(res[0], 1000000) << " localhost" << endl;

  Address::resolve(&res, "yahoo.com", false);

  for (unsigned int i = 0; i < res.size(); ++i) {
    cout << res[i].isIPv4() << " " << res[i].isIPv6() << ", "
         << res[i].toString() << "\t" << res[i].getPort() << endl;
  }

  cout << PingClient::ping(res[0], 1000000) << " yahoo.com" << endl;

  Address::resolve(&res, "gglevv.jp", false);

  for (unsigned int i = 0; i < res.size(); ++i) {
    cout << res[i].isIPv4() << " " << res[i].isIPv6() << ", "
         << res[i].toString() << "\t" << res[i].getPort() << endl;
  }

  cout << PingClient::ping(res[0], 1000000) << " gglevv.jp" << endl;

  Address::resolve(&res, "google.jp", false);

  for (unsigned int i = 0; i < res.size(); ++i) {
    cout << res[i].isIPv4() << " " << res[i].isIPv6() << ", "
         << res[i].toString() << "\t" << res[i].getPort() << endl;
  }

  cout << PingClient::ping(res[0], 1000000) << " google.com" << endl;

  return 0;
}
