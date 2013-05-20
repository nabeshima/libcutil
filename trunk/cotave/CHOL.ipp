/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
  extern int dpotrf_( char *uplo,
                      int *n,
                      double *a,
                      int *lda,
                      int *info );
  
  extern int spotrf_( char *uplo,
                      int *n,
                      float *a,
                      int *lda,
                      int *info );
}


#include <typeinfo>


namespace cotave {


template< int DIM, typename DOUBLE > inline
CHOL< DIM, DOUBLE >::CHOL( const CMatrix< DIM, DIM, DOUBLE > &mat ) {
  char uplo = 'L';
  int n = DIM;
  
  mL = mat;
  DOUBLE *a = mL.data();
  
  int lda = DIM;
  int info;
  
  if ( typeid( double ) == typeid( DOUBLE ) ) {
    dpotrf_( &uplo, &n, (double*)a, &lda, &info );
  }
  else if ( typeid( float ) == typeid( DOUBLE ) ) {
    spotrf_( &uplo, &n, (float*)a, &lda, &info );
  }
  
  for ( int i = 0; i < DIM; ++i ) {
    for ( int j = i + 1; j < DIM; ++j ) {
      mL( j, i ) = mL( i, j );
      mL( i, j ) = 0.0;
    }
  }
}

template< int DIM, typename DOUBLE > inline
const CMatrix< DIM, DIM, DOUBLE >& CHOL< DIM, DOUBLE >::L() const {
  return mL;
}


template< int DIM, typename DOUBLE > inline
CHOLm< DIM, DOUBLE >::CHOLm( const CMatrix< DIM, DIM, DOUBLE > &mat )
  : CHOL< DIM, DOUBLE >( mat ), mD( 0.0 ) {
  
  for ( int i = 0; i < DIM; ++i ) {
    double d = CHOL< DIM, DOUBLE >::mL( i, i );
    
    CHOL< DIM, DOUBLE >::mL( i, i ) = 1.0;
    
    for ( int j = i + 1; j < DIM; ++j ) {
      CHOL< DIM, DOUBLE >::mL( j, i ) /= d;
    }
    
    mD( i, i ) = d * d;
  }
}

template< int DIM, typename DOUBLE > inline
const CMatrix< DIM, DIM, DOUBLE >& CHOLm< DIM, DOUBLE >::D() const {
  return mD;
}


}
