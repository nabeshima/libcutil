/**
 * $Id$
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "SharedPtr.h"

#include <iostream>
#include <string>

using namespace std;
using namespace cutil;

class B {
 public:
  explicit B(const string &a) { cout << " B() " << this << " " << a << endl; }

  ~B() { cout << "~B() " << this << endl; }

  void show() { cout << "B::show()       " << this << endl; }

  void showConst() const { cout << "B::show() const " << this << endl; }

  void showPtr(B *t) const {
    cout << "B::showPtr() const " << endl;
    cout << "   ";
    t->showConst();
  }

  void showSharedPtr(const SharedPtr<B> s) const {
    cout << "B::showSharedPtr() const " << &*s << endl;
    cout << "   ";
    s->showConst();
  }
};

int main() {
  SharedPtr<B> spd(new B("D"));
  {
    SharedPtr<B> spb(new B("B"));

    spb->show();
    spb->showConst();
    (*spb).show();
    (*spb).showConst();

    SharedPtr<B> spc(spb);

    SharedPtr<B> spa(new B("A"));

    spc->show();
    spc->showConst();
    (*spc).show();
    (*spc).showConst();

    spd->show();
    spd->showConst();
    (*spd).show();
    (*spd).showConst();

    if (spd == spb) {
      cout << "spd is not spb" << endl;
    } else {
      cout << "spd is the same as spb" << endl;
    }

    spd = spb;

    if (spd == spb) {
      cout << "spd is not spb" << endl;
    } else {
      cout << "spd is the same as spb" << endl;
    }
  }

  spd->show();
  spd->showConst();
  (*spd).show();
  (*spd).showConst();

  spd->showPtr(&*spd);
  spd->showSharedPtr(spd);

  SharedPtr<B> spk(NULL);
  if (spk) {
    cout << "not null" << endl;
  } else {
    cout << "null" << endl;
  }

  if (spd) {
    cout << "not null" << endl;
  } else {
    cout << "null" << endl;
  }

  return 0;
}
