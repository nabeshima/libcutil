/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <climits>
#include <iostream>
#include <intX_t.h>

#ifndef LLONG_MAX
#define LLONG_MAX	9223372036854775807LL
#endif
#ifndef LLONG_MIN
#define LLONG_MIN	(-LLONG_MAX - 1LL)
#endif
#ifndef ULLONG_MAX
#define ULLONG_MAX	18446744073709551615ULL
#endif

using namespace std;
using namespace cutil;

#include <limits.h>

int main() {
  {
    intX_t ix;
    intX_t i( -12 );
    intX_t l( 13L );
    intX_t ui( 14U );
    intX_t ul( 15UL );
    intX_t ix2( i );
    
    cout << ix2 << endl;

    ix2 = 14;
    cout << ix2 << endl;

    cout << ix2 << " " << ul << endl;
    cout << ( ix2 < ul ) << endl;
    cout << ( ul < ix2 ) << endl;
    cout << ( ix2 > ul ) << endl;
    cout << ( ul > ix2 ) << endl;
    cout << ( ul == ix2 ) << endl;
    cout << ( ul != ix2 ) << endl;
    cout << ( ui == ix2 ) << endl;
    cout << ( ui != ix2 ) << endl;
    cout << ( ul <= ix2 ) << endl;
    cout << ( ui >= ix2 ) << endl;
    
    cout << ix2 + ix2 << endl;

    intX_t max1( ULONG_MAX );
    intX_t max2( LONG_MIN );
    
    cout << "LLONG_MAX: " << LLONG_MAX << endl;
    cout << "ULLONG_MAX: " << ULLONG_MAX << endl;
    cout << "LLONG_MIN: " << LLONG_MIN << endl;
    cout << "max1: " << max1 << endl;
    cout << "max2: " << max2 << endl;
    cout << "max1 + max2: " << max1 + max2 << endl;
    cout << "max2 + max1: " << max2 + max1 << endl;
    cout << "max2 - max1: " << max2 - max1 << endl;
    
    
    intX_t max3 = max1 + max1;

    cout << "max3: " << max3 << endl;
    cout << "max3 - max3: " << max3 - max3 << endl;
    cout << "max3 - max2: " << ( max3 - max2 ) << endl;
    cout << "max3 - max2 + max2: " << ( max3 - max2 ) + max2 << endl;
    cout << "max2 - max2 + max3: " << max2 - max2 + max3 << endl;
    cout << "max2 + max3 - max2: " << ( max2 + max3 ) - max2 << endl;
    cout << "max3 -= max2 + max1: " << ( max3 -= max2 + max1 ) << endl;
    
    intX_t l1( LONG_MAX );
    intX_t l2( LONG_MIN + 1 );
    cout << "l1: " << l1 << endl;
    cout << "l2: " << l2 << endl;
    cout << "max3 + l2: " << max3 + l2 << endl;
    cout << "l2 + l1: " << l2 + l1 << endl;
    cout << "l2 - l1: " << l2 - l1 << endl;
    cout << "l1 - l1: " << l1 - l1 << endl;
    
    cout << "l1-=l1: " << (l1-=l1) << endl;
    cout << "l1+=l2: " << (l1+=l2) << endl;
    cout << "l1+=l2: " << (l1+=l2) << endl;

    cout << "max3: " << max3 << endl;
    cout << "-max3: " << -max3 << endl;
    cout << "++(-l2): " << ( ++(-l2) ) << endl;
    cout << "--(-l2): " << ( --(-l2) ) << endl;
    
    cout << "l2++: " << ( l2++ ) << endl;
    cout << "l2--: " << ( l2-- ) << endl;
    cout << "l2: " << ( l2 ) << endl;
    
    //    intX_t hex( "0xaaafffffeeeeefffab000119871032234211111111111111115555555555555555555444444442fffbbbbbbbbbc" );
    intX_t hex( "0xa" );
    cout << "hex: " << hex << endl;

    cout << "max3: " << ( max3 ) << endl;
    intX_t val( "0x1000000000000000000000000000000000000000000000000000000000000000000000000" );
    intX_t tmp( val );
    
    
    for ( int i = 1; i < 100000000; ++i ) {
      cout << ( val >> i ) << ": " << i << endl;
      if ( val >> i == 1 ) {
        break;
      }
    }
    
    for ( int i = 0; i < 100000000; ++i ) {
      cout << ( val = val / 2 ) << ": " << i << endl;
      if ( val == 1 ) {
        break;
      }
    }

    for ( int i = 0; i < 10000000; ++i ) {
      cout << ( tmp /= 2 ) << ": " << i << endl;
      if ( tmp == 1 ) {
        break;
      }
    } 

    for ( int i = 0; i < 287; ++i ) {
      cout << ( tmp *= 2 ) << ": " << i << endl;
    } 

    for ( int i = 0; i < 287; ++i ) {
      cout << ( val = val * 2 ) << ": " << i << endl;
    } 
  }
  
  intX_t a( "0x110000001000000100000001" );
  intX_t b( "0x100000010000001000111111" );
  intX_t c( "0x100000000000000000000001" );
  intX_t d( "0x110000011000001100111111" );
  intX_t e( "0x010000011000001100111110" );

  
  cout << "a: " << a << endl;
  cout << "b: " << b << endl;
  cout << "a & b: " << ( a & b ) << endl;
  cout << "    c: " << c << endl;
  cout << "a | b: " << ( a | b ) << endl;
  cout << "    d: " << d << endl;  

  cout << "a ^ b: " << ( a ^ b ) << endl;
  cout << "    e: " << e << endl;  
  


  intX_t f( "0xccCCccaaa999AAAddddddddddfff" );
  intX_t g( "0x3333335556665552222222222000" );

  cout << " f: " << f << endl << std::showbase;
  cout << " f : " << std::hex << f << endl << std::dec;
  cout << " g: " << g << endl;  
  cout << " g : " << std::hex << g << endl << std::dec;
  cout << "~f: " << ~f << endl;
  cout << "~f : " << std::hex << ~f << endl << std::dec;
  cout << "~g: " << ~g << endl;  
  cout << "~g : " << std::hex << ~g << endl << std::dec;
  
  intX_t h( "0x0f010f010f010f01ffffeeeeeeeeeeeabcd" );
  cout << " h : " << h << endl;
  cout << " h : " << std::hex << h << endl << std::dec;
  cout << " h : " << h.toBinString() << endl;
  cout << " ~h: " << ~h << endl;
  cout << " ~h: " << std::hex << ~h << endl;
  cout << " ~h: " << (~h).toBinString() << endl;
  cout << "~~h: " << ~~h << endl;
  cout << "~~h: " << std::hex << ~~h << endl;
  cout << "~~h: " << (~~h).toBinString() << endl << std::dec;

  intX_t t = 1;
  for ( intX_t i = 0; i < "0xff"; ++i ) {
    cout << i << ": " << ( t <<= 1 ) << endl;
  }
  cout << (t).toBinString() << endl;
  cout << (~t).toBinString() << endl;
  cout << ( t + ~t ).toBinString() << endl;
  
  intX_t val = 1;
  ( val <<= 2047 ) -= 1;
  
  cout << val << endl;

  cout << intX_t( 4 ) * intX_t( -496 ) << endl;
  intX_t x( 4 );
  intX_t y( -496 );
  cout << ( x *= y ) << endl;

  return 0;
}
