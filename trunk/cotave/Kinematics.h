/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_KINEMATICS_H_
#define _COTAVE_KINEMATICS_H_


#include "CMatrix.h"


namespace cotave {

class RotationMatrix;
class Quaternion;
typedef RotationMatrix Matrix33;


class RowVector3: public CRowVector< 3, double > {
public:
  RowVector3();
  RowVector3( double val );
  RowVector3( const double *vals );
  RowVector3( const CMatrix< 1, 3, double > &vec );
  RowVector3( double c0, double c1, double c2 );
  
  //! ����
  double innerProduct( const RowVector3 &vec ) const;
};

class ColumnVector3: public CColumnVector< 3, double > {
public:
  ColumnVector3();
  ColumnVector3( double val );
  ColumnVector3( const double *vals );
  ColumnVector3( const CMatrix< 3, 1, double > &vec );
  ColumnVector3( double r0, double r1, double r2 );
  
  //! ����
  double innerProduct( const ColumnVector3 &vec ) const;
  //! ���� (*this) x vec
  ColumnVector3 outerProduct( const ColumnVector3 &vec ) const;
  //! ���� (*this) x ��������skew symmetric matrix (R.transpose() == -R)
  RotationMatrix outerProduct() const;
  
  //! (*this)����֥٥��ȥ�Ȥ���, R�ǲ�ž������.
  ColumnVector3& rotate( const RotationMatrix &R );
  //! (*this)����֥٥��ȥ�Ȥ���, q�ǲ�ž������.
  ColumnVector3& rotate( const Quaternion &q );
};

//! ��ž����
class RotationMatrix: public CMatrix< 3, 3, double > {
public:
  static const RotationMatrix& E();
  
public:
  RotationMatrix();
  RotationMatrix( const double *vals );
  RotationMatrix( const CMatrix< 3, 3, double > &mat );
  RotationMatrix( double r00, double r01, double r02,
                  double r10, double r11, double r12,
                  double r20, double r21, double r22 );
  RotationMatrix( const ColumnVector3 &axis, double angle );
  RotationMatrix( const Quaternion &q );
  RotationMatrix( double yaw, double pitch, double roll );

  //! optimally aproximation [Challis, 1995]
  RotationMatrix& optimize();
  
  //! Gram-Schmidt orthonormalization   
  RotationMatrix& orthonormalize();
  
  RowVector3 row( int r ) const;
  ColumnVector3 col( int c ) const;
  
  //! (*this)��ľ�����ʤ��Ȥ��ꤷ��, quaternion�ν���
  Quaternion quaternion() const;
  RotationMatrix& convertFromAxisAngle( const ColumnVector3 &axis, double angle );
  //! �����顼��(Z-Y-X)������Ѵ�
  RotationMatrix& convertFromEulerAngles( double yaw, double pitch, double roll );  
  void convertToAxisAngle( ColumnVector3 &axis, double &angle ) const;
  //! �����顼��(Z-Y-X)������Ѵ�
  void convertToEulerAngles( double &yaw, double &pitch, double &roll ) const;
  
  /*!
    \brief ��®�٤��������ʬ�ؤ��Ѵ�
    
    ����RotationMatrix��ʪ�κ�ɸ�Ϥ�ɽ����������Ǥ���Ȥ�,
    ���ɺ�ɸ�Ϥ��鸫����®��omega��ʪ�κ�ɸ�Ϥ���ž����Ȥ���,��ž����λ����Ѳ���.
    ���ɺ�ɸ�Ϥǵ���.
  */
  RotationMatrix derivative( const ColumnVector3 &omega ) const;
  
  /*!
    \brief ������ʬ�����®�٤ؤ��Ѵ�
    
    ����RotationMatrix��ʪ�κ�ɸ�Ϥ�ɽ���Ȥ�,
    ���ɺ�ɸ�Ϥǵ��Ҥ��줿�����λ����Ѳ��̤���,
    ���ɺ�ɸ�Ϥ��鸫����®��omega��׻�.
  */  
  ColumnVector3 omega( const RotationMatrix &derivative ) const;

  //! vec����֥٥��ȥ�Ȥ���, (*this)�ǲ�ž�������٥��ȥ���֤�.
  ColumnVector3 rotate( const ColumnVector3 &vec ) const;
  
  
  //---------------------------------------------------
  // not for rotation matrix but for general 3x3 matrix
  //! ����
  double determinant() const;
  //! �չ���. determinant������˷׻����Ƥ���Ȥ��ϰ������Ϥ�.       
  Matrix33 inverse( double determinant = 0.0 ) const;
  
  /*!
    \brief ���쥹����ʬ�� (cholesky decomposition). 
    
    �������оι���Ǥʤ��Ȥ���������̤ˤʤ�. 
    ʬ����ʬ��������Ϥ���,¿�����������ؿ�������˻���.
    ������LUʬ��Ǥ�Ĥ���,��®���Τ���.
    
    M --> L * L.transpose()
    ( L�ϲ����ѹ��� )
  */
  Matrix33 cholesky_decomposition( bool transpose_flag = false ) const;
};


//! ���������˥���
class Quaternion: public CColumnVector< 4, double > {
public:
  Quaternion();
  Quaternion( const double *vals );
  Quaternion( const CMatrix< 4, 1, double > &vec );
  //! ���������ǤȤ��ƽ����
  Quaternion( double q0, double q1, double q2, double q3 );
  //! a0�򥹥�����ʬ,vec��٥�����ʬ�Ȥ��ƽ����.
  Quaternion( double q0, const CMatrix< 3, 1, double > &vec );  
  //! ��ž���Ȳ�ž�Ѥǽ���� convertFromAxisAngle()
  Quaternion( const ColumnVector3 &axis, double angle );
  //! rodrigues�ѥ�᡼���ǽ����
  Quaternion( const ColumnVector3 &rodrigues );
  //! ��ž����ǽ����
  Quaternion( const RotationMatrix &rot );
  
  //! �����顼��ʬ
  double scalar() const;
  //! �٥��ȥ���ʬ
  ColumnVector3 vector() const;
  
  //! ���򥯥������˥���. ����������Ƥ���еե��������˥���.
  Quaternion conjugation() const;
  
  Quaternion& operator*=( const Quaternion &a );

  //! ��������ѱ黻��������skew symmetric matrix R (R.transpose() == -R): (*this) * q = R * q
  CMatrix< 4, 4, double > leftProduct() const;
  //! ��������ѱ黻��������skew symmetric matrix R' (R'.transpose() == -R'): q * (*this) = R' * q
  CMatrix< 4, 4, double > rightProduct() const;
  
  //! ����quaternion�����������ž������Ѵ�.
  RotationMatrix rotationMatrix() const;

  //! ����quaternion�����������ɥꥲ���ѥ�᡼��. (�ð�����0)
  ColumnVector3 rodriguesParameters() const;
  
  Quaternion& convertFromAxisAngle( const ColumnVector3 &axis, double angle );
  void convertToAxisAngle( ColumnVector3 &axis, double &angle ) const;
  
    
  /*!
    \brief ��®�٤��������ʬ�ؤ��Ѵ�
    
    ����Quaternion��ʪ�κ�ɸ�Ϥ�ɽ���Ȥ�,
    ���ɺ�ɸ�Ϥ��鸫����®��omega��ʪ�κ�ɸ�Ϥ���ž����Ȥ���,���������˥���λ����Ѳ���.
    ���ɺ�ɸ�Ϥǵ���.
  */
  Quaternion derivative( const ColumnVector3 &omega ) const;
  
  /*!
    \brief ������ʬ�����®�٤ؤ��Ѵ�
    
    ����Quaternion��ʪ�κ�ɸ�Ϥ�ɽ���Ȥ�,
    ���ɺ�ɸ�Ϥǵ��Ҥ��줿ʪ�κ�ɸ�ϥ��������˥���λ����Ѳ��̤���,
    ���ɺ�ɸ�Ϥ��鸫����®��omega��׻�.
  */  
  ColumnVector3 omega( const Quaternion &derivative ) const;

  //! vec����֥٥��ȥ�Ȥ���, (*this)�ǲ�ž�������٥��ȥ���֤�.
  ColumnVector3 rotate( const ColumnVector3 &vec ) const;
};

Quaternion operator*( const Quaternion &a, const Quaternion &b );


}

#include "Kinematics.ipp"

#endif

