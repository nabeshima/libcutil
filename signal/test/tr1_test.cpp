/**
 * $Id: tr1_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <stdint.h>
}

#include <tr1/functional>

#include <iostream>
#include <sstream>
#include <string>

using namespace std::tr1;                // function, bind
using namespace std::tr1::placeholders;  // _1, _2

using namespace std;

class A {
 public:
  int show() {
    cout << "A::show()" << endl;
    return 1;
  }
  char get() { return 'A'; }
  int operator()(int i) {
    cout << "A op " << i << endl;
    return 0;
  }
};

class B : public A {
 public:
  int64_t tell() { return 100000; }

  //  typedef int result_type;
  //  typedef result_of< B ( int, int ) >::type result_type;

  double operator()(int i) {
    cout << "B op " << i << endl;
    return i * 10e-8;
  }

  int operator()(int i, int j) {
    cout << "B op " << i << " " << j << endl;
    return i + j;
  }

  //  template< class T >
  //  struct result { typedef double type; };
};

void print() { cout << "print" << endl; }

void printI(int i) { cout << i << endl; }

int main() {
  A a;
  B b;

  function<int64_t()> af1(bind(&A::get, &a));

  {
    int j = af1();
    cout << "a.get() " << j << endl;
  }

  af1 = bind(&A::get, &a);
  {
    int j = af1();
    cout << "a.get() " << j << endl;
  }

  af1 = bind(&B::show, &b);
  {
    int j = af1();
    cout << "a.show() " << j << endl;
  }

  af1 = bind(&A::get, &b);
  {
    int j = af1();
    cout << "b.get() " << j << endl;
  }

  af1 = bind(&B::get, &b);
  {
    int j = af1();
    cout << "b.get() " << j << endl;
  }

  af1 = bind(&B::tell, &b);
  {
    int j = af1();
    cout << "b.tell() " << j << endl;
  }

  function<int()> af2 = bind(&B::show, &b);
  af2();

  function<int()> af3 = bind(&B::show, &b);
  af3();

  af1 = bind(&A::get, &a);
  af1();
  af1();

  {
    int i = af2();
    int j = af1();

    cout << "b.show() " << i << endl;
    cout << "a.get() " << af1() << endl;
    cout << i + j << endl;
  }

  function<void()> vf;
  {
    int p = 1000 + af1() * af2();
    vf = bind(&printI, ref(p));
    vf();

    p = 1;
    vf();
  }

  {
    int p = 182;
    A a;
    vf = bind<int>(a, p);
    p = 1;
  }
  vf();

  {
    int p = 1820;
    B b;
    function<int(int)> iif = bind<int>(b, p, _1);

    cout << iif(7) << endl;
    cout << iif(5) << endl;
  }

  {
    int p = 1820;
    B b;
    function<int(int)> iif = bind<int>(b, p, _1);

    cout << iif(7) << endl;
    cout << iif(5) << endl;

    string str("541 45");

    stringstream sstrm(str);
    function<int(int)>::argument_type p1;
    int p2;

    sstrm >> p1 >> p2;

    iif(p1);
  }

  return 0;
}
