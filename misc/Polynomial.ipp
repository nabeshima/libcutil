/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <sstream>

namespace cutil {

template <typename DOUBLE>
inline void Polynomial<DOUBLE>::remove_zero() {
  while (coeff.size() > 0) {
    if (coeff.back() == 0) {
      coeff.pop_back();
    } else {
      break;
    }
  }
}

template <typename DOUBLE>
inline Polynomial<DOUBLE>::Polynomial() {}

template <typename DOUBLE>
inline Polynomial<DOUBLE>::Polynomial(const Polynomial &opp)
    : coeff(opp.coeff) {}

template <typename DOUBLE>
inline Polynomial<DOUBLE>::Polynomial(const std::vector<DOUBLE> &opp)
    : coeff(opp) {
  remove_zero();
}

template <typename DOUBLE>
inline Polynomial<DOUBLE>::Polynomial(const DOUBLE *opp, int n)
    : coeff(opp, opp + n) {
  remove_zero();
}

template <typename DOUBLE>
inline Polynomial<DOUBLE>::Polynomial(int n)
    : coeff(n, static_cast<DOUBLE>(0)) {}

template <typename DOUBLE>
inline Polynomial<DOUBLE> &Polynomial<DOUBLE>::operator=(
    const Polynomial<DOUBLE>::Polynomial &opp) {
  coeff = opp.coeff;
  return *this;
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> &Polynomial<DOUBLE>::operator=(
    const std::vector<DOUBLE> &opp) {
  coeff = opp;
  remove_zero();
  return *this;
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> &Polynomial<DOUBLE>::operator+=(
    const Polynomial<DOUBLE> &opp) {
  if (size() < opp.size()) {
    for (int i = 0; i < size(); ++i) {
      elem(i) += opp(i);
    }
    for (int i = size(); i < opp.size(); ++i) {
      coeff.push_back(opp(i));
    }
  } else {
    for (int i = 0; i < opp.size(); ++i) {
      elem(i) += opp(i);
    }
  }
  remove_zero();
  return *this;
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> &Polynomial<DOUBLE>::operator-=(
    const Polynomial<DOUBLE> &opp) {
  if (size() < opp.size()) {
    for (int i = 0; i < size(); ++i) {
      elem(i) -= opp(i);
    }
    for (int i = size(); i < opp.size(); ++i) {
      coeff.push_back(-opp(i));
    }
  } else {
    for (int i = 0; i < opp.size(); ++i) {
      elem(i) -= opp(i);
    }
  }
  remove_zero();
  return *this;
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> &Polynomial<DOUBLE>::operator*=(
    const Polynomial<DOUBLE> &opp) {
  *this = *this * opp;
  return *this;
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> &Polynomial<DOUBLE>::operator*=(DOUBLE b) {
  if (b == static_cast<DOUBLE>(0)) {
    coeff.clear();
  } else {
    for (int i = 0; i < size(); ++i) {
      elem(i) *= b;
    }
  }
  return *this;
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> &Polynomial<DOUBLE>::operator/=(DOUBLE b) {
  for (int i = 0; i < size(); ++i) {
    elem(i) /= b;
  }
  return *this;
}

template <typename DOUBLE>
inline int Polynomial<DOUBLE>::size() const {
  return coeff.size();
}

template <typename DOUBLE>
inline DOUBLE &Polynomial<DOUBLE>::elem(int i) {
  return coeff[i];
}

template <typename DOUBLE>
inline DOUBLE &Polynomial<DOUBLE>::operator[](int i) {
  return elem(i);
}

template <typename DOUBLE>
inline DOUBLE &Polynomial<DOUBLE>::operator()(int i) {
  return elem(i);
}

template <typename DOUBLE>
inline const DOUBLE &Polynomial<DOUBLE>::elem(int i) const {
  return coeff[i];
}

template <typename DOUBLE>
inline const DOUBLE &Polynomial<DOUBLE>::operator[](int i) const {
  return elem(i);
}

template <typename DOUBLE>
inline const DOUBLE &Polynomial<DOUBLE>::operator()(int i) const {
  return elem(i);
}

template <typename DOUBLE>
inline std::string Polynomial<DOUBLE>::toString() const {
  if (size() == 0) {
    return "()";
  }

  std::stringstream strm;
  strm << "( ";
  for (int i = 0; i < size(); ++i) {
    strm << elem(i) << " ";
  }
  strm << ')';
  return strm.str();
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> operator+(const Polynomial<DOUBLE> &a,
                                    const Polynomial<DOUBLE> &b) {
  if (a.size() < b.size()) {
    Polynomial<DOUBLE> ret(b);
    for (int i = 0; i < a.size(); ++i) {
      ret(i) += a(i);
    }

    ret.remove_zero();
    return ret;
  } else {
    Polynomial<DOUBLE> ret(a);
    for (int i = 0; i < b.size(); ++i) {
      ret(i) += b(i);
    }

    ret.remove_zero();
    return ret;
  }
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> operator-(const Polynomial<DOUBLE> &a,
                                    const Polynomial<DOUBLE> &b) {
  int n = (a.size() < b.size()) ? b.size() : a.size();
  Polynomial<DOUBLE> ret(n);

  for (int i = 0; i < a.size(); ++i) {
    ret(i) = a(i);
  }

  for (int i = 0; i < b.size(); ++i) {
    ret(i) -= b(i);
  }

  ret.remove_zero();
  return ret;
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> operator-(const Polynomial<DOUBLE> &a) {
  Polynomial<DOUBLE> ret(a.size());
  for (int i = 0; i < a.size(); ++i) {
    ret(i) = -a(i);
  }
  return ret;
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> operator*(const Polynomial<DOUBLE> &a,
                                    const Polynomial<DOUBLE> &b) {
  if (a.size() == 0 || b.size() == 0) {
    return Polynomial<DOUBLE>();
  } else {
    int n = b.size() + a.size() - 1;
    Polynomial<DOUBLE> ret(n);

    for (int i = 0; i < a.size(); ++i) {
      for (int j = 0; j < b.size(); ++j) {
        ret(i + j) += a(i) * b(j);
      }
    }
    return ret;
  }
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> operator*(const Polynomial<DOUBLE> &a, DOUBLE b) {
  if (b == static_cast<DOUBLE>(0) || a.size() == 0) {
    return Polynomial<DOUBLE>();
  } else {
    Polynomial<DOUBLE> ret(a.size());
    for (int i = 0; i < a.size(); ++i) {
      ret(i) = a(i) * b;
    }
    return ret;
  }
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> operator*(DOUBLE b, const Polynomial<DOUBLE> &a) {
  return a * b;
}

template <typename DOUBLE>
inline Polynomial<DOUBLE> operator/(const Polynomial<DOUBLE> &a, DOUBLE b) {
  Polynomial<DOUBLE> ret(a.size());
  for (int i = 0; i < a.size(); ++i) {
    ret(i) = a(i) / b;
  }
  return ret;
}
}
