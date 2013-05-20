/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cotave {

Slerp::Slerp( const ColumnVector3 &d1, const ColumnVector3 &d2 )
  : base( d1 ),
    angle_coeff( ::acos( d1.innerProduct( d2 ) ) * 0.5 ),
    vector_coeff( d1.outerProduct( d2 ).normalize() ) {}

ColumnVector3 Slerp::operator()( double t ) const {
  return Quaternion( ::cos( angle_coeff * t ), ::sin( angle_coeff * t ) * vector_coeff ).rotate( base );
}

}
