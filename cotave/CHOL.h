/**
 * $Id: CHOL.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_CHOL_H_
#define _COTAVE_CHOL_H_

#include "CMatrix.h"

namespace cotave {

//! コレスキー分解(Cholesky decomposition). mat = L * L'
template <int DIM, typename DOUBLE = double>
class CHOL {
 protected:
  CMatrix<DIM, DIM, DOUBLE> mL;

 public:
  explicit CHOL(const CMatrix<DIM, DIM, DOUBLE>& mat);
  const CMatrix<DIM, DIM, DOUBLE>& L() const;
};

//! 修正コレスキー分解(Modified cholesky decomposition). mat = L * D * L'
template <int DIM, typename DOUBLE = double>
class CHOLm : public CHOL<DIM, DOUBLE> {
 private:
  CMatrix<DIM, DIM, DOUBLE> mD;

 public:
  explicit CHOLm(const CMatrix<DIM, DIM, DOUBLE>& mat);
  const CMatrix<DIM, DIM, DOUBLE>& D() const;
};
}  // namespace cotave

#include "CHOL.ipp"

#endif
