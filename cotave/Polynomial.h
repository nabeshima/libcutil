/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_POLYNOMIAL_H_
#define _CUTIL_POLYNOMIAL_H_

#include <string>
#include <vector>

namespace cotave {
template <typename DOUBLE = double>
class Polynomial {
 private:
  std::vector<DOUBLE> coeff;

 public:
  Polynomial();
  Polynomial(const Polynomial<DOUBLE> &opp);
  explicit Polynomial(const std::vector<DOUBLE> &opp);
  explicit Polynomial(const DOUBLE *opp, int n);
  explicit Polynomial(int n);
  void remove_zero();

  Polynomial<DOUBLE> &operator=(const Polynomial<DOUBLE> &opp);
  Polynomial<DOUBLE> &operator=(const std::vector<DOUBLE> &opp);

  Polynomial<DOUBLE> &operator+=(const Polynomial<DOUBLE> &opp);
  Polynomial<DOUBLE> &operator-=(const Polynomial<DOUBLE> &opp);
  Polynomial<DOUBLE> &operator*=(const Polynomial<DOUBLE> &opp);
  Polynomial<DOUBLE> &operator*=(DOUBLE b);
  Polynomial<DOUBLE> &operator/=(DOUBLE b);

  int size() const;

  DOUBLE &elem(int i);
  DOUBLE &operator[](int i);
  DOUBLE &operator()(int i);

  const DOUBLE &elem(int i) const;
  const DOUBLE &operator[](int i) const;
  const DOUBLE &operator()(int i) const;

  DOUBLE substitute(DOUBLE x) const;
  Polynomial<DOUBLE> substitute(const Polynomial<DOUBLE> &x) const;

  std::string toString() const;
};

template <typename DOUBLE>
Polynomial<DOUBLE> operator+(const Polynomial<DOUBLE> &a,
                             const Polynomial<DOUBLE> &b);

template <typename DOUBLE>
Polynomial<DOUBLE> operator-(const Polynomial<DOUBLE> &a,
                             const Polynomial<DOUBLE> &b);

template <typename DOUBLE>
Polynomial<DOUBLE> operator-(const Polynomial<DOUBLE> &a);

template <typename DOUBLE>
Polynomial<DOUBLE> operator*(const Polynomial<DOUBLE> &a,
                             const Polynomial<DOUBLE> &b);

template <typename DOUBLE>
Polynomial<DOUBLE> operator*(const Polynomial<DOUBLE> &a, DOUBLE b);

template <typename DOUBLE>
Polynomial<DOUBLE> operator*(DOUBLE b, const Polynomial<DOUBLE> &a);

template <typename DOUBLE>
Polynomial<DOUBLE> operator/(const Polynomial<DOUBLE> &a, DOUBLE b);

}  // namespace cotave

#include "Polynomial.ipp"

#endif
