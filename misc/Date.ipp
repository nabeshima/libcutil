/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cutil {
inline std::string Date::getDateTimeString(const std::string& format,
                                           std::string* millisecond) {
  timeval tv;
  gettimeofday(&tv, NULL);

  tm timeinfo;
  char buffer[80];
  localtime_r(&tv.tv_sec, &timeinfo);
  strftime(buffer, 80, format.c_str(), &timeinfo);

  if (millisecond != NULL) {
    char mbuf[4];
    sprintf(mbuf, "%03d", tv.tv_usec / 1000);
    *millisecond = mbuf;
  }
  return buffer;
}
}
