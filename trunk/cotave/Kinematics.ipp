/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cmath>

#include "SVD.h"

namespace cotave {


// RowVector3 ___________________________

inline
RowVector3::RowVector3() {}

inline
RowVector3::RowVector3( double val ) 
  : CRowVector< 3, double >( val ) {}

inline
RowVector3::RowVector3( const double *vals )
  : CRowVector< 3, double >( vals ) {}

inline
RowVector3::RowVector3( const CMatrix< 1, 3, double > &vec )
  : CRowVector< 3, double >( vec ){};

inline
RowVector3::RowVector3( double c0, double c1, double c2 ) {
  elem( 0 ) = c0; elem( 1 ) = c1; elem( 2 ) = c2;
}

inline
double RowVector3::innerProduct( const RowVector3 &vec ) const {
  return ( elem( 0 ) * vec( 0 ) + elem( 1 ) * vec( 1 ) + elem( 2 ) * vec( 2 ) );
}


// ColumnVector3 ___________________________

inline
ColumnVector3::ColumnVector3() {}

inline
ColumnVector3::ColumnVector3( double val ) 
  : CColumnVector< 3, double >( val ) {}

inline
ColumnVector3::ColumnVector3( const double *vals )
  : CColumnVector< 3, double >( vals ) {}

inline
ColumnVector3::ColumnVector3( const CMatrix< 3, 1, double > &vec )
  : CColumnVector< 3, double >( vec ){};

inline
ColumnVector3::ColumnVector3( double r0, double r1, double r2 ) {
  elem( 0 ) = r0;
  elem( 1 ) = r1;
  elem( 2 ) = r2;
}

inline
double ColumnVector3::innerProduct( const ColumnVector3 &vec ) const {
  return ( elem( 0 ) * vec( 0 ) + elem( 1 ) * vec( 1 ) + elem( 2 ) * vec( 2 ) );
}

inline
ColumnVector3 ColumnVector3::outerProduct( const ColumnVector3 &vec ) const {
  return ColumnVector3( elem( 1 ) * vec( 2 ) - elem( 2 ) * vec( 1 ),
			elem( 2 ) * vec( 0 ) - elem( 0 ) * vec( 2 ),
			elem( 0 ) * vec( 1 ) - elem( 1 ) * vec( 0 ) );
}

inline
RotationMatrix ColumnVector3::outerProduct() const {
  return RotationMatrix( 0.0, -elem( 2 ), elem( 1 ),
			 elem( 2 ), 0.0, -elem( 0 ),
			 -elem( 1 ), elem( 0 ), 0.0 );
}

inline
ColumnVector3& ColumnVector3::rotate( const RotationMatrix &R ) {
  return ( (*this) = R * (*this) );
}

inline
ColumnVector3& ColumnVector3::rotate( const Quaternion &q ) {
  return ( (*this) = ( q * Quaternion( 0, (*this) ) * q.conjugation() ).vector() );
}


// RotationMatrix ___________________________

inline
const RotationMatrix& RotationMatrix::E() {
  static const RotationMatrix ret( 1, 0, 0,
                                   0, 1, 0,
                                   0, 0, 1 );
  return ret;
}

inline
RotationMatrix::RotationMatrix() {}

inline
RotationMatrix::RotationMatrix( const double *vals )
  : CMatrix< 3, 3, double >( vals ) {}

inline
RotationMatrix::RotationMatrix( const CMatrix< 3, 3, double > &mat )
  : CMatrix< 3, 3, double >( mat ) {}

inline
RotationMatrix::RotationMatrix( double r00, double r01, double r02,
                                double r10, double r11, double r12,
                                double r20, double r21, double r22 ) {
  elem( 0 ) = r00; elem( 1 ) = r01; elem( 2 ) = r02;
  elem( 3 ) = r10; elem( 4 ) = r11; elem( 5 ) = r12;
  elem( 6 ) = r20; elem( 7 ) = r21; elem( 8 ) = r22;
}

inline
RotationMatrix::RotationMatrix( const ColumnVector3 &axis, double angle ) {
  convertFromAxisAngle( axis, angle );
}

inline
RotationMatrix::RotationMatrix( const Quaternion &q )
  : CMatrix< 3, 3, double >( q.rotationMatrix() ) {}

inline
RotationMatrix::RotationMatrix( double yaw, double pitch, double roll ) {
  convertFromEulerAngles( yaw, pitch, roll );
}

inline
RotationMatrix& RotationMatrix::optimize() {
  SVD< 3, 3, double > svd( *this );
  return ( *this = svd.U() * svd.V().transpose() );
}

inline
RotationMatrix& RotationMatrix::orthonormalize() {
  ColumnVector3
    x0( col( 0 ) ),
    x1( col( 1 ) ),
    x2( col( 2 ) );
  
  x0.normalize();
  x1 = x1 - x1.innerProduct( x0 ) * x0;
  x1.normalize();
  x2 = x2 - x2.innerProduct( x0 ) * x0 - x2.innerProduct( x1 ) * x1;
  x2.normalize();
  
  insert( x0, 0, 0 );
  insert( x1, 0, 1 );
  insert( x2, 0, 2 );
  
  return (*this);
}

inline
RowVector3 RotationMatrix::row( int r ) const { 
  return RowVector3( elem( r, 0 ), elem( r, 1 ), elem( r, 2 ) );
}

inline
ColumnVector3 RotationMatrix::col( int c ) const {
  return ColumnVector3( elem( 0, c ), elem( 1, c ), elem( 2, c ) );
}

inline
Quaternion RotationMatrix::quaternion() const {
  double s = 0.5 / sqrt( trace() + 1.0 );
  return Quaternion( 0.25 / s, 
		     ( elem( 2, 1 ) - elem( 1, 2 ) ) * s,
		     ( elem( 0, 2 ) - elem( 2, 0 ) ) * s,
		     ( elem( 1, 0 ) - elem( 0, 1 ) ) * s );
}

inline
RotationMatrix& RotationMatrix::convertFromAxisAngle( const ColumnVector3 &axis, double angle ) {
  double
    c = cos( angle ),
    s = sin( angle ),
    t = 1.0 - c;
  
  elem( 0, 0 ) = c + axis( 0 ) * axis( 0 ) * t;
  elem( 1, 1 ) = c + axis( 1 ) * axis( 1 ) * t;
  elem( 2, 2 ) = c + axis( 2 ) * axis( 2 ) * t;
  
  double 
    tmp1 = axis( 0 ) * axis( 1 ) * t,
    tmp2 = axis( 2 ) * s;
  
  elem( 1, 0 ) = tmp1 + tmp2;
  elem( 0, 1 ) = tmp1 - tmp2;
  
  tmp1 = axis( 0 ) * axis( 2 ) * t;
  tmp2 = axis( 1 ) * s;

  elem( 2, 0 ) = tmp1 - tmp2;
  elem( 0, 2 ) = tmp1 + tmp2;

  tmp1 = axis( 1 ) * axis( 2 ) * t;
  tmp2 = axis( 0 ) * s;
  
  elem( 2, 1 ) = tmp1 + tmp2;
  elem( 1, 2 ) = tmp1 - tmp2;

  return (*this);
}

inline
RotationMatrix& RotationMatrix::convertFromEulerAngles( double yaw, double pitch, double roll ) {
  double
    sr = sin( roll  ), cr = cos( roll  ),
    sp = sin( pitch ), cp = cos( pitch ),
    sy = sin( yaw   ), cy = cos( yaw   );
  
  elem( 0, 0 ) = cy * cp;
  elem( 0, 1 ) = -sy * cr + cy * sp * sr;
  elem( 0, 2 ) = sy * sr + cy * sp * cr;
  elem( 1, 0 ) = sy * cp;
  elem( 1, 1 ) = cy * cr + sy * sp * sr;
  elem( 1, 2 ) = -cy * sr + sy * sp * cr;
  elem( 2, 0 ) = -sp;
  elem( 2, 1 ) = cp * sr;
  elem( 2, 2 ) = cp * cr;

  return (*this);
}

inline
void RotationMatrix::convertToAxisAngle( ColumnVector3 &axis, double &angle ) const {
  axis( 0 ) = elem( 2, 1 ) - elem( 1, 2 );
  axis( 1 ) = elem( 0, 2 ) - elem( 2, 0 );
  axis( 2 ) = elem( 1, 0 ) - elem( 0, 1 );
  
  //! 特異点の場合.
  if ( fabs( axis( 0 ) ) < COTAVE_EPSILON &&
       fabs( axis( 1 ) ) < COTAVE_EPSILON &&
       fabs( axis( 2 ) ) < COTAVE_EPSILON ) {
    if ( fabs( elem( 0, 1 ) + elem( 1, 0 ) ) < 0.1 &&
         fabs( elem( 0, 2 ) + elem( 2, 0 ) ) < 0.1 &&
         fabs( elem( 1, 2 ) + elem( 2, 1 ) ) < 0.1 &&
         fabs( trace() ) < 3.1 ) { 
      angle = axis( 1 ) = axis( 2 ) =0.0;
      axis( 0 ) = 1.0;
      return;
    }
    angle = M_PI;
    axis( 0 ) = ( elem( 0, 0 ) + 1.0 ) * 0.5;
    axis( 0 ) = ( axis( 0 ) > 0.0 )? sqrt( axis( 0 ) ): 0.0;
    axis( 1 ) = ( elem( 1, 1 ) + 1.0 ) * 0.5;
    axis( 1 ) = ( axis( 1 ) > 0.0 )? sqrt( axis( 1 ) ): 0.0;
    axis( 2 ) = ( elem( 2, 2 ) + 1.0 ) * 0.5;
    axis( 2 ) = ( axis( 2 ) > 0.0 )? sqrt( axis( 2 ) ): 0.0;
    
    bool
      xZero = ( fabs( axis( 0 ) ) < COTAVE_EPSILON ),
      yZero = ( fabs( axis( 1 ) ) < COTAVE_EPSILON ),
      zZero = ( fabs( axis( 2 ) ) < COTAVE_EPSILON ),
      xyPositive = ( elem( 0, 1 ) > 0.0 ),
      xzPositive = ( elem( 0, 2 ) > 0.0 ),
      yzPositive = ( elem( 1, 2 ) > 0.0 );
    
    if ( xZero && !yZero && !zZero ) {
      if ( !yzPositive ) {
        axis( 1 ) *= -1.0;
      }
    } 
    else if ( yZero && !zZero ) {
      if ( !xzPositive ) {
        axis( 2 ) *= -1.0;
      }
    } 
    else if ( zZero ) {
      if ( !xyPositive ) {
        axis( 0 ) *= -1.0;
      }
    }
    return;
  }
  
  axis.normalize();
  
  angle = acos( ( trace() - 1.0 ) * 0.5 );
}

inline
void RotationMatrix::convertToEulerAngles( double &yaw, double &pitch, double &roll ) const {
  if ( elem( 1, 0 ) == 0.0 && 
       elem( 0, 0 ) == 0.0 ) {
    yaw = 0.0;
    pitch = atan2( -elem( 2, 0 ), sqrt( elem( 2, 1 ) * elem( 2, 1 ) + elem( 2, 2 ) * elem( 2, 2 ) ) );
    roll = atan2( elem( 0, 1 ), elem( 0, 2 ) );
  }
  else {
    yaw = atan2( elem( 1, 0 ), elem( 0, 0 ) );
    if ( yaw <= -M_PI ) {
      yaw += M_PI;
    }
    if ( yaw > M_PI ) {
      yaw -= M_PI;
    }
    double
      ca = cos( yaw ),
      sa = sin( yaw );
    pitch = atan2( -elem( 2, 0 ), elem( 0, 0 ) * ca + elem( 1, 0 ) * sa );
    roll = atan2( elem( 0, 2 ) * sa - elem( 1, 2 ) * ca, -elem( 0, 1 ) * sa + elem( 1, 1 ) * ca );
  }
}

inline
RotationMatrix RotationMatrix::derivative( const ColumnVector3 &omega ) const {
  return omega.outerProduct() * (*this);
}

inline
ColumnVector3 RotationMatrix::omega( const RotationMatrix &derivative ) const {
  RotationMatrix tmp( derivative * this->transpose() );
  return ColumnVector3( tmp( 2, 1 ), tmp( 0, 2 ), tmp( 1, 0 ) );
}

inline
ColumnVector3 RotationMatrix::rotate( const ColumnVector3 &vec ) const {
  return (*this) * vec;
}

inline
double Matrix33::determinant() const {
  return
    elem( 0 ) * ( elem( 4 ) * elem( 8 ) - elem( 5 ) * elem( 7 ) )
    + elem( 1 ) * ( - elem( 3 ) * elem( 8 ) + elem( 5 ) * elem( 6 ) )
    + elem( 2 ) * ( elem( 3 ) * elem( 7 ) - elem( 4 ) * elem( 6 ) );
}

inline
Matrix33 Matrix33::inverse( double det ) const {
  Matrix33 ret;
  if ( det == 0.0 ) {
    det = this->determinant();
  }
  
  if ( det != 0.0 ) {
    ret( 0 ) = ( elem( 4 ) * elem( 8 ) - elem( 5 ) * elem( 7 ) ) / det;
    ret( 1 ) = ( elem( 2 ) * elem( 7 ) - elem( 1 ) * elem( 8 ) ) / det;
    ret( 2 ) = ( elem( 1 ) * elem( 5 ) - elem( 2 ) * elem( 4 ) ) / det;

    ret( 3 ) = ( elem( 5 ) * elem( 6 ) - elem( 3 ) * elem( 8 ) ) / det;
    ret( 4 ) = ( elem( 0 ) * elem( 8 ) - elem( 2 ) * elem( 6 ) ) / det;
    ret( 5 ) = ( elem( 2 ) * elem( 3 ) - elem( 0 ) * elem( 5 ) ) / det;

    ret( 6 ) = ( elem( 3 ) * elem( 7 ) - elem( 4 ) * elem( 6 ) ) / det;
    ret( 7 ) = ( elem( 1 ) * elem( 6 ) - elem( 0 ) * elem( 7 ) ) / det;
    ret( 8 ) = ( elem( 0 ) * elem( 4 ) - elem( 1 ) * elem( 3 ) ) / det;
  }
  return ret;
}

inline
Matrix33 Matrix33::cholesky_decomposition( bool transpose_flag ) const {
  Matrix33 ret;
  if ( !transpose_flag ){
    ret( 0 ) = sqrt( elem( 0 ) );
    ret( 1 ) = 0.0;
    ret( 2 ) = 0.0;
    
    ret( 3 ) = elem( 3 ) / ret( 0 );
    ret( 4 ) = sqrt( elem( 4 ) - ret( 3 ) * ret( 3 ) );
    ret( 5 ) = 0.0;
    
    ret( 6 ) = elem( 6 ) / ret( 0 );
    ret( 7 ) = ( elem( 7 ) - ret( 6 ) * ret( 3 ) ) / ret( 4 );
    ret( 8 ) = sqrt( elem( 8 ) - ret( 6 ) * ret( 6 ) - ret( 7 ) * ret( 7 ) );
  }
  else {
    ret( 0 ) = sqrt( elem( 0 ) );
    ret( 1 ) = elem( 3 ) / ret( 0 );
    ret( 2 ) = elem( 6 ) / ret( 0 );

    ret( 3 ) = 0.0;
    ret( 4 ) = sqrt( elem( 4 ) - ret( 1 ) * ret( 1 ) );
    ret( 5 ) = ( elem( 7 ) - ret( 2 ) * ret( 1 ) ) / ret( 4 );
    
    ret( 6 ) = 0.0;
    ret( 7 ) = 0.0;
    ret( 8 ) = sqrt( elem( 8 ) - ret( 2 ) * ret( 2 ) - ret( 5 ) * ret( 5 ) );
  }
  return ret;
}



// Quaternion ___________________________

inline
Quaternion::Quaternion() {
  elem( 0 ) = 1.0; 
  elem( 1 ) = 0.0;
  elem( 2 ) = 0.0;
  elem( 3 ) = 0.0;
}

inline
Quaternion::Quaternion( const double *vals ) 
  : CColumnVector< 4, double >( vals ) {}

inline
Quaternion::Quaternion( const CMatrix< 4, 1, double > &vec ) 
  : CColumnVector< 4, double >( vec ) {}

inline
Quaternion::Quaternion( double q0, double q1, double q2, double q3 ) {
  elem( 0 ) = q0;
  elem( 1 ) = q1;
  elem( 2 ) = q2;
  elem( 3 ) = q3;
}

inline
Quaternion::Quaternion( double q0, const CMatrix< 3, 1, double > &vec ) {
  elem( 0 ) = q0;
  elem( 1 ) = vec( 0 );
  elem( 2 ) = vec( 1 );
  elem( 3 ) = vec( 2 );
}

inline
Quaternion::Quaternion( const ColumnVector3 &axis, double angle ) {
  convertFromAxisAngle( axis, angle );
}

inline
Quaternion::Quaternion( const ColumnVector3 &rodrigues ) {
  double tmp
    = elem( 0 ) = 1.0 / sqrt( 1.0 + rodrigues.absolute2() );
  elem( 1 ) = tmp * rodrigues.elem( 0 );
  elem( 2 ) = tmp * rodrigues.elem( 1 );
  elem( 3 ) = tmp * rodrigues.elem( 2 );
}

inline
Quaternion::Quaternion( const RotationMatrix &rot ) 
  : CColumnVector< 4, double >( rot.quaternion() ) {}

inline
double Quaternion::scalar() const {
  return elem( 0 );
}

inline
ColumnVector3 Quaternion::vector() const {
  return ColumnVector3( elem( 1 ), elem( 2 ), elem( 3 ) );
}

inline
Quaternion Quaternion::conjugation() const {
  return Quaternion( elem( 0 ), -elem( 1 ), -elem( 2 ), -elem( 3 ) );
}

inline
Quaternion& Quaternion::operator*=( const Quaternion &a ) {
  return (*this) = (*this) * a;
}

inline
CMatrix< 4, 4, double > Quaternion::leftProduct() const {
  CMatrix< 4, 4, double > ret;
  ret( 0, 0 ) =  elem( 0 ); ret( 0, 1 ) = -elem( 1 ); ret( 0, 2 ) = -elem( 2 ); ret( 0, 3 ) = -elem( 3 );
  ret( 1, 0 ) =  elem( 1 ); ret( 1, 1 ) =  elem( 0 ); ret( 1, 2 ) = -elem( 3 ); ret( 1, 3 ) =  elem( 2 );
  ret( 2, 0 ) =  elem( 2 ); ret( 2, 1 ) =  elem( 3 ); ret( 2, 2 ) =  elem( 0 ); ret( 2, 3 ) = -elem( 1 );
  ret( 3, 0 ) =  elem( 3 ); ret( 3, 1 ) = -elem( 2 ); ret( 3, 2 ) =  elem( 1 ); ret( 3, 3 ) =  elem( 0 );
  return ret;
}

inline
CMatrix< 4, 4, double > Quaternion::rightProduct() const {
  CMatrix< 4, 4, double > ret;
  ret( 0, 0 ) =  elem( 0 ); ret( 0, 1 ) = -elem( 1 ); ret( 0, 2 ) = -elem( 2 ); ret( 0, 3 ) = -elem( 3 );
  ret( 1, 0 ) =  elem( 1 ); ret( 1, 1 ) =  elem( 0 ); ret( 1, 2 ) =  elem( 3 ); ret( 1, 3 ) = -elem( 2 );
  ret( 2, 0 ) =  elem( 2 ); ret( 2, 1 ) = -elem( 3 ); ret( 2, 2 ) =  elem( 0 ); ret( 2, 3 ) =  elem( 1 );
  ret( 3, 0 ) =  elem( 3 ); ret( 3, 1 ) =  elem( 2 ); ret( 3, 2 ) = -elem( 1 ); ret( 3, 3 ) =  elem( 0 );
  return ret;
}

inline
RotationMatrix Quaternion::rotationMatrix() const {
  double
    xx = elem( 1 ) * elem( 1 ),
    yy = elem( 2 ) * elem( 2 ),
    zz = elem( 3 ) * elem( 3 ),
    xy = elem( 1 ) * elem( 2 ),
    yz = elem( 2 ) * elem( 3 ),
    xz = elem( 1 ) * elem( 3 ),
    wx = elem( 0 ) * elem( 1 ),
    wy = elem( 0 ) * elem( 2 ),
    wz = elem( 0 ) * elem( 3 );
  return RotationMatrix( 1.0 - 2.0 * ( yy + zz ), 2.0 * ( xy - wz ), 2.0 * ( xz + wy ),
			 2.0 * ( xy + wz ), 1.0 - 2.0 * ( xx + zz ), 2.0 * ( yz - wx ),
			 2.0 * ( xz - wy ), 2.0 * ( yz + wx ), 1.0 - 2.0 * ( xx + yy ) );
}

inline
ColumnVector3 Quaternion::rodriguesParameters() const {
  return ColumnVector3( elem( 1 ) / elem( 0 ), elem( 2 ) / elem( 0 ), elem( 3 ) / elem( 0 ) );
}

inline
Quaternion& Quaternion::convertFromAxisAngle( const ColumnVector3 &axis, double angle ) {
  double sina_den = sin( angle * 0.5 ) / axis.absolute();
  elem( 0 ) = cos( angle * 0.5 );
  elem( 1 ) = axis( 0 ) * sina_den;
  elem( 2 ) = axis( 1 ) * sina_den;
  elem( 3 ) = axis( 2 ) * sina_den;
  return (*this);
}

inline
void Quaternion::convertToAxisAngle( ColumnVector3 &axis, double &angle ) const {
  angle = acos( elem( 0 ) );
  axis = ColumnVector3( elem( 1 ), elem( 2 ), elem( 3 ) ) / sin( angle );
  angle *= 2.0;
}

inline
Quaternion Quaternion::derivative( const ColumnVector3 &omega ) const {
  return 0.5 * Quaternion( 0.0, omega ) * (*this);
}

inline
ColumnVector3 Quaternion::omega( const Quaternion &derivative ) const {
  return 2.0 * ( derivative * this->conjugation() ).vector();
}

inline
Quaternion operator*( const Quaternion &a, const Quaternion &b ){
  return Quaternion( a( 0 ) * b( 0 ) - a( 1 ) * b( 1 ) - a( 2 ) * b( 2 ) - a( 3 ) * b( 3 ),
		     a( 0 ) * b( 1 ) + a( 1 ) * b( 0 ) + a( 2 ) * b( 3 ) - a( 3 ) * b( 2 ),
		     a( 0 ) * b( 2 ) + a( 2 ) * b( 0 ) - a( 1 ) * b( 3 ) + a( 3 ) * b( 1 ),
		     a( 0 ) * b( 3 ) + a( 3 ) * b( 0 ) + a( 1 ) * b( 2 ) - a( 2 ) * b( 1 ) );
}

inline
ColumnVector3 Quaternion::rotate( const ColumnVector3 &vec ) const {
  return ( (*this) * Quaternion( 0, vec ) * this->conjugation() ).vector();
}


}
