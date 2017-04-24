/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_DATE_H_
#define _CUTIL_DATE_H_

#include <string>

namespace cutil {
class Date {
 public:
  static std::string getDateTimeString(const std::string& format,
                                       std::string* millisecond = NULL);
};
}

#include "Date.ipp"

#endif
