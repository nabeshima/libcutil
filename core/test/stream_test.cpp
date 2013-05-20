/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "IOStream.h"

#include <sstream>

using namespace std;
using namespace cutil;

class Test {
public:
  int write( const void *buf, int size ) {
    char *cbuf = (char*)buf;
    for ( int i = 0; i < size; ++i ) {
      cout << cbuf[ i ] << flush;
    }
    return size;
  }
  
  int read( void *buf, int size ) {
    char *cbuf = (char*)buf;
    for ( int i = 0; i < 10; ++i ) {
      cbuf[ i ] = (char)( ( i + 1 ) % 10 + '0' );
    }
    cbuf[ 10 ] = '\0';
    return 11;
  }
  
  bool isOpen() {
    return true;
  }
};


int main() {
  Test t;
  OStream< Test > ostrm( t );
  
  int v = 23;
  ostrm << v << "test" << endl << endl;
  

  IStream< Test > istrm( t );
  int i;
  istrm >> i;
  cout << i << endl;
  
  return 0;
}
