/**
 * $Id: SfM.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_SfM_H_
#define _COTAVE_SfM_H_

#include <utility>

#include "Kinematics.h"

namespace cotave {

/*!
  \brief SfM:Structure from Motion.

  対応点を渡して,運動を計算する.
  [B.K.P. Horn, 1987]
  RANSACなどはクラスの外で行う.
*/
class SfM {
 private:
  CMatrix<4, 4, double> N;
  ColumnVector3 sumL;
  ColumnVector3 sumR;
  double absSumL;
  double absSumR;
  int num;

  double scl;
  Quaternion rot;
  ColumnVector3 trans;
  double err;

 public:
  SfM();

  // data push
  void reset();
  void accumulate(const ColumnVector3 &pL, const ColumnVector3 &pR);
  void accumulate(const std::pair<ColumnVector3, ColumnVector3> &p);
  void calculate();

  // result
  double scale() const;
  RotationMatrix rotation() const;
  const Quaternion &quaternion() const;
  const ColumnVector3 &translation() const;
  double error() const;
};
}  // namespace cotave

#include "SfM.ipp"

#endif
