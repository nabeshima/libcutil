/**
 * $Id: stl_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>
#include <vector>
#include <STLFileIO.h>

using namespace std;
using namespace cutil;
using namespace cotave;

int main() {
  vector<ColumnVector3> vertices;
  vector<IntVector3> indices;

  STLFileIO::read("cube_ascii.stl", &vertices, &indices);
  STLFileIO::read("cube_binary.stl", &vertices, &indices);

  return 0;
}
