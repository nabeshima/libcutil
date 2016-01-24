/**
 * $Id: CMatrix.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cmath>

#include "SVD.h"

namespace cotave {

// CMatrix ___________________________

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE>::CMatrix() {}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE>::CMatrix(DOUBLE val, bool diag_flag) {
  fill(val, diag_flag);
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE>::CMatrix(const DOUBLE *vals,
                                            bool diag_flag) {
  fill(vals, diag_flag);
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE>::CMatrix(
    const CMatrix<ROWS, COLS, DOUBLE> &mat) {
  fill(mat);
}

template <int ROWS, int COLS, typename DOUBLE>
inline int CMatrix<ROWS, COLS, DOUBLE>::rows() const {
  return ROWS;
}

template <int ROWS, int COLS, typename DOUBLE>
inline int CMatrix<ROWS, COLS, DOUBLE>::cols() const {
  return COLS;
}

template <int ROWS, int COLS, typename DOUBLE>
inline int CMatrix<ROWS, COLS, DOUBLE>::size() const {
  return ROWS * COLS;
}

template <int ROWS, int COLS, typename DOUBLE>
inline DOUBLE *CMatrix<ROWS, COLS, DOUBLE>::data() {
  return d;
}

template <int ROWS, int COLS, typename DOUBLE>
inline DOUBLE &CMatrix<ROWS, COLS, DOUBLE>::elem(int i) {
  return d[i];
}

template <int ROWS, int COLS, typename DOUBLE>
inline DOUBLE &CMatrix<ROWS, COLS, DOUBLE>::elem(int r, int c) {
  return d[r * COLS + c];
}

template <int ROWS, int COLS, typename DOUBLE>
inline const DOUBLE *CMatrix<ROWS, COLS, DOUBLE>::data() const {
  return d;
}

template <int ROWS, int COLS, typename DOUBLE>
inline const DOUBLE &CMatrix<ROWS, COLS, DOUBLE>::elem(int i) const {
  return d[i];
}

template <int ROWS, int COLS, typename DOUBLE>
inline const DOUBLE &CMatrix<ROWS, COLS, DOUBLE>::elem(int r, int c) const {
  return d[r * COLS + c];
}

template <int ROWS, int COLS, typename DOUBLE>
inline DOUBLE &CMatrix<ROWS, COLS, DOUBLE>::operator[](int i) {
  return elem(i);
}

template <int ROWS, int COLS, typename DOUBLE>
inline DOUBLE &CMatrix<ROWS, COLS, DOUBLE>::operator()(int i) {
  return elem(i);
}

template <int ROWS, int COLS, typename DOUBLE>
inline DOUBLE &CMatrix<ROWS, COLS, DOUBLE>::operator()(int r, int c) {
  return elem(r, c);
}

template <int ROWS, int COLS, typename DOUBLE>
inline const DOUBLE &CMatrix<ROWS, COLS, DOUBLE>::operator[](int i) const {
  return elem(i);
}

template <int ROWS, int COLS, typename DOUBLE>
inline const DOUBLE &CMatrix<ROWS, COLS, DOUBLE>::operator()(int i) const {
  return elem(i);
}

template <int ROWS, int COLS, typename DOUBLE>
inline const DOUBLE &CMatrix<ROWS, COLS, DOUBLE>::operator()(int r,
                                                             int c) const {
  return elem(r, c);
}

template <int ROWS, int COLS, typename DOUBLE>
inline void CMatrix<ROWS, COLS, DOUBLE>::fill(DOUBLE val, bool diag_flag) {
  if (diag_flag) {
    for (int i = 0; i < ROWS * COLS; ++i) {
      elem(i) = 0;
    }

    const int min = (ROWS < COLS) ? ROWS : COLS;
    for (int i = 0; i < min; ++i) {
      elem(i, i) = val;
    }
  } else {
    for (int i = 0; i < ROWS * COLS; ++i) {
      elem(i) = val;
    }
  }
}

template <int ROWS, int COLS, typename DOUBLE>
inline void CMatrix<ROWS, COLS, DOUBLE>::fill(const DOUBLE *vals,
                                              bool diag_flag) {
  if (diag_flag) {
    for (int i = 0; i < ROWS * COLS; ++i) {
      elem(i) = 0;
    }

    const int min = (ROWS < COLS) ? ROWS : COLS;
    for (int i = 0; i < min; ++i) {
      elem(i, i) = vals[i];
    }
  } else {
    for (int i = 0; i < ROWS * COLS; ++i) {
      elem(i) = vals[i];
    }
  }
}

template <int ROWS, int COLS, typename DOUBLE>
inline void CMatrix<ROWS, COLS, DOUBLE>::fill(
    const CMatrix<ROWS, COLS, DOUBLE> &mat) {
  for (int i = 0; i < ROWS * COLS; ++i) {
    elem(i) = mat(i);
  }
}

template <int ROWS, int COLS, typename DOUBLE>
template <int RW, int CL>
inline CMatrix<ROWS, COLS, DOUBLE> &CMatrix<ROWS, COLS, DOUBLE>::insert(
    const CMatrix<RW, CL, DOUBLE> &mat, int r, int c) {
  for (int rr = 0; rr < RW; ++rr) {
    for (int cc = 0; cc < CL; ++cc) {
      elem(rr + r, cc + c) = mat(rr, cc);
    }
  }
  return (*this);
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<1, COLS, DOUBLE> CMatrix<ROWS, COLS, DOUBLE>::row(int r) const {
  CMatrix<1, COLS, DOUBLE> ret;
  for (int c = 0; c < COLS; ++c) {
    ret(c) = elem(r, c);
  }
  return ret;
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, 1, DOUBLE> CMatrix<ROWS, COLS, DOUBLE>::col(int c) const {
  CMatrix<ROWS, 1, DOUBLE> ret;
  for (int r = 0; r < ROWS; ++r) {
    ret(r) = elem(r, c);
  }
  return ret;
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<COLS, ROWS, DOUBLE> CMatrix<ROWS, COLS, DOUBLE>::transpose()
    const {
  CMatrix<COLS, ROWS, DOUBLE> ret;
  for (int r = 0; r < ROWS; ++r) {
    for (int c = 0; c < COLS; ++c) {
      ret(c, r) = elem(r, c);
    }
  }
  return ret;
}

template <int ROWS, int COLS, typename DOUBLE>
inline DOUBLE CMatrix<ROWS, COLS, DOUBLE>::absolute2() const {
  DOUBLE sum = 0;
  for (int i = 0; i < ROWS * COLS; ++i) {
    sum += elem(i) * elem(i);
  }
  return sum;
}

template <int ROWS, int COLS, typename DOUBLE>
inline DOUBLE CMatrix<ROWS, COLS, DOUBLE>::absolute() const {
  return (DOUBLE)sqrt(absolute2());
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> &CMatrix<ROWS, COLS, DOUBLE>::normalize() {
  return ((*this) /= absolute());
}

template <int ROWS, int COLS, typename DOUBLE>
inline DOUBLE CMatrix<ROWS, COLS, DOUBLE>::trace() const {
  DOUBLE sum = 0;
  const int min = (ROWS < COLS) ? ROWS : COLS;
  for (int i = 0; i < min; ++i) {
    sum += elem(i, i);
  }
  return sum;
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<COLS, ROWS, DOUBLE> CMatrix<ROWS, COLS, DOUBLE>::inverse()
    const {
  return pseudo_inverse();
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<COLS, ROWS, DOUBLE> CMatrix<ROWS, COLS, DOUBLE>::pseudo_inverse()
    const {
  SVD<ROWS, COLS, DOUBLE> svd(*this);

  const int min = (ROWS < COLS) ? ROWS : COLS;
  const CMatrix<ROWS, COLS, DOUBLE> &S = svd.S();

  CMatrix<COLS, ROWS, DOUBLE> Si(0.0);
  for (int i = 0; i < min; ++i) {
    if (fabs(S(i, i)) > COTAVE_EPSILON) {
      Si(i, i) = 1.0 / S(i, i);
    }
  }
  return svd.V() * Si * svd.U().transpose();
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<COLS, ROWS, DOUBLE> CMatrix<ROWS, COLS, DOUBLE>::sr_inverse(
    double k) const {
  if (COLS < ROWS) {
    return this->transpose() *
           ((*this) * this->transpose() + CMatrix<ROWS, ROWS, DOUBLE>(k, true))
               .inverse();
  } else {
    return (CMatrix<COLS, COLS, DOUBLE>(k, true) + this->transpose() * (*this))
               .inverse() *
           this->transpose();
  }
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> &CMatrix<ROWS, COLS, DOUBLE>::operator=(
    const DOUBLE *vals) {
  for (int i = 0; i < ROWS * COLS; ++i) {
    elem(i) = vals[i];
  }
  return (*this);
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> &CMatrix<ROWS, COLS, DOUBLE>::operator=(
    const CMatrix<ROWS, COLS, DOUBLE> &mat) {
  for (int i = 0; i < ROWS * COLS; ++i) {
    elem(i) = mat(i);
  }
  return (*this);
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> &CMatrix<ROWS, COLS, DOUBLE>::operator+=(
    const CMatrix<ROWS, COLS, DOUBLE> &mat) {
  for (int i = 0; i < ROWS * COLS; ++i) {
    elem(i) += mat(i);
  }
  return (*this);
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> &CMatrix<ROWS, COLS, DOUBLE>::operator-=(
    const CMatrix<ROWS, COLS, DOUBLE> &mat) {
  for (int i = 0; i < ROWS * COLS; ++i) {
    elem(i) -= mat(i);
  }
  return (*this);
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> &CMatrix<ROWS, COLS, DOUBLE>::operator*=(
    DOUBLE d) {
  for (int i = 0; i < ROWS * COLS; ++i) {
    elem(i) *= d;
  }
  return (*this);
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> &CMatrix<ROWS, COLS, DOUBLE>::operator/=(
    DOUBLE d) {
  for (int i = 0; i < ROWS * COLS; ++i) {
    elem(i) /= d;
  }
  return (*this);
}

template <int ROWS, int COLS, typename DOUBLE>
inline bool CMatrix<ROWS, COLS, DOUBLE>::operator==(
    const CMatrix<ROWS, COLS, DOUBLE> &a) const {
  for (int i = 0; i < ROWS * COLS; ++i) {
    if (elem(i) != a(i)) {
      return false;
    }
  }
  return true;
}

template <int ROWS, int COLS, typename DOUBLE>
inline bool CMatrix<ROWS, COLS, DOUBLE>::operator!=(
    const CMatrix<ROWS, COLS, DOUBLE> &a) const {
  return !((*this) == a);
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> operator+(
    const CMatrix<ROWS, COLS, DOUBLE> &a,
    const CMatrix<ROWS, COLS, DOUBLE> &b) {
  CMatrix<ROWS, COLS, DOUBLE> ret;
  for (int i = 0; i < ROWS * COLS; ++i) {
    ret(i) = a(i) + b(i);
  }
  return ret;
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> operator-(
    const CMatrix<ROWS, COLS, DOUBLE> &a,
    const CMatrix<ROWS, COLS, DOUBLE> &b) {
  CMatrix<ROWS, COLS, DOUBLE> ret;
  for (int i = 0; i < ROWS * COLS; ++i) {
    ret(i) = a(i) - b(i);
  }
  return ret;
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> operator-(
    const CMatrix<ROWS, COLS, DOUBLE> &a) {
  CMatrix<ROWS, COLS, DOUBLE> ret;
  for (int i = 0; i < ROWS * COLS; ++i) {
    ret(i) = -a(i);
  }
  return ret;
}

template <int ROWS, int COLS, int MED, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> operator*(
    const CMatrix<ROWS, MED, DOUBLE> &a, const CMatrix<MED, COLS, DOUBLE> &b) {
  CMatrix<ROWS, COLS, DOUBLE> ret(0.0);
  for (int r = 0; r < ROWS; ++r) {
    for (int c = 0; c < COLS; ++c) {
      for (int m = 0; m < MED; ++m) {
        ret(r, c) += a(r, m) * b(m, c);
      }
    }
  }
  return ret;
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> operator*(
    const CMatrix<ROWS, COLS, DOUBLE> &a, DOUBLE b) {
  CMatrix<ROWS, COLS, DOUBLE> ret;
  for (int i = 0; i < ROWS * COLS; ++i) {
    ret(i) = a(i) * b;
  }
  return ret;
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> operator*(
    DOUBLE b, const CMatrix<ROWS, COLS, DOUBLE> &a) {
  return a * b;
}

template <int ROWS, int COLS, typename DOUBLE>
inline CMatrix<ROWS, COLS, DOUBLE> operator/(
    const CMatrix<ROWS, COLS, DOUBLE> &a, DOUBLE b) {
  CMatrix<ROWS, COLS, DOUBLE> ret;
  for (int i = 0; i < ROWS * COLS; ++i) {
    ret(i) = a(i) / b;
  }
  return ret;
}

template <int ROWS, int COLS, typename DOUBLE>
inline std::istream &operator>>(std::istream &stream,
                                const CMatrix<ROWS, COLS, DOUBLE> &a) {
  for (int i = 0; i < ROWS * COLS; ++i) {
    stream >> (DOUBLE &)a(i);
  }
  return stream;
}

template <int ROWS, int COLS, typename DOUBLE>
inline std::ostream &operator<<(std::ostream &stream,
                                const CMatrix<ROWS, COLS, DOUBLE> &a) {
  for (int r = 0; r < ROWS - 1; ++r) {
    for (int c = 0; c < COLS - 1; ++c) {
      stream << (const DOUBLE &)a(r, c) << " ";
    }
    { stream << (const DOUBLE &)a(r, COLS - 1) << std::endl; }
  }
  {
    for (int c = 0; c < COLS - 1; ++c) {
      stream << (const DOUBLE &)a(ROWS - 1, c) << " ";
    }
    { stream << (const DOUBLE &)a(ROWS - 1, COLS - 1); }
  }
  return stream;
}

// CRowVector ___________________________

template <int COLS, typename DOUBLE>
inline CRowVector<COLS, DOUBLE>::CRowVector() {}

template <int COLS, typename DOUBLE>
inline CRowVector<COLS, DOUBLE>::CRowVector(DOUBLE val)
    : CMatrix<1, COLS, DOUBLE>(val) {}

template <int COLS, typename DOUBLE>
inline CRowVector<COLS, DOUBLE>::CRowVector(const DOUBLE *vals)
    : CMatrix<1, COLS, DOUBLE>(vals) {}

template <int COLS, typename DOUBLE>
inline CRowVector<COLS, DOUBLE>::CRowVector(const CMatrix<1, COLS, DOUBLE> &mat)
    : CMatrix<1, COLS, DOUBLE>(mat) {}

// CColumnVector ___________________________

template <int ROWS, typename DOUBLE>
inline CColumnVector<ROWS, DOUBLE>::CColumnVector() {}

template <int ROWS, typename DOUBLE>
inline CColumnVector<ROWS, DOUBLE>::CColumnVector(DOUBLE val)
    : CMatrix<ROWS, 1, DOUBLE>(val) {}

template <int ROWS, typename DOUBLE>
inline CColumnVector<ROWS, DOUBLE>::CColumnVector(const DOUBLE *vals)
    : CMatrix<ROWS, 1, DOUBLE>(vals) {}

template <int ROWS, typename DOUBLE>
inline CColumnVector<ROWS, DOUBLE>::CColumnVector(
    const CMatrix<ROWS, 1, DOUBLE> &mat)
    : CMatrix<ROWS, 1, DOUBLE>(mat) {}

inline IntVector3::IntVector3() {}

inline IntVector3::IntVector3(int val) : CColumnVector<3, int>(val) {}

inline IntVector3::IntVector3(const int *vals) : CColumnVector<3, int>(vals) {}

inline IntVector3::IntVector3(const CMatrix<3, 1, int> &vec)
    : CColumnVector<3, int>(vec) {}

inline IntVector3::IntVector3(const CMatrix<1, 3, int> &vec)
    : CColumnVector<3, int>(vec.data()) {}

inline IntVector3::IntVector3(int r0, int r1, int r2) {
  elem(0) = r0;
  elem(1) = r1;
  elem(2) = r2;
}
}
