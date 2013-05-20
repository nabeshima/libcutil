/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cotave {

SfM::SfM() {
  reset();
}

void SfM::reset() {
  N.fill( 0.0 );
  sumL.fill( 0.0 );
  sumR.fill( 0.0 );
  absSumL = 0.0;
  absSumR = 0.0;
  num = 0;

  scl = 1.0;
  rot.fill( 0.0 ); rot( 0 ) = 1.0;
  trans.fill( 0.0 );
  err = 0.0;
}

void SfM::accumulate( const ColumnVector3 &pL, const ColumnVector3 &pR ) {
  sumL += pL;
  sumR += pR;
  absSumL += pL.absolute2();
  absSumR += pR.absolute2();
  N += 
    Quaternion( 0.0, pL ).rightProduct().transpose() * 
    Quaternion( 0.0, pR ).leftProduct();
  ++num;
}

void SfM::accumulate( const std::pair< ColumnVector3, ColumnVector3 > &p ) {
  accumulate( p.first, p.second );
}

void SfM::calculate() {
  double
    varL = absSumL - sumL.absolute2() / (double)num,
    varR = absSumR - sumR.absolute2() / (double)num;
  
  scl = ::sqrt( varR / varL );
  
  SVD< 4, 4, double > svd( N -
                           Quaternion( 0.0, sumL ).rightProduct().transpose() * 
                           Quaternion( 0.0, sumR ).leftProduct() / (double)num );
  double D = svd.S()( 0, 0 );
  rot = svd.V().col( 0 );
  
  trans = ( sumR - scl * rot.rotate( sumL ) ) / (double)num;
  
  // 数値誤差で,負になることがある.
  err = ::fabs( scl * 2.0 * ( ::sqrt( varL * varR ) - D ) );
}

double SfM::scale() const {
  return scl;
}

RotationMatrix SfM::rotation() const {
  return rot.rotationMatrix();
}

const Quaternion& SfM::quaternion() const {
  return rot;
}

const ColumnVector3& SfM::translation() const {
  return trans;
}

double SfM::error() const {
  return err;
}

}
