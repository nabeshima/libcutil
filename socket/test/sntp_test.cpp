/**
 * $Id: sntp_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "SNTPClient.h"

#include <iostream>
#include <vector>

using namespace std;
using namespace cutil;

int main() {
  Address ntpserver;

  vector<Address> res;
  Address::resolve(&res, "ntp.nict.jp", false);

  for (unsigned int i = 0; i < res.size(); ++i) {
    cout << res[i].isIPv4() << " " << res[i].isIPv6() << ", "
         << res[i].toString() << "\t" << res[i].getPort() << endl;
    if (res[i].isIPv4()) {
      ntpserver = res[i];
      break;
    }
  }
  ntpserver.setPort(123);

  timeval tv = SNTPClient::getClock(ntpserver, 10, 1000000);
  cout << " tv_sec: " << tv.tv_sec << endl;
  cout << "tv_usec: " << tv.tv_usec << endl;

  time_t timer;
  timer = tv.tv_sec;
  tm t_st;
  localtime_r(&timer, &t_st);
  cout << (t_st.tm_year + 1900) << "/" << (t_st.tm_mon + 1) << "/"
       << t_st.tm_mday << ", " << t_st.tm_hour << ":" << t_st.tm_min << ":"
       << t_st.tm_sec << endl;

  if (SNTPClient::setClock(ntpserver, 10, 1000000)) {
    cout << "set clock" << endl;
  } else {
    cout << "fail to set clock. no auth." << endl;
  }
}
