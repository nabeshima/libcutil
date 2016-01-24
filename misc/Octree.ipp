/**
 * $Id: Octree.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace {

//! 下位24bitが指定された番号.
inline unsigned int getBitSeparatedNumber(unsigned char n) {
  int s = n;
  s = (s | s << 8) & 0x0000f00f;
  s = (s | s << 4) & 0x000c30c3;
  s = (s | s << 2) & 0x00249249;
  return s;
}

inline unsigned char getBitCombinedNumber(unsigned int s) {
  s = (s & 0x00249249) | ((s & 0x00249249) >> 2);
  s = (s & 0x000c30c3) | ((s & 0x000c30c3) >> 4);
  s = (s & 0x0000f00f) | ((s & 0x0000f00f) >> 8);
  return (unsigned char)s;
}
}

namespace cutil {

template <class T, unsigned int MAX_LEVEL>
inline int Octree<T, MAX_LEVEL>::level(unsigned int morton_number) {
  unsigned int ref = (0x00000007 << ((MAX_LEVEL - 1) * 3));

  for (unsigned int i = 0; i < MAX_LEVEL; ++i) {
    if ((morton_number & ref) != 0) {
      return MAX_LEVEL - 1 - i;
    }
    ref >>= 3;
  }
  return -1;
}

template <class T, unsigned int MAX_LEVEL>
inline unsigned int Octree<T, MAX_LEVEL>::mortonNumber(unsigned char ix,
                                                       unsigned char iy,
                                                       unsigned char iz) {
  return getBitSeparatedNumber(ix) | getBitSeparatedNumber(iy) << 1 |
         getBitSeparatedNumber(iz) << 2;
}

template <class T, unsigned int MAX_LEVEL>
inline void Octree<T, MAX_LEVEL>::position(unsigned int morton_number,
                                           unsigned char* ix, unsigned char* iy,
                                           unsigned char* iz) {
  *ix = getBitCombinedNumber(morton_number);
  *iy = getBitCombinedNumber(morton_number >> 1);
  *iz = getBitCombinedNumber(morton_number >> 2);
}

template <class T, unsigned int MAX_LEVEL>
inline unsigned int Octree<T, MAX_LEVEL>::up(unsigned int morton_number) {
  return (morton_number >> 3);
}

template <class T, unsigned int MAX_LEVEL>
inline unsigned int Octree<T, MAX_LEVEL>::down(unsigned int morton_number) {
  return (morton_number << 3);
}

template <class T, unsigned int MAX_LEVEL>
inline unsigned int Octree<T, MAX_LEVEL>::enclose(unsigned int morton_numberA,
                                                  unsigned int morton_numberB) {
  return (morton_numberA >> (3 * (level(morton_numberA ^ morton_numberB) + 1)));
}

template <class T, unsigned int MAX_LEVEL>
inline void Octree<T, MAX_LEVEL>::clear() {
  for (int i = 0; i < MAX_LEVEL; ++i) {
    instances[i].clear();
    indices[i].clear();
  }
}

template <class T, unsigned int MAX_LEVEL>
inline T& Octree<T, MAX_LEVEL>::assignInstance(unsigned int morton_number) {
  int lv = level(morton_number);

  std::map<unsigned int, unsigned int>::iterator p =
      indices[lv].find(morton_number);
  if (p != indices[lv].end()) {
    return instances[lv][p->second];
  } else {
    indices[lv].insert(std::pair<unsigned int, unsigned int>(
        morton_number, instances[lv].size()));
    instances[lv].push_back(T());
    return instances[lv].back();
  }
}

template <class T, unsigned int MAX_LEVEL>
inline T& Octree<T, MAX_LEVEL>::assignInstance(unsigned int level,
                                               unsigned char ix,
                                               unsigned char iy,
                                               unsigned char iz) {
  return assignInstance(mortonNumber(ix, iy, iz) >>
                        ((MAX_LEVEL - 1 - level) * 3));
}

template <class T, unsigned int MAX_LEVEL>
inline T& Octree<T, MAX_LEVEL>::operator[](unsigned int morton_number) {
  return assignInstance(morton_number);
}

template <class T, unsigned int MAX_LEVEL>
inline T& Octree<T, MAX_LEVEL>::operator()(unsigned int morton_number) {
  return assignInstance(morton_number);
}

template <class T, unsigned int MAX_LEVEL>
inline T& Octree<T, MAX_LEVEL>::operator()(unsigned int level, unsigned char ix,
                                           unsigned char iy, unsigned char iz) {
  return assignInstance(level, ix, iy, iz);
}

template <class T, unsigned int MAX_LEVEL>
inline int Octree<T, MAX_LEVEL>::size(int level) const {
  if (level == -1) {
    int sum = 0;
    for (int i = 0; i < MAX_LEVEL; ++i) {
      sum += instances[i].size();
    }
    return sum;
  } else {
    return instances[level].size();
  }
}

template <class T, unsigned int MAX_LEVEL>
inline const std::map<unsigned int, unsigned int>&
Octree<T, MAX_LEVEL>::indexMap(unsigned int level) const {
  return indices[level];
}

template <class T, unsigned int MAX_LEVEL>
inline const std::vector<T>& Octree<T, MAX_LEVEL>::instanceVector(
    unsigned int level) const {
  return instances[level];
}
}
