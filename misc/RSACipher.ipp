/**
 * $Id: RSACipher.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <sys/time.h>
}

#include <cstdlib>

namespace cutil {

namespace {

inline intX_t LCM(const intX_t &a, const intX_t &b) {
  //! Extended Euclid's Algorithm
  intX_t x1(1), y1(0), z1(a), x2(0), y2(1), z2(b), zero(0);

  while (z2 != zero) {
    intX_t tmp(z1 / z2), x3(x1 - tmp * x2), y3(y1 - tmp * y2),
        z3(z1 - tmp * z2);
    x1 = x2;
    y1 = y2;
    z1 = z2;
    x2 = x3;
    y2 = y3;
    z2 = z3;
  }

  return (a * b) / z1;
}

inline intX_t POWMOD(const intX_t &a, const intX_t &b, const intX_t &m) {
  intX_t l(1), i(a), j(b), one(1), two(2), tmp;

  while (j > one) {
    tmp = j / two;

    if ((j - tmp * two) == one) {
      l = l * i;
    }

    j = tmp;

    i = (i * i) % m;
    l %= m;
  }

  return (i * l) % m;
}

inline bool FERMATCHECK(const intX_t &x) {
  intX_t zero(0);
  if (x % intX_t(3) == zero || x % intX_t(5) == zero || x % intX_t(7) == zero ||
      x % intX_t(11) == zero || x % intX_t(13) == zero ||
      x % intX_t(17) == zero) {
    return false;
  }
  return (POWMOD(intX_t(2), x - intX_t(1), x) == intX_t(1));
}
}

inline RSAKey::RSAKey() {}

inline RSAKey::RSAKey(const intX_t &n, const intX_t &ed) : n(n), ed(ed) {}

inline void RSAKey::setKey(const intX_t &n, const intX_t &ed) {
  this->n = n;
  this->ed = ed;
}

inline void RSAKey::generate(intX_t *n, intX_t *e, intX_t *d,
                             int security_bits) {
  {  // seed of rand()
    timeval rand_tv;
    ::gettimeofday(&rand_tv, NULL);
    ::srand(rand_tv.tv_sec ^ rand_tv.tv_usec);
  }

  intX_t p(1), q(1);  // prime numbers

  while (!FERMATCHECK(p)) {
    p = 1;

    for (int i = 1; i < security_bits / 2 - 1; ++i) {
      p <<= 1;
      p += (::rand() / ((double)RAND_MAX + 1.0) < 0.5) ? intX_t(0) : intX_t(1);
    }
    p <<= 1;
    ++p;

    //    cout << "p: " << p << endl;
  }
  while (!FERMATCHECK(q)) {
    q = 1;
    for (int i = 1; i < security_bits / 2 - 1; ++i) {
      q <<= 1;
      q += (::rand() / ((double)RAND_MAX + 1.0) < 0.5) ? intX_t(0) : intX_t(1);
    }
    q <<= 1;
    ++q;

    //    cout << "q: " << q << endl;
  }

  *n = p * q;

  intX_t L(LCM(p - intX_t(1), q - intX_t(1)));

  *e = (L % intX_t(11) == intX_t(0)) ? intX_t(13) : intX_t(11);

  //! Extended Euclid's Algorithm
  intX_t x1(1), y1(0), z1(L), x2(0), y2(1), z2(*e);

  while (z2 != intX_t(0)) {
    intX_t tmp(z1 / z2), x3(x1 - tmp * x2), y3(y1 - tmp * y2),
        z3(z1 - tmp * z2);
    x1 = x2;
    y1 = y2;
    z1 = z2;
    x2 = x3;
    y2 = y3;
    z2 = z3;
  }

  *d = y1;
  while (d < 0) {
    *d += L;
  }
}

inline intX_t RSACipher::cipher(const intX_t &X, const RSAKey &K) {
  return POWMOD(X, K.ed, K.n);
}

inline intX_t RSACipher::encrypt(const intX_t &M, const RSAKey &K) {
  return cipher(M, K);
}

inline intX_t RSACipher::decrypt(const intX_t &C, const RSAKey &K) {
  return cipher(C, K);
}
}
