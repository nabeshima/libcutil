/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Polynomial.h"

#include <complex>
#include <iostream>

using namespace std;
using namespace cotave;

int main() {
  {
    double data[] = {1.0, 1.0};
    Polynomial<double> p1(data, 2);

    for (int i = 0; i < 3; ++i) {
      p1 *= p1;
      cout << p1.toString() << endl;
    }

    p1 *= 0;
    cout << p1.toString() << endl;
  }
  {
    double data[] = {1.0, 1.0};
    Polynomial<double> p1(data, 2);
    Polynomial<double> ret;

    for (int i = 0; i < 3; ++i) {
      ret *= p1;
      cout << ret.toString() << endl;
    }
  }

  {
    double data[] = {1.0, 1.0};
    Polynomial<double> p1(data, 2);
    Polynomial<double> ret(p1);

    for (int i = 0; i < 10; ++i) {
      ret *= p1;
      cout << ret.toString() << endl;
    }

    ret /= 3.0;
    cout << ret.toString() << endl;

    ret -= ret;
    cout << ret.toString() << endl;

    ret = p1;
    cout << ret.toString() << endl;

    ret += -p1;
    cout << ret.toString() << endl;
  }

  {
    double data[] = {1.0, 1.0};
    Polynomial<double> p1(data, 2);

    for (int i = 0; i < 2; ++i) {
      p1 *= p1;
      cout << p1.toString() << endl;
    }

    double data2[] = {1.0, -2.0, 1.0};
    Polynomial<double> p2(data2, 3);

    cout << p1.substitute(3.0) << endl;
    cout << p1.substitute(p2).toString() << endl;
  }

  {
    complex<double> data[2];
    data[0] = complex<double>(1.0, -2.0);
    data[1] = complex<double>(-1.0, -3.0);
    Polynomial<complex<double> > p1(data, 2);

    for (int i = 0; i < 1; ++i) {
      p1 *= p1;
      cout << p1.toString() << endl;
    }
    p1 /= 2.0;
    cout << p1.toString() << endl;

    p1 /= complex<double>(3.0, -1.0);
    cout << p1.toString() << endl;

    Polynomial<complex<double> > p2 = p1.substitute(p1);
    cout << p2.toString() << endl;
  }

  return 0;
}
