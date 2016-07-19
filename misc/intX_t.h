/**
 * $Id: intX_t.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_INTX_T_H_
#define _CUTIL_INTX_T_H_

extern "C" {
#include <stdint.h>
}

#include <string>
#include <vector>

namespace cutil {

class intX_t {
 private:
  static const uint64_t BASE1 = 10;
  static const uint64_t BASE10 =
      1000000000000000000ULL;  //! the largest 10^x less than LLONG_MAX
  static const uint64_t BASE_CNUM = 18;

  enum SIGN { POSITIVE = 1, NEGATIVE = -1 } sign;
  std::vector<int64_t> body;

  void initByString(const std::string &x);

  static void eraseTopZero(intX_t *result);
  static bool absNoless(const intX_t &l, const intX_t &s);
  static void absAdd(intX_t *result, const intX_t &larger,
                     const intX_t &smaller);
  static void absSub(intX_t *result, const intX_t &larger,
                     const intX_t &smaller);
  static void absMul(intX_t *result, const intX_t &a, const intX_t &b);
  static intX_t absDiv(intX_t *result, const intX_t &num, const intX_t &den);
  static void shiftRight(intX_t *result, const uint64_t &x);
  static void shiftLeft(intX_t *result, const uint64_t &x);
  template <class Functor>
  static void bitOperation(intX_t *result, const intX_t &a, const intX_t &b,
                           Functor func);
  template <class Functor>
  static void bitOperation(intX_t *result, const intX_t &a, Functor func);

 public:
  intX_t();

  template <typename INTEGER>
  explicit intX_t(const INTEGER &x);
  explicit intX_t(const char *x);
  explicit intX_t(const std::string &x);

  template <typename INTEGER>
  intX_t &operator=(const INTEGER &x);

  intX_t operator+(const intX_t &x) const;
  intX_t operator-(const intX_t &x) const;
  intX_t operator*(const intX_t &x) const;
  intX_t operator/(const intX_t &x) const;
  intX_t operator%(const intX_t &x) const;

  intX_t &operator+=(const intX_t &x);
  intX_t &operator-=(const intX_t &x);
  intX_t &operator*=(const intX_t &x);
  intX_t &operator/=(const intX_t &x);
  intX_t &operator%=(const intX_t &x);

  intX_t &operator++();
  intX_t &operator--();
  intX_t operator++(int);
  intX_t operator--(int);

  intX_t operator-() const;

  intX_t operator&(const intX_t &x) const;
  intX_t operator|(const intX_t &x) const;
  intX_t operator^(const intX_t &x) const;
  intX_t &operator&=(const intX_t &x);
  intX_t &operator|=(const intX_t &x);
  intX_t &operator^=(const intX_t &x);

  intX_t operator~() const;

  intX_t operator>>(const uint64_t &x) const;
  intX_t operator<<(const uint64_t &x) const;
  intX_t &operator>>=(const uint64_t &x);
  intX_t &operator<<=(const uint64_t &x);

  bool operator==(const intX_t &x) const;
  bool operator!=(const intX_t &x) const;
  bool operator<(const intX_t &x) const;
  bool operator>(const intX_t &x) const;
  bool operator<=(const intX_t &x) const;
  bool operator>=(const intX_t &x) const;

  std::string toString() const;
  std::string toHexString() const;
  std::string toBinString() const;

  //! little endian
  static void toBinArray(std::vector<unsigned char> *result, const intX_t &a);
};

std::ostream &operator<<(std::ostream &stream, const intX_t &x);
}  // namespace cutil

#include "intX_t.ipp"

#endif
