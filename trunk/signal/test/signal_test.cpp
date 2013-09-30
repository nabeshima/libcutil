/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>
#include <cmath>

#include "Signal.h"

using namespace std;
using namespace std::tr1;
using namespace cutil;



int func1( int i ) {
  return i * 2;
}

int func2( int i ) {
  return i / 2;
}

int func3( int i ) {
  return i * i;
}

struct Add {
  double operator()( double i, double j ) {
    return i + j;
  }
};

struct Print {
  void operator()() {
    cout << "print class" << endl;
  }
};

void print() {
  cout << "print func" << endl;
}

double mul( double i, double j ) {
  return i * j;
}

int main() {
  {
    Signal< int ( int ) > sig;
    sig.connect( func1 );
    sig.connect( func2 );
    sig.connect( func3 );
    
    sig( 12 );
    
    list< int >::iterator
      it = sig.results.begin(),
      endIt = sig.results.end();
    while ( it != endIt ) {
      cout << *it << endl;
      ++it;
    }
  }

  {
    Signal< double ( double, double ) > sig;
    sig.connect( mul );
    sig.connect( Add() );
    
    sig( 12.1, 18.9 );
    
    list< double >::iterator
      it = sig.results.begin(),
      endIt = sig.results.end();
    while ( it != endIt ) {
      cout << *it << endl;
      ++it;
    }
  }

  {
    Signal< void () > sig;
    sig.connect( print );
    sig.connect( Print() );

    sig();
  }

  
  return 0;
}
