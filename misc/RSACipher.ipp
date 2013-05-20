/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <sys/time.h>
}

#include <cstdlib>

namespace cutil {

namespace {

inline
intX_t LCM( const intX_t &a,
            const intX_t &b ) throw () {
  
  //! Extended Euclid's Algorithm
  intX_t 
    x1( 1 ), y1( 0 ), z1( a ),
    x2( 0 ), y2( 1 ), z2( b );
  
  while ( z2 != 0 ) {
    intX_t
      tmp( z1 / z2 ),
      x3( x1 - tmp * x2 ),
      y3( y1 - tmp * y2 ),
      z3( z1 - tmp * z2 );
    x1 = x2; 
    y1 = y2; 
    z1 = z2; 
    x2 = x3; 
    y2 = y3; 
    z2 = z3; 
  }
  
  return ( a * b ) / z1;
}

inline
intX_t POWMOD( const intX_t &a, const intX_t &b, const intX_t &m ) throw () {
  intX_t 
    l( 1 ),
    i( a ),
    j( b ),
    tmp;
  
  while ( j > 1 ) {
    tmp = j / 2;
    
    if ( ( j - tmp * 2 ) == 1 ) {
      l = l * i;
    }
    
    j = tmp;
    
    i = ( i * i ) % m;
    l %= m;
  }
  
  return ( i * l ) % m;
}


inline
bool FERMATCHECK( const intX_t &x ) {
  if ( x % 3 == 0 ||
       x % 5 == 0 ||
       x % 7 == 0 ||
       x % 11 == 0 ||
       x % 13 == 0 ||
       x % 17 == 0 ) {
    return false;
  }
  return ( POWMOD( 2, x - 1, x ) == 1 );
}

}

inline
RSAKey::RSAKey() throw () {}

inline
RSAKey::RSAKey( const intX_t &n,
                const intX_t &ed ) throw () 
  : n( n ), ed( ed ) {}

inline
void RSAKey::setKey( const intX_t &n,
                     const intX_t &ed ) throw () {
  this->n = n;
  this->ed = ed;
}


inline
void RSAKey::generate( intX_t &n,
                       intX_t &e,
                       intX_t &d,
                       int security_bits ) throw () {
  { // seed of rand()
    timeval rand_tv;
    ::gettimeofday( &rand_tv, NULL );
    ::srand( rand_tv.tv_sec ^ rand_tv.tv_usec );
  }
  
  intX_t p( 1 ), q( 1 );   // prime numbers
  
  while ( !FERMATCHECK( p ) ) {
    p = 1;
    
    for ( int i = 1; i < security_bits / 2 - 1; ++i ) {
      p <<= 1;
      p += ( ::rand() / ( (double)RAND_MAX + 1.0 ) < 0.5 )? 0: 1;
    }
    p <<= 1;
    p += 1;

    //    cout << "p: " << p << endl;
  }
  while ( !FERMATCHECK( q ) ) {
    q = 1;
    for ( int i = 1; i < security_bits / 2 - 1; ++i ) {
      q <<= 1;
      q += ( ::rand() / ( (double)RAND_MAX + 1.0 ) < 0.5 )? 0: 1;
    }
    q <<= 1;
    q += 1;

    //    cout << "q: " << q << endl;
  }    
  
  
  n = p * q;

  intX_t L( LCM( p - 1, q - 1 ) );
  
  e = ( L % 11 == 0 )? 13: 11;
  
  //! Extended Euclid's Algorithm
  intX_t 
    x1( 1 ), y1( 0 ), z1( L ),
    x2( 0 ), y2( 1 ), z2( e );
  
  while ( z2 != 0 ) {
    intX_t
      tmp( z1 / z2 ),
      x3( x1 - tmp * x2 ),
      y3( y1 - tmp * y2 ),
      z3( z1 - tmp * z2 );
    x1 = x2; 
    y1 = y2; 
    z1 = z2; 
    x2 = x3; 
    y2 = y3; 
    z2 = z3; 
  }
  
  d = y1;
  while ( d < 0 ) {
    d += L;
  }
}

inline
intX_t RSACipher::cipher( const intX_t &X, const RSAKey &K ) throw () {
  return POWMOD( X, K.ed, K.n );
}

inline
intX_t RSACipher::encrypt( const intX_t &M, const RSAKey &K ) throw () {
  return cipher( M, K );
}

inline
intX_t RSACipher::decrypt( const intX_t &C, const RSAKey &K ) throw () {
  return cipher( C, K );
}

}
