/**
 * $Id: Slerp.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cotave {

inline Slerp::Slerp(const ColumnVector3 &d1, const ColumnVector3 &d2)
    : base(d1),
      angle_coeff(::acos(d1.innerProduct(d2)) * 0.5),
      vector_coeff(d1.outerProduct(d2).normalize()) {}

inline ColumnVector3 Slerp::operator()(double t) const {
  return Quaternion(::cos(angle_coeff * t),
                    ::sin(angle_coeff * t) * vector_coeff)
      .rotate(base);
}
}
