/**
 * $Id: cotave_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cotave.h>

#include <iostream>
#include <utility>
#include <vector>

using namespace std;
using namespace cotave;

class Gradient {
 public:
  ColumnVector3 accel;  // 加速度

  CColumnVector<6> operator()(const CColumnVector<6> &y, double time) const {
    CColumnVector<6> ret;
    ret(0) = y(3 + 0);  // 位置のgradient = 速度
    ret(1) = y(3 + 1);
    ret(2) = y(3 + 2);
    ret(3) = accel(0);  // 速度のgradient = 加速度
    ret(4) = accel(1);
    ret(5) = accel(2);
    return ret;
  }
};

class F {
 public:
  CColumnVector<2> operator()(const CColumnVector<2> &x) const {
    CColumnVector<2> ret;
    ret(0) = x(0) + sin(x(1));
    ret(1) = x(1) + 0.1;
    return ret;
  }
};

class H {
 public:
  CColumnVector<2> operator()(const CColumnVector<2> &x) const {
    CColumnVector<2> ret;
    ret(0) = x(0) + x(1);
    ret(1) = x(1) * x(1);
    return ret;
  }
};

class ERROR {
 public:
  double operator()(const SfM &sfm,
                    const pair<ColumnVector3, ColumnVector3> &data) const {
    return (data.second -
            (sfm.scale() * sfm.rotation() * data.first + sfm.translation()))
        .absolute2();
  }
};

int main() {
  double d[] = {1,  2,  3,  4,  5,  6,  7,  8,  9,  10,
                -1, -2, -3, -4, -5, -6, -7, -8, -9, -10};
  CMatrix<2, 10, double> a(d);
  CMatrix<10, 3, double> b(0.58);

  cout << "a" << endl << a << endl;
  cout << "b" << endl << b << endl;
  cout << "a * 1.4" << endl << a * 1.4 << endl;
  cout << "a * b" << endl << a * b << endl;
  cout << "a / 1.7" << endl << a / 1.7 << endl;
  cout << "a + a * 3" << endl << a + a * 3.0 << endl;
  cout << "a.transpose()" << endl << a.transpose() << endl;
  cout << "a.absolute()" << endl << a.absolute() << endl;
  cout << "a.absolute2()" << endl << a.absolute2() << endl;
  cout << "a.normalize()" << endl << a.normalize() << endl;
  cout << "a" << endl << a << endl;

  double e[] = {14, 12, -90, 3};
  CMatrix<2, 2, double> c(e);
  cout << "a.insert( c, 0, 0 )" << endl << a.insert(c, 0, 0) << endl;
  cout << "a.insert( c, 0, 7 )" << endl << a.insert(c, 0, 7) << endl;

  cout << "a.trace()" << endl << a.trace() << endl;

  cout << "a *= 3" << endl << (a *= 3.0) << endl;
  cout << "a /= 5" << endl << (a /= 5) << endl;
  cout << "a == a" << endl << (a == a) << endl;
  cout << "a != a" << endl << (a != a) << endl;

  double v[] = {4, 2, 1, 5, 6, 9, 1, 3, 7, 5};
  CColumnVector<sizeof(v) / sizeof(v[0])> cv(v);
  cout << "cv" << endl << cv << endl;
  cout << "a * cv" << endl << a * cv << endl;

  CRowVector<sizeof(v) / sizeof(v[0])> rv(v);
  cout << "rv" << endl << cv << endl;
  cout << "rv * cv" << endl << rv * cv << endl;
  cout << "cv * rv" << endl << cv * rv << endl;

  RowVector3 r(1.0, 0.0, 1.1);
  cout << r << endl;

  cout << endl << endl << "Rotations" << endl;

  double yaw = M_PI / 4.0, roll = 0.0, pitch = 0.0;
  RotationMatrix ypr;
  ypr.convertFromEulerAngles(yaw, pitch, roll);
  cout << ":: " << yaw << " " << pitch << " " << roll << endl << endl;
  cout << ypr << endl;
  ypr.convertToEulerAngles(&yaw, &pitch, &roll);
  cout << ":: " << yaw << " " << pitch << " " << roll << endl << endl;

  cout << endl << ypr << endl;
  cout << endl << RotationMatrix(ypr.quaternion()) << endl;

  ColumnVector3 pos(1.0, 0.5, 1.2);
  cout << pos.transpose() << endl;
  cout << ypr.rotate(pos).transpose() << endl;
  cout << pos.transpose() << endl;
  cout << ypr.quaternion().rotate(pos).transpose() << endl;

  cout << Quaternion(pos.normalize(), 1.5).transpose() << endl;
  cout << RotationMatrix(pos.normalize(), 1.5).quaternion().transpose() << endl
       << endl;

  cout << (RotationMatrix(pos.normalize(), 1.5) * pos).transpose() << endl;
  cout << (RotationMatrix(pos.normalize(), 1.5).rotate(pos)).transpose()
       << endl;
  cout << (Quaternion(pos.normalize(), 1.5).rotate(pos)).transpose() << endl;

  Matrix33 mat(1, 3, 4, 6, 3, 6, 1, 0, 9);
  cout << mat << endl << endl;
  cout << mat.inverse() << endl << endl;
  cout << mat.inverse() * mat << endl << endl;
  cout << mat * mat.inverse() << endl << endl;

  mat = Matrix33(324, 1, 3, 1, 34, 5, 3, 5, 51);

  cout << mat.cholesky_decomposition(true) << endl << endl;
  cout << mat.cholesky_decomposition() << endl << endl;
  cout << mat.cholesky_decomposition() * mat.cholesky_decomposition(true)
       << endl
       << endl;
  cout << mat.cholesky_decomposition(true) * mat.cholesky_decomposition()
       << endl
       << endl;
  double det = mat.determinant();
  cout << det << endl;
  cout << mat.inverse(det) << endl << endl;
  cout << mat.inverse() << endl << endl;
  cout << mat.inverse(det) * mat << endl << endl;
  cout << mat.inverse() * mat << endl << endl;
  cout << mat * mat.inverse(det) << endl << endl;
  cout << mat * mat.inverse() << endl << endl;

  mat = Matrix33(324, 1, 3, 1, 34, 5, 3, 5, 51);

  CMatrix<5, 3> mat123;
  for (int i = 0; i < mat123.size(); ++i) {
    mat123(i) = i * i + i;
  }

  SVD<5, 3> svd(mat123);

  cout << "U" << endl << svd.U() << endl << endl;
  cout << "S" << endl << svd.S() << endl << endl;
  cout << "V" << endl << svd.V() << endl << endl;

  cout << "U * S * Vt" << endl
       << svd.U() * svd.S() * svd.V().transpose() << endl
       << endl;
  cout << "mat123" << endl << mat123 << endl << endl;

  CMatrix<5, 3> mat1234;
  for (int i = 0; i < mat1234.size(); ++i) {
    mat1234(i) = i * i + i;
  }

  cout << "pinv( mat1234 )" << endl << mat1234.pseudo_inverse() << endl << endl;

  cout << "mat123 * pinv( mat123 )" << endl
       << mat123 * mat123.pseudo_inverse() << endl
       << endl;

  cout << "pinv( mat123 ) * mat123" << endl
       << mat123.pseudo_inverse() * mat123 << endl
       << endl;

  cout << "V * V'" << endl << svd.V() * svd.V().transpose() << endl << endl;
  cout << "U * U'" << endl << svd.U() * svd.U().transpose() << endl << endl;

  cout << "pinv( mat123 )" << endl << mat123.pseudo_inverse() << endl << endl;

  cout << "mat123 * pinv( mat123 )" << endl
       << mat123 * mat123.pseudo_inverse() << endl
       << endl;

  cout << "pinv( mat123 ) * mat123" << endl
       << mat123.pseudo_inverse() * mat123 << endl
       << endl;

  cout << "inv( mat123 ) * mat123" << endl
       << mat123.inverse() * mat123 << endl
       << endl;

  cout << "sinv( mat123 ) * mat123" << endl
       << mat123.sr_inverse(0.02) * mat123 << endl
       << endl;

  cout << &RotationMatrix::E() << endl;
  cout << "sinv( mat123 ) * mat123" << endl
       << mat123.sr_inverse(0.02) * mat123 << endl
       << endl;
  cout << &RotationMatrix::E() << endl;

  {
    RotationMatrix rot(0.9, 0.2, 0, 0, 0.8, -0.8, 0.9, 0.8, 0.8);
    cout << "rot " << endl << rot << endl << endl;
    rot.optimize();
    cout << "rot.optimize() " << endl << rot << endl << endl;
    Quaternion q1(rot);

    rot = RotationMatrix(0.1, 0, -1, 0, 1, 0.8, -0.1, -0.8, -0.8);
    cout << "rot " << endl << rot << endl << endl;
    rot.optimize();
    cout << "rot.optimize() " << endl << rot << endl << endl;
    Quaternion q2(rot);

    cout << "q1 * q2 " << endl << q1 * q2 << endl << endl;
    cout << "q2 * q1 " << endl << q2 * q1 << endl << endl;
    cout << "q1.leftProduct() * q2 " << endl
         << q1.leftProduct() * q2 << endl
         << endl;
    cout << "q2.rightProduct() * q1 " << endl
         << q2.rightProduct() * q1 << endl
         << endl;
    cout << "q2.leftProduct() * q1 " << endl
         << q2.leftProduct() * q1 << endl
         << endl;
    cout << "q1.rightProduct() * q2 " << endl
         << q1.rightProduct() * q2 << endl
         << endl;
  }

  {
    ColumnVector3 d1(1, 1, 1), d2(0, -1, 1);

    d1.normalize() *= 2.0;
    d2.normalize() *= 2.0;

    cout << "d1 " << endl << d1.transpose() << endl;
    cout << "d2 " << endl << d2.transpose() << endl;

    Slerp slerp(d1, d2);
    double t = 0.0;

    while (t <= 1.0) {
      cout << t << ":: " << slerp(t).transpose() << endl;
      t += 0.1;
    }
    cout << endl;
  }

  {
    Gradient grad;
    grad.accel.fill(0.0);
    grad.accel(2) = -9.8;

    double time = 0.0;
    double time_step = 5.0;
    CColumnVector<6> posvel(0.0);

    while (time <= 10.0) {
      cout << time << " " << posvel.transpose() << endl;
      posvel = RungeKutta::step(grad, posvel, time, time_step);
      time += time_step;
    }
    cout << endl;
  }

  {
    RotationMatrix R(0.0);
    R += ColumnVector3(-1, 1, 0) * ColumnVector3(-1, 1, 0).transpose();
    R += ColumnVector3(1, 1, 0) * ColumnVector3(1, 1, 0).transpose();
    R += ColumnVector3(1, -0.1, 0.9) * ColumnVector3(1, -0.1, 0.9).transpose();
    R += ColumnVector3(0, -0.1, 18) * ColumnVector3(0, -0.1, 18).transpose();

    CHOL<3> chol(R);
    cout << " CHOL R" << endl << R << endl << endl;
    cout << " CHOL L" << endl << chol.L() << endl << endl;
    cout << " CHOL L*L'" << endl
         << chol.L() * chol.L().transpose() << endl
         << endl;

    CHOLm<3> cholm(R);
    cout << " CHOLm L" << endl << cholm.L() << endl << endl;
    cout << " CHOLm D'" << endl << cholm.D() << endl << endl;
    cout << " CHOLm L*D*L'" << endl
         << cholm.L() * cholm.D() * cholm.L().transpose() << endl
         << endl;
  }

  {
    CColumnVector<2> x;
    x(0) = 1;
    x(1) = 0.1;

    CMatrix<2, 2> P0;
    P0(0, 0) = 0.1;
    P0(0, 1) = -0.001;
    P0(1, 0) = -0.001;
    P0(1, 1) = 0.1;

    CColumnVector<2> Q;
    Q(0) = 0.01;
    Q(1) = 0.01;

    CColumnVector<2> R;
    R(0) = 0.01;
    R(1) = 0.1;

    UKF<2, 2> ukf(x, P0);

    double time = 0.0;
    double time_step = 0.1;

    F f;
    H h;

    CColumnVector<2> y;

    while (time <= 5.0) {
      y = h(x);
      ukf.update(f, h, Q, R, y);

      cout << time << ";\t " << x.transpose() << ";\t " << ukf.x().transpose()
           << endl;

      time += time_step;
      x = f(x);
    }
    cout << endl;
  }

  {
    double scale = 0.1244;
    RotationMatrix rot(1.0, 2.0, -0.5);
    ColumnVector3 trans(2.3, -0.9, 0.1e6);

    cout << "true scale " << scale << endl;
    cout << "true rotation " << endl << rot << endl;
    cout << "true translation " << trans.transpose() << endl << endl;

    SfM sfm;
    sfm.accumulate(ColumnVector3(1, 1, 0.2),
                   scale * rot * ColumnVector3(1, 1, 0.2) + trans);
    sfm.accumulate(ColumnVector3(1, 0, 1),
                   scale * rot * ColumnVector3(1, 0, 1) + trans);
    sfm.accumulate(ColumnVector3(1, -1, 1),
                   scale * rot * ColumnVector3(1, -1, 1) + trans);
    sfm.accumulate(ColumnVector3(0, 1, 1),
                   scale * rot * ColumnVector3(0, 1, 1) + trans);
    sfm.accumulate(ColumnVector3(0, -1, 1),
                   scale * rot * ColumnVector3(0, -1, 1) + trans);
    sfm.accumulate(ColumnVector3(-1, 1, 1),
                   scale * rot * ColumnVector3(-1, 1, 1) + trans);
    sfm.accumulate(ColumnVector3(-1, 1, 0),
                   scale * rot * ColumnVector3(-1, 1, 0) + trans);
    sfm.calculate();

    cout << "scale " << sfm.scale() << endl;
    cout << "quaternion " << sfm.quaternion().transpose() << endl;
    cout << "rotation " << endl << sfm.rotation() << endl;
    cout << "translation " << sfm.translation().transpose() << endl;
    cout << "error " << sfm.error() << endl << endl;
  }

  {
    double scale = 0.1244;
    RotationMatrix rot(1.0, 2.0, -0.5);
    ColumnVector3 trans(2.3, -0.9, 0.1e6);

    cout << "true scale " << scale << endl;
    cout << "true rotation " << endl << rot << endl;
    cout << "true translation " << trans.transpose() << endl << endl;

    vector<pair<ColumnVector3, ColumnVector3> > data;

    data.push_back(pair<ColumnVector3, ColumnVector3>(
        ColumnVector3(1, 1, 0.2),
        scale * rot * ColumnVector3(1, 1, 0.2) + trans));
    data.push_back(pair<ColumnVector3, ColumnVector3>(
        ColumnVector3(1, 0, 0), scale * rot * ColumnVector3(1, 0, 0) + trans));
    data.push_back(pair<ColumnVector3, ColumnVector3>(
        ColumnVector3(11, 8, 0),
        scale * rot * ColumnVector3(11, 8, 0) + trans));
    data.push_back(pair<ColumnVector3, ColumnVector3>(
        ColumnVector3(1, 8, 0.9),
        scale * rot * ColumnVector3(1, 8, 0.9) + trans));
    data.push_back(pair<ColumnVector3, ColumnVector3>(
        ColumnVector3(0.2, -9, 0.9),
        scale * rot * ColumnVector3(0.2, -9, 0.9) + trans));
    data.push_back(pair<ColumnVector3, ColumnVector3>(
        ColumnVector3(0.1, -9, 0.91),
        scale * rot * ColumnVector3(0.1, -9, 0.91) + trans));
    data.push_back(pair<ColumnVector3, ColumnVector3>(
        ColumnVector3(0.2, -9, 0.91),
        scale * rot * ColumnVector3(0.2, -9, 0.91) + trans));
    data.push_back(pair<ColumnVector3, ColumnVector3>(
        ColumnVector3(102, 9, 1),
        scale * rot * ColumnVector3(102, 9, 1) + trans));

    data.push_back(pair<ColumnVector3, ColumnVector3>(
        ColumnVector3(-102, 9, 1),
        scale * rot * ColumnVector3(-102, 9, 100) + trans));
    data.push_back(pair<ColumnVector3, ColumnVector3>(
        ColumnVector3(-12, 90, 1),
        scale * rot * ColumnVector3(-1, 90, 0) + trans));
    data.push_back(pair<ColumnVector3, ColumnVector3>(
        ColumnVector3(-9, 9, 1),
        scale * rot * ColumnVector3(1, 9, 100) + trans));

    SfM sfm;
    for (int i = 0; i < 10; ++i) {
      if (RANSAC::run(&sfm, &data, 4, 5, ERROR(), 0.001)) {
        break;
      }
      cout << "SfM on RANSAC failed " << i << "." << endl;
    }

    cout << "scale " << sfm.scale() << endl;
    cout << "quaternion " << sfm.quaternion().transpose() << endl;
    cout << "rotation " << endl << sfm.rotation() << endl;
    cout << "translation " << sfm.translation().transpose() << endl;
    cout << "error " << sfm.error() << endl << endl;
  }

  return 0;
}
