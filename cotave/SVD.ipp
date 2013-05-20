/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
  extern int dgesdd_( char *jobz,
                      int *m,
                      int *n,
                      double *a,
                      int *lda,
                      double *s,
                      double *u,
                      int *ldu,
                      double *vt,
                      int *ldvt,
                      double *work,
                      int *lwork,
                      int *iwork,
                      int *info );
  
  extern int sgesdd_( char *jobz,
                      int *m,
                      int *n,
                      float *a,
                      int *lda,
                      float *s,
                      float *u,
                      int *ldu,
                      float *vt,
                      int *ldvt,
                      float *work,
                      int *lwork,
                      int *iwork,
                      int *info );
}


#include <typeinfo>


namespace cotave {


template< int ROWS, int COLS, typename DOUBLE > inline
SVD< ROWS, COLS, DOUBLE >::SVD( const CMatrix< ROWS, COLS, DOUBLE > &mat ) {
  
  char jobz = 'A';
  int
    m = ROWS,
    n = COLS;

  CMatrix< COLS, ROWS, DOUBLE > At( mat.transpose() );
  CColumnVector< ( ROWS < COLS )? ROWS: COLS, DOUBLE > ms;
  
  DOUBLE *a = At.data();
  int
    lda = ROWS;
  
  DOUBLE *s = ms.data();
  DOUBLE *u = mU.data();
  DOUBLE *v = mV.data();
  int
    ldu = ROWS,
    ldvt = COLS;  

  int
    min = ( ROWS < COLS )? ROWS: COLS,
    max = ( COLS > ROWS )? COLS: ROWS,
    max2 =  4 * min * min + 4 * COLS,
    max3 = ( max > max2 )? max: max2;
  
  int
    lwork = 3 * min * min + max3;
  DOUBLE work[ lwork ];
  int iwork[ 8 * min ];
  int info;
  
  if ( typeid( double ) == typeid( DOUBLE ) ) {
    dgesdd_( &jobz, &m, &n, (double*)a, &lda, (double*)s, (double*)u, &ldu, (double*)v, &ldvt, (double*)work, &lwork, iwork, &info );
  }
  else if ( typeid( float ) == typeid( DOUBLE ) ) {
    sgesdd_( &jobz, &m, &n, (float*)a, &lda, (float*)s, (float*)u, &ldu, (float*)v, &ldvt, (float*)work, &lwork, iwork, &info );
  }
  
  mS.fill( ms.data(), true );
  mU = mU.transpose();
}

template< int ROWS, int COLS, typename DOUBLE > inline
const CMatrix< ROWS, ROWS, DOUBLE >& SVD< ROWS, COLS, DOUBLE >::U() const {
  return mU;
}

template< int ROWS, int COLS, typename DOUBLE > inline
const CMatrix< ROWS, COLS, DOUBLE >& SVD< ROWS, COLS, DOUBLE >::S() const {
  return mS;
}

template< int ROWS, int COLS, typename DOUBLE > inline
const CMatrix< COLS, COLS, DOUBLE >& SVD< ROWS, COLS, DOUBLE >::V() const {
  return mV;
}

}
