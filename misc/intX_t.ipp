/**
 * $Id: intX_t.ipp 5 2013-09-25 08:42:57Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <sstream>
#include <iomanip>

namespace cutil {

namespace {

inline int64_t convertToDec(char c) {
  if ('0' <= c && c <= '9') {
    return c - '0';
  } else if ('a' <= c && c <= 'z') {
    return 10 + c - 'a';
  } else if ('A' <= c && c <= 'Z') {
    return 10 + c - 'A';
  } else {
    return 0;  //! assertion
  }
}

class AND {
 public:
  unsigned char operator()(const unsigned char &a, const unsigned char &b) {
    return (a & b);
  }
};

class OR {
 public:
  unsigned char operator()(const unsigned char &a, const unsigned char &b) {
    return (a | b);
  }
};

class XOR {
 public:
  unsigned char operator()(const unsigned char &a, const unsigned char &b) {
    return (a ^ b);
  }
};

class NOT {
 public:
  unsigned char operator()(const unsigned char &a) { return ~a; }
};
}

inline void intX_t::initByString(const std::string &x) {
  uint64_t digit;

  std::string::const_iterator it = x.begin(), endIt = x.end();

  if (it != endIt && *it == '0') {
    digit = 8;
    ++it;

    if (it != endIt && *it == 'x') {
      digit = 16;
      ++it;
    }
  } else {
    digit = 10;

    if (it != endIt && *it == '-') {
      sign = NEGATIVE;
      ++it;
    }
  }

  while (it != endIt) {
    *this *= intX_t(digit);
    *this += intX_t(convertToDec(*it));
    ++it;
  }
}

inline void intX_t::eraseTopZero(intX_t *result) {
  std::vector<int64_t>::reverse_iterator rit = result->body.rbegin(),
                                         rendIt = result->body.rend();

  while (rit != rendIt) {
    if (*rit == 0) {
      result->body.erase((++rit).base());
    } else {
      break;
    }
  }

  if (result->body.size() == 0) {
    result->sign = POSITIVE;
  }
}

//! abs( l ) is no less than abs( s ). l >= s
inline bool intX_t::absNoless(const intX_t &l, const intX_t &s) {
  if (l.body.size() < s.body.size()) {
    return false;
  } else if (l.body.size() > s.body.size()) {
    return true;
  } else {
    std::vector<int64_t>::const_reverse_iterator lit = l.body.rbegin(),
                                                 lendIt = l.body.rend(),
                                                 sit = s.body.rbegin();

    while (lit != lendIt) {
      if (*lit < *sit) {
        return false;
      } else if (*lit > *sit) {
        return true;
      }
      ++lit;
      ++sit;
    }
    return true;
  }
}

inline void intX_t::absAdd(intX_t *result, const intX_t &larger,
                           const intX_t &smaller) {
  result->body.resize(larger.body.size());

  int64_t overflow = 0;

  std::vector<int64_t>::iterator rit = result->body.begin();

  std::vector<int64_t>::const_iterator lit = larger.body.begin(),
                                       lendIt = larger.body.end(),
                                       sit = smaller.body.begin(),
                                       sendIt = smaller.body.end();

  while (sit != sendIt) {
    *rit = *lit + *sit + overflow;
    overflow = *rit / BASE10;
    *rit -= overflow * BASE10;  // sum % BASE10;

    ++rit;
    ++lit;
    ++sit;
  }
  while (lit != lendIt) {
    *rit = *lit + overflow;
    overflow = *rit / BASE10;
    *rit -= overflow * BASE10;  // sum % BASE10;

    ++rit;
    ++lit;
  }
  if (overflow > 0) {
    result->body.push_back(overflow);
  }
}

inline void intX_t::absSub(intX_t *result, const intX_t &larger,
                           const intX_t &smaller) {
  result->body.resize(larger.body.size());

  int64_t underflow = 0;

  std::vector<int64_t>::iterator rit = result->body.begin();

  std::vector<int64_t>::const_iterator lit = larger.body.begin(),
                                       lendIt = larger.body.end(),
                                       sit = smaller.body.begin(),
                                       sendIt = smaller.body.end();

  while (sit != sendIt) {
    *rit = *lit - *sit - underflow;
    if (*rit < 0) {
      underflow = 1 + (-*rit / BASE10);
      *rit += BASE10;
    } else {
      underflow = 0;
    }

    ++rit;
    ++lit;
    ++sit;
  }
  while (lit != lendIt) {
    *rit = *lit - underflow;
    if (*rit < 0) {
      underflow = 1 + (-*rit / BASE10);
      *rit += BASE10;
    } else {
      underflow = 0;
    }

    ++rit;
    ++lit;
  }

  eraseTopZero(result);
}

inline void intX_t::absMul(intX_t *result, const intX_t &a, const intX_t &b) {
  intX_t zero;

  if (a == zero || b == zero) {
    *result = zero;
    return;
  }

  intX_t atmp(a), btmp(b);

  *result = zero;

  while (btmp != zero) {
    if ((btmp.body.front() & 1) != 0) {
      *result += atmp;
    }
    btmp >>= 1;
    atmp <<= 1;
  }
}

inline intX_t intX_t::absDiv(intX_t *result, const intX_t &num,
                             const intX_t &den) {
  intX_t zero;

  if (num == zero) {
    *result = zero;
    return zero;
  }
  if (den == zero) {
    *result = num.body.size() / zero.body.size();  // FPE Exception.
  }

  int64_t shift = 0;

  intX_t ntmp(num), dtmp(den);

  *result = zero;

  while (dtmp < ntmp) {
    dtmp <<= 1;
    ++shift;
  }
  if (dtmp > ntmp) {
    dtmp >>= 1;
    --shift;
  }

  if (shift >= 0) {
    for (int64_t i = 0; i <= shift; ++i) {
      if (dtmp <= ntmp) {
        ntmp -= dtmp;
        dtmp >>= 1;
        *result <<= 1;

        ++(*result);
      } else {
        dtmp >>= 1;
        *result <<= 1;
      }
    }
  }

  ntmp.sign = POSITIVE;
  return ntmp;
}

inline void intX_t::shiftRight(intX_t *result, const uint64_t &x) {
  std::vector<int64_t>::reverse_iterator rit, rendIt = result->body.rend();

  for (uint64_t xx = x; xx > 0; --xx) {
    int64_t underflow = 0;

    rit = result->body.rbegin();

    while (rit != rendIt) {
      *rit += underflow;
      underflow = ((*rit & 1) == 1) ? BASE10 : 0;
      *rit >>= 1;

      ++rit;
    }
  }

  eraseTopZero(result);
}

inline void intX_t::shiftLeft(intX_t *result, const uint64_t &x) {
  for (uint64_t xx = x; xx > 0; --xx) {
    int64_t overflow = 0;

    std::vector<int64_t>::iterator rit = result->body.begin(),
                                   rendIt = result->body.end();

    while (rit != rendIt) {
      *rit <<= 1;
      *rit += overflow;
      overflow = *rit / BASE10;
      *rit -= overflow * BASE10;

      ++rit;
    }
    if (overflow > 0) {
      result->body.push_back(overflow);
    }
  }
}

void intX_t::toBinArray(std::vector<unsigned char> *result, const intX_t &a) {
  result->clear();

  intX_t atmp(a), zero(0), mask(0x100);

  while (atmp != zero) {
    intX_t div;
    intX_t res = absDiv(&div, atmp, mask);
    result->push_back((res.body.size() == 0) ? 0 : res.body.front());
    atmp = div;
  }
}

template <class Functor>
void intX_t::bitOperation(intX_t *result, const intX_t &a, const intX_t &b,
                          Functor func) {
  bool largerFlag = absNoless(a, b);

  std::vector<unsigned char> abit, bbit;

  toBinArray(&abit, a);
  toBinArray(&bbit, b);

  std::vector<unsigned char> &larger = (largerFlag) ? abit : bbit,
                             &smaller = (largerFlag) ? bbit : abit;

  std::vector<unsigned char>::iterator lit = larger.begin(),
                                       lendIt = larger.end(),
                                       sit = smaller.begin(),
                                       sendIt = smaller.end();

  while (sit != sendIt) {
    *lit = func(*lit, *sit);

    ++lit;
    ++sit;
  }
  while (lit != lendIt) {
    *lit = func(*lit, 0);
    ++lit;
  }

  std::vector<unsigned char>::const_reverse_iterator rit = larger.rbegin(),
                                                     rendIt = larger.rend();

  while (rit != rendIt) {
    *result <<= 8;
    *result += intX_t(*rit);
    ++rit;
  }
}

template <class Functor>
void intX_t::bitOperation(intX_t *result, const intX_t &a, Functor func) {
  std::vector<unsigned char> abit;

  toBinArray(&abit, a);

  std::vector<unsigned char>::iterator it = abit.begin(), endIt = abit.end();

  while (it != endIt) {
    *it = func(*it);
    ++it;
  }

  std::vector<unsigned char>::const_reverse_iterator rit = abit.rbegin(),
                                                     rendIt = abit.rend();

  while (rit != rendIt) {
    *result <<= 8;
    *result += intX_t(*rit);
    ++rit;
  }
}

inline intX_t::intX_t() : sign(POSITIVE) {}

template <typename INTEGER>
inline intX_t::intX_t(const INTEGER &x)
    : sign(POSITIVE) {
  if (x == 0) {
    return;
  }

  sign = (x > 0) ? POSITIVE : NEGATIVE;

  uint64_t xx = (x > 0) ? x : -x;

  if (BASE10 <= xx) {
    int64_t div = xx / BASE10;
    body.push_back(xx - div * BASE10);  // xx % BASE10
    body.push_back(div);
  } else {
    body.push_back(xx);
  }
}

inline intX_t::intX_t(const char *x) : sign(POSITIVE) { initByString(x); }

inline intX_t::intX_t(const std::string &x) : sign(POSITIVE) {
  initByString(x);
}

template <typename INTEGER>
inline intX_t &intX_t::operator=(const INTEGER &x) {
  return (*this = intX_t(x));
}

inline intX_t intX_t::operator+(const intX_t &x) const {
  intX_t ret;

  bool largerFlag = absNoless(*this, x);

  const intX_t &larger = (largerFlag) ? *this : x,
               &smaller = (largerFlag) ? x : *this;

  if (sign == x.sign) {
    ret.sign = sign;
    absAdd(&ret, larger, smaller);
  } else {
    ret.sign = larger.sign;
    absSub(&ret, larger, smaller);
  }

  return ret;
}

inline intX_t intX_t::operator-(const intX_t &x) const {
  intX_t ret;

  bool largerFlag = absNoless(*this, x);

  const intX_t &larger = (largerFlag) ? *this : x,
               &smaller = (largerFlag) ? x : *this;

  if (sign == -x.sign) {
    ret.sign = sign;
    absAdd(&ret, larger, smaller);
  } else {
    ret.sign = larger.sign;
    if (&larger == &x) {
      ret.sign = (ret.sign == POSITIVE) ? NEGATIVE : POSITIVE;
    }
    absSub(&ret, larger, smaller);
  }

  return ret;
}

inline intX_t intX_t::operator*(const intX_t &x) const {
  intX_t ret;
  absMul(&ret, *this, x);
  ret.sign = (sign == x.sign) ? POSITIVE : NEGATIVE;
  return ret;
}

inline intX_t intX_t::operator/(const intX_t &x) const {
  intX_t ret;
  absDiv(&ret, *this, x);
  ret.sign = (sign == x.sign) ? POSITIVE : NEGATIVE;
  return ret;
}

inline intX_t intX_t::operator%(const intX_t &x) const {
  intX_t dummy;
  return absDiv(&dummy, *this, x);
}

inline intX_t &intX_t::operator+=(const intX_t &x) {
  bool largerFlag = absNoless(*this, x);

  const intX_t &larger = (largerFlag) ? *this : x,
               &smaller = (largerFlag) ? x : *this;

  if (sign == x.sign) {
    absAdd(this, larger, smaller);
  } else {
    sign = larger.sign;
    absSub(this, larger, smaller);
  }
  return *this;
}

inline intX_t &intX_t::operator-=(const intX_t &x) {
  bool largerFlag = absNoless(*this, x);

  const intX_t &larger = (largerFlag) ? *this : x,
               &smaller = (largerFlag) ? x : *this;

  if (sign == -x.sign) {
    absAdd(this, larger, smaller);
  } else {
    sign = larger.sign;
    if (&larger == &x) {
      sign = (sign == POSITIVE) ? NEGATIVE : POSITIVE;
    }
    absSub(this, larger, smaller);
  }

  return *this;
}

inline intX_t &intX_t::operator*=(const intX_t &x) {
  SIGN s = (sign == x.sign) ? POSITIVE : NEGATIVE;
  absMul(this, *this, x);
  sign = s;
  return *this;
}

inline intX_t &intX_t::operator/=(const intX_t &x) {
  SIGN s = (sign == x.sign) ? POSITIVE : NEGATIVE;
  absDiv(this, *this, x);
  sign = s;
  return *this;
}

inline intX_t &intX_t::operator%=(const intX_t &x) {
  intX_t dummy;
  return (*this = absDiv(&dummy, *this, x));
}

inline intX_t &intX_t::operator++() { return (*this += intX_t(1)); }

inline intX_t &intX_t::operator--() { return (*this -= intX_t(1)); }

inline intX_t intX_t::operator++(int) {
  intX_t ret(*this);
  ++(*this);
  return ret;
}

inline intX_t intX_t::operator--(int) {
  intX_t ret(*this);
  --(*this);
  return ret;
}

inline intX_t intX_t::operator-() const {
  intX_t ret(*this);
  ret.sign = (sign == POSITIVE) ? NEGATIVE : POSITIVE;
  return ret;
}

inline intX_t intX_t::operator&(const intX_t &x) const {
  intX_t ret;
  bitOperation(&ret, *this, x, AND());
  return ret;
}

inline intX_t intX_t::operator|(const intX_t &x) const {
  intX_t ret;
  bitOperation(&ret, *this, x, OR());
  return ret;
}

inline intX_t intX_t::operator^(const intX_t &x) const {
  intX_t ret;
  bitOperation(&ret, *this, x, XOR());
  return ret;
}

inline intX_t &intX_t::operator&=(const intX_t &x) {
  bitOperation(this, *this, x, AND());
  return *this;
}

inline intX_t &intX_t::operator|=(const intX_t &x) {
  bitOperation(this, *this, x, OR());
  return *this;
}

inline intX_t &intX_t::operator^=(const intX_t &x) {
  bitOperation(this, *this, x, XOR());
  return *this;
}

inline intX_t intX_t::operator~() const {
  intX_t ret;
  bitOperation(&ret, *this, NOT());
  return ret;
}

inline intX_t intX_t::operator>>(const uint64_t &x) const {
  intX_t ret(*this);
  shiftRight(&ret, x);
  return ret;
}

inline intX_t intX_t::operator<<(const uint64_t &x) const {
  intX_t ret(*this);
  shiftLeft(&ret, x);
  return ret;
}

inline intX_t &intX_t::operator>>=(const uint64_t &x) {
  shiftRight(this, x);
  return *this;
}

inline intX_t &intX_t::operator<<=(const uint64_t &x) {
  shiftLeft(this, x);
  return *this;
}

inline bool intX_t::operator==(const intX_t &x) const {
  if (sign != x.sign) {
    return false;
  }
  if (body.size() != x.body.size()) {
    return false;
  }

  std::vector<int64_t>::const_reverse_iterator it = body.rbegin(),
                                               endIt = body.rend(),
                                               xit = x.body.rbegin();

  while (it != endIt) {
    if (*it != *xit) {
      return false;
    }
    ++it;
    ++xit;
  }
  return true;
}

inline bool intX_t::operator!=(const intX_t &x) const { return !(*this == x); }

inline bool intX_t::operator<(const intX_t &x) const { return !(*this >= x); }

inline bool intX_t::operator>(const intX_t &x) const { return !(*this <= x); }

inline bool intX_t::operator<=(const intX_t &x) const {
  if (sign < x.sign) {
    return true;
  } else if (sign > x.sign) {
    return false;
  }

  return absNoless(x, *this);
}

inline bool intX_t::operator>=(const intX_t &x) const {
  if (sign > x.sign) {
    return true;
  } else if (sign < x.sign) {
    return false;
  }

  return absNoless(*this, x);
}

inline std::string intX_t::toString() const {
  if (body.size() == 0) {
    return "0";
  }

  std::stringstream sstrm;
  sstrm << ((sign == NEGATIVE) ? "-" : "");

  std::vector<int64_t>::const_reverse_iterator it = body.rbegin(),
                                               endIt = body.rend();

  while (it != endIt) {
    sstrm << *it << std::right << std::setw(BASE_CNUM) << std::setfill('0');
    ++it;
  }
  return sstrm.str();
}

inline std::string intX_t::toBinString() const {
  if (body.size() == 0) {
    return "0";
  }

  std::string ret;

  std::vector<unsigned char> bin;
  toBinArray(&bin, *this);

  std::vector<unsigned char>::reverse_iterator it = bin.rbegin(),
                                               endIt = bin.rend();

  while (it != endIt) {
    for (int i = 7; i >= 0; --i) {
      ret += (((*it >> i) & 0x1) == 0) ? '0' : '1';
    }
    ++it;
  }

  return ret;
}

inline std::string intX_t::toHexString() const {
  std::stringstream sstrm;
  sstrm << std::hex << std::right;

  std::vector<unsigned char> bin;
  toBinArray(&bin, *this);
  std::vector<unsigned char>::reverse_iterator it = bin.rbegin(),
                                               endIt = bin.rend();

  while (it != endIt) {
    sstrm << std::setw(2) << std::setfill('0') << (int)*it;
    ++it;
  }

  return sstrm.str();
}

inline std::ostream &operator<<(std::ostream &stream, const intX_t &x) {
  std::ios::fmtflags flags = stream.flags();
  if (flags & std::ios::hex) {
    if (flags & std::ios::showbase) {
      stream << "0x";
    }
    return (stream << x.toHexString());
  } else {
    return (stream << x.toString());
  }
}
}
