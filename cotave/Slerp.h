/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_Slerp_H_
#define _COTAVE_Slerp_H_


#include "Kinematics.h"


namespace cotave {

/*!
  \brief 球面線形補間(Spherical linear interpolation)
  
  同じ絶対値の(方向)ベクトルを球面線形補間する.
*/
class Slerp {
private:
  ColumnVector3 base;
  
  double angle_coeff;
  ColumnVector3 vector_coeff;
public:
  Slerp( const ColumnVector3 &d1, const ColumnVector3 &d2 );
  ColumnVector3 operator()( double t ) const;
};

}


#include "Slerp.ipp"


#endif
