/**
 * $Id: lexical_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "./lexical_cast.h"

#include <iostream>
#include <string>

using namespace std;
using namespace cutil;

int main() {
  string str = lexical_cast<string>(180);

  cout << str << endl;

  int i = lexical_cast<int>("179");

  cout << i * i << endl;

  return 0;
}
