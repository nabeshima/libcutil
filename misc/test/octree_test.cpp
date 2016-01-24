/**
 * $Id: octree_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <Octree.h>

#include <iostream>
#include <vector>
#include <map>

using namespace std;
using namespace cutil;

int main() {
  const int LEVEL = 3;

  Octree<char, LEVEL> octree;

  for (int i = 0; i < LEVEL; ++i) {
    octree(i, 1, 7, 5) = 'a' + i;
  }

  for (int i = 0; i < LEVEL; ++i) {
    const std::map<unsigned int, unsigned int> &im = octree.indexMap(i);

    const std::vector<char> &iv = octree.instanceVector(i);

    std::map<unsigned int, unsigned int>::const_iterator it = im.begin(),
                                                         endIt = im.end();

    while (it != endIt) {
      cout << "m:" << it->first << ", lv:" << octree.level(it->first)
           << ", value:" << iv[it->second] << endl;

      unsigned int m = it->first;
      while (m != 0) {
        cout << "m:" << m << ", lv:" << octree.level(m)
             << ", value:" << octree[m] << endl;
        m = octree.up(m);
      }
      cout << endl;

      ++it;
    }
  }

  return 0;
}
