/**
 * $Id: SfM.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cotave {

inline SfM::SfM() { reset(); }

inline void SfM::reset() {
  N.fill(0.0);
  sumL.fill(0.0);
  sumR.fill(0.0);
  absSumL = 0.0;
  absSumR = 0.0;
  num = 0;

  scl = 1.0;
  rot.fill(0.0);
  rot(0) = 1.0;
  trans.fill(0.0);
  err = 0.0;
}

inline void SfM::accumulate(const ColumnVector3 &pL, const ColumnVector3 &pR) {
  sumL += pL;
  sumR += pR;
  absSumL += pL.absolute2();
  absSumR += pR.absolute2();
  N += Quaternion(0.0, pL).rightProduct().transpose() *
       Quaternion(0.0, pR).leftProduct();
  ++num;
}

inline void SfM::accumulate(const std::pair<ColumnVector3, ColumnVector3> &p) {
  accumulate(p.first, p.second);
}

inline void SfM::calculate() {
  double varL = absSumL - sumL.absolute2() / (double)num,
         varR = absSumR - sumR.absolute2() / (double)num;

  scl = ::sqrt(varR / varL);

  SVD<4, 4, double> svd(N -
                        Quaternion(0.0, sumL).rightProduct().transpose() *
                            Quaternion(0.0, sumR).leftProduct() / (double)num);
  double D = svd.S()(0, 0);
  rot = svd.V().col(0);

  trans = (sumR - scl * rot.rotate(sumL)) / (double)num;

  // 数値誤差で,負になることがある.
  err = ::fabs(scl * 2.0 * (::sqrt(varL * varR) - D));
}

inline double SfM::scale() const { return scl; }

inline RotationMatrix SfM::rotation() const { return rot.rotationMatrix(); }

inline const Quaternion &SfM::quaternion() const { return rot; }

inline const ColumnVector3 &SfM::translation() const { return trans; }

inline double SfM::error() const { return err; }
}
