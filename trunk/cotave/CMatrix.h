/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_MATRIX_H_
#define _COTAVE_MATRIX_H_


#include <iostream>


namespace cotave {

//! zero tolerance
static const double COTAVE_EPSILON = 1.0e-13;

//! 固定サイズ行列
template< int ROWS, int COLS, typename DOUBLE = double >
class CMatrix {
protected:
  DOUBLE d[ ROWS * COLS ];
  
public:
  CMatrix();
  CMatrix( DOUBLE val, bool diag_flag = false );
  CMatrix( const DOUBLE *vals, bool diag_flag = false );
  CMatrix( const CMatrix< ROWS, COLS, DOUBLE > &mat );
  
  int rows() const;
  int cols() const;
  int size() const;
  
  DOUBLE* data();
  DOUBLE& elem( int i );
  DOUBLE& elem( int r, int c );

  const DOUBLE* data() const;
  const DOUBLE& elem( int i ) const;
  const DOUBLE& elem( int r, int c ) const;
  
  DOUBLE& operator[]( int i );
  DOUBLE& operator()( int i );
  DOUBLE& operator()( int r, int c );
  
  const DOUBLE& operator[]( int i ) const;
  const DOUBLE& operator()( int i ) const;
  const DOUBLE& operator()( int r, int c ) const;
  
  void fill( DOUBLE val, bool diag_flag = false );
  void fill( const DOUBLE *vals, bool diag_flag = false );
  void fill( const CMatrix< ROWS, COLS, DOUBLE > &mat );

  template< int RW, int CL >
  CMatrix< ROWS, COLS, DOUBLE >& insert( const CMatrix< RW, CL, DOUBLE > &mat, int r, int c );
  
  CMatrix< 1, COLS, DOUBLE > row( int r ) const;
  CMatrix< ROWS, 1, DOUBLE > col( int c ) const;
  
  CMatrix< COLS, ROWS, DOUBLE > transpose() const;
  
  DOUBLE absolute() const;
  DOUBLE absolute2() const;
  CMatrix< ROWS, COLS, DOUBLE >& normalize();
  
  DOUBLE trace() const;
  CMatrix< COLS, ROWS, DOUBLE > inverse() const;
  CMatrix< COLS, ROWS, DOUBLE > pseudo_inverse() const;
  CMatrix< COLS, ROWS, DOUBLE > sr_inverse( double k ) const;

  
  CMatrix< ROWS, COLS, DOUBLE >& operator=( const DOUBLE *vals );
  CMatrix< ROWS, COLS, DOUBLE >& operator=( const CMatrix< ROWS, COLS, DOUBLE > &mat );
  CMatrix< ROWS, COLS, DOUBLE >& operator+=( const CMatrix< ROWS, COLS, DOUBLE > &mat );
  CMatrix< ROWS, COLS, DOUBLE >& operator-=( const CMatrix< ROWS, COLS, DOUBLE > &mat );
  CMatrix< ROWS, COLS, DOUBLE >& operator*=( DOUBLE d );
  CMatrix< ROWS, COLS, DOUBLE >& operator/=( DOUBLE d );
  bool operator==( const CMatrix< ROWS, COLS, DOUBLE >& a ) const;
  bool operator!=( const CMatrix< ROWS, COLS, DOUBLE >& a ) const;
};

template< int ROWS, int COLS, typename DOUBLE >
CMatrix< ROWS, COLS, DOUBLE > operator+( const CMatrix< ROWS, COLS, DOUBLE > &a, const CMatrix< ROWS, COLS, DOUBLE > &b );

template< int ROWS, int COLS, typename DOUBLE >
CMatrix< ROWS, COLS, DOUBLE > operator-( const CMatrix< ROWS, COLS, DOUBLE > &a, const CMatrix< ROWS, COLS, DOUBLE > &b );

template< int ROWS, int COLS, typename DOUBLE >
CMatrix< ROWS, COLS, DOUBLE > operator-( const CMatrix< ROWS, COLS, DOUBLE > &a );

template< int ROWS, int COLS, int MED, typename DOUBLE >
CMatrix< ROWS, COLS, DOUBLE > operator*( const CMatrix< ROWS, MED, DOUBLE > &a, const CMatrix< MED, COLS, DOUBLE > &b );

template< int ROWS, int COLS, typename DOUBLE >
CMatrix< ROWS, COLS, DOUBLE > operator*( const CMatrix< ROWS, COLS, DOUBLE > &a, DOUBLE b );

template< int ROWS, int COLS, typename DOUBLE >
CMatrix< ROWS, COLS, DOUBLE > operator*( DOUBLE b, const CMatrix< ROWS, COLS, DOUBLE > &a );

template< int ROWS, int COLS, typename DOUBLE >
CMatrix< ROWS, COLS, DOUBLE > operator/( const CMatrix< ROWS, COLS, DOUBLE > &a, DOUBLE b );

template< int ROWS, int COLS, typename DOUBLE >
std::istream& operator>>( std::istream &stream, const CMatrix< ROWS, COLS, DOUBLE > &a );

template< int ROWS, int COLS, typename DOUBLE >
std::ostream& operator<<( std::ostream &stream, const CMatrix< ROWS, COLS, DOUBLE > &a );

template< int COLS, typename DOUBLE = double >
class CRowVector: public CMatrix< 1, COLS, DOUBLE > {
public:
  CRowVector();
  CRowVector( DOUBLE val );
  CRowVector( const DOUBLE *vals );
  CRowVector( const CMatrix< 1, COLS, DOUBLE > &mat );
};

template< int ROWS, typename DOUBLE = double >
class CColumnVector: public CMatrix< ROWS, 1, DOUBLE > {
public:
  CColumnVector();
  CColumnVector( DOUBLE val );
  CColumnVector( const DOUBLE *vals );
  CColumnVector( const CMatrix< ROWS, 1, DOUBLE > &mat );
};


class IntVector3: public CColumnVector< 3, int > {
public:
  IntVector3();
  IntVector3( int val );
  IntVector3( const int *vals );
  IntVector3( const CMatrix< 3, 1, int > &vec );
  IntVector3( const CMatrix< 1, 3, int > &vec );
  IntVector3( int r0, int r1, int r2 );
};

}

#include "CMatrix.ipp"

#endif
