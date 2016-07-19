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

  bool fail;
  i = lexical_cast<int>("str", &fail);

  cout << "fail:" << fail << ", " << i * i << endl;

  i = lexical_cast<int>('1', &fail);
  cout << "fail:" << fail << ", " << i * i << endl;

  // char k = lexical_cast<char>(142, &fail);
  string k = lexical_cast<string>(142, &fail);

  // char* u = lexical_cast<char*>(142, &fail);

  return 0;
}
