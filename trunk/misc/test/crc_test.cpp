/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cstring>
#include <cstdio>
#include <iostream>
#include <iomanip>

using namespace std;

#include <CRC32C.h>

using namespace cutil;

int main() {
  const char key[] ="abcdefghijklmn";
  const char key3[] ="abcdefghijklmn\0\0\0\0";
  const char key2[] ="abcdefghijklmnnnnnnnnnn";
  
  unsigned char c[ 4 ];
  for ( int i = 0; i < 4; ++i ) {
    c[ i ] = 0;
  }
  
  CRC32C crc;
  crc.push( key, 14 );
  crc.crc( c );
  
  cout << "CRC ";
  for ( int i = 0; i < 4; ++i ) {
    cout << std::hex << std::setw( 2 ) << std::setfill( '0' )<< (int)c[ i ];
  }
  cout << endl;
  
  crc.reset();
  crc.push( key3, 18 );
  crc.crc( c );
  
  cout << "CRC ";
  for ( int i = 0; i < 4; ++i ) {
    cout << std::hex << std::setw( 2 ) << std::setfill( '0' )<< (int)c[ i ];
  }
  cout << endl;
  
  crc.reset();
  crc.push( key, ::strlen( key ) );
  crc.push( &c[ 0 ], 1 );
  crc.push( &c[ 1 ], 1 );
  crc.push( &c[ 2 ], 1 );
  crc.push( &c[ 3 ], 1 );
  crc.crc( c );
  
  for ( int i = 0; i < 4; ++i ) {
    cout << std::hex << std::setw( 2 ) << std::setfill( '0' )<< (int)c[ i ];
  }
  cout << endl;
  
  crc.reset();
  crc.push( key2, ::strlen( key2 ) );
  crc.crc( c );
  
  cout << "CRC ";
  for ( int i = 0; i < 4; ++i ) {
    cout << std::hex << std::setw( 2 ) << std::setfill( '0' )<< (int)c[ i ];
  }
  cout << endl;

  //  printf("%x\n", crc32((const unsigned char*)key, ::strlen( key )));
  
  return 0;
}
