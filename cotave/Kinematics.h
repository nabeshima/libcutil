/**
 * $Id: Kinematics.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
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

class RowVector3 : public CRowVector<3, double> {
 public:
  RowVector3();
  explicit RowVector3(double val);
  explicit RowVector3(const double *vals);
  RowVector3(const CMatrix<1, 3, double> &vec);  // NOLINT
  explicit RowVector3(double c0, double c1, double c2);

  //! 内積
  double innerProduct(const RowVector3 &vec) const;
};

class ColumnVector3 : public CColumnVector<3, double> {
 public:
  ColumnVector3();
  explicit ColumnVector3(double val);
  explicit ColumnVector3(const double *vals);
  ColumnVector3(const CMatrix<3, 1, double> &vec);  // NOLINT
  explicit ColumnVector3(double r0, double r1, double r2);

  //! 内積
  double innerProduct(const ColumnVector3 &vec) const;
  //! 外積 (*this) x vec
  ColumnVector3 outerProduct(const ColumnVector3 &vec) const;
  //! 外積 (*this) x に等価なskew symmetric matrix (R.transpose() == -R)
  RotationMatrix outerProduct() const;

  //! (*this)を位置ベクトルとして, Rで回転させる.
  ColumnVector3 &rotate(const RotationMatrix &R);
  //! (*this)を位置ベクトルとして, qで回転させる.
  ColumnVector3 &rotate(const Quaternion &q);
};

//! 回転行列
class RotationMatrix : public CMatrix<3, 3, double> {
 public:
  static const RotationMatrix &E();

 public:
  RotationMatrix();
  explicit RotationMatrix(const double *vals);
  RotationMatrix(const CMatrix<3, 3, double> &mat);  // NOLINT
  RotationMatrix(double r00, double r01, double r02, double r10, double r11,
                 double r12, double r20, double r21, double r22);
  explicit RotationMatrix(const ColumnVector3 &axis, double angle);
  explicit RotationMatrix(const Quaternion &q);
  RotationMatrix(double yaw, double pitch, double roll);

  //! optimally aproximation [Challis, 1995]
  RotationMatrix &optimize();

  //! Gram-Schmidt orthonormalization
  RotationMatrix &orthonormalize();

  RowVector3 row(int r) const;
  ColumnVector3 col(int c) const;

  //! (*this)が直交行列なことを仮定した, quaternionの出力
  Quaternion quaternion() const;
  RotationMatrix &convertFromAxisAngle(const ColumnVector3 &axis, double angle);
  //! オイラー角(Z-Y-X)からの変換
  RotationMatrix &convertFromEulerAngles(double yaw, double pitch, double roll);
  void convertToAxisAngle(ColumnVector3 *axis, double *angle) const;
  //! オイラー角(Z-Y-X)からの変換
  void convertToEulerAngles(double *yaw, double *pitch, double *roll) const;

  /*!
    \brief 角速度から時間微分への変換

    このRotationMatrixが物体座標系を表す姿勢行列であるとき,
    ワールド座標系から見た角速度omegaで物体座標系が回転するときの,回転行列の時間変化量.
    ワールド座標系で記述.
  */
  RotationMatrix derivative(const ColumnVector3 &omega) const;

  /*!
    \brief 時間微分から角速度への変換

    このRotationMatrixが物体座標系を表すとき,
    ワールド座標系で記述された姿勢の時間変化量から,
    ワールド座標系から見た角速度omegaを計算.
  */
  ColumnVector3 omega(const RotationMatrix &derivative) const;

  //! vecを位置ベクトルとして, (*this)で回転させたベクトルを返す.
  ColumnVector3 rotate(const ColumnVector3 &vec) const;

  //---------------------------------------------------
  // not for rotation matrix but for general 3x3 matrix
  //! 行列式
  double determinant() const;
  //! 逆行列. determinantを事前に計算してあるときは引数に渡す.
  Matrix33 inverse(double determinant = 0.0) const;

  /*!
    \brief コレスキー分解 (cholesky decomposition).

    正定値対称行列でないとおかしい結果になる.
    分散共分散行列を渡して,多次元の正規関数を作る時に使用.
    本当はLU分解でも可だが,高速化のため.

    M --> L * L.transpose()
    ( Lは下三角行列 )
  */
  Matrix33 cholesky_decomposition(bool transpose_flag = false) const;
};

//! クォータニオン
class Quaternion : public CColumnVector<4, double> {
 public:
  Quaternion();
  explicit Quaternion(const double *vals);
  Quaternion(const CMatrix<4, 1, double> &vec);  // NOLINT
  //! 引数を要素として初期化
  Quaternion(double q0, double q1, double q2, double q3);
  //! a0をスカラ部分,vecをベクタ部分として初期化.
  Quaternion(double q0, const CMatrix<3, 1, double> &vec);
  //! 回転軸と回転角で初期化 convertFromAxisAngle()
  Quaternion(const ColumnVector3 &axis, double angle);
  //! rodriguesパラメータで初期化
  explicit Quaternion(const ColumnVector3 &rodrigues);
  //! 回転行列で初期化
  explicit Quaternion(const RotationMatrix &rot);

  //! スカラー成分
  double scalar() const;
  //! ベクトル成分
  ColumnVector3 vector() const;

  //! 共役クォータニオン. 正規化されていれば逆クォータニオン.
  Quaternion conjugation() const;

  Quaternion &operator*=(const Quaternion &a);

  //! 左からの積演算に等価なskew symmetric matrix R (R.transpose() == -R):
  //! (*this) * q = R * q
  CMatrix<4, 4, double> leftProduct() const;
  //! 右からの積演算に等価なskew symmetric matrix R' (R'.transpose() == -R'): q
  //! * (*this) = R' * q
  CMatrix<4, 4, double> rightProduct() const;

  //! このquaternionに相当する回転行列に変換.
  RotationMatrix rotationMatrix() const;

  //! このquaternionに相当するロドリゲスパラメータ. (特異点は0)
  ColumnVector3 rodriguesParameters() const;

  Quaternion &convertFromAxisAngle(const ColumnVector3 &axis, double angle);
  void convertToAxisAngle(ColumnVector3 *axis, double *angle) const;

  /*!
    \brief 角速度から時間微分への変換

    このQuaternionが物体座標系を表すとき,
    ワールド座標系から見た角速度omegaで物体座標系が回転するときの,クォータニオンの時間変化量.
    ワールド座標系で記述.
  */
  Quaternion derivative(const ColumnVector3 &omega) const;

  /*!
    \brief 時間微分から角速度への変換

    このQuaternionが物体座標系を表すとき,
    ワールド座標系で記述された物体座標系クォータニオンの時間変化量から,
    ワールド座標系から見た角速度omegaを計算.
  */
  ColumnVector3 omega(const Quaternion &derivative) const;

  //! vecを位置ベクトルとして, (*this)で回転させたベクトルを返す.
  ColumnVector3 rotate(const ColumnVector3 &vec) const;
};

Quaternion operator*(const Quaternion &a, const Quaternion &b);
}  // namespace cotave

#include "Kinematics.ipp"

#endif
