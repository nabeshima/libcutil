/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_INTX_T_H_
#define _CUTIL_INTX_T_H_

extern "C" {
#include <stdint.h>
}

#include <vector>
#include <string>

namespace cutil {

class intX_t {
private:
  static const uint64_t BASE1 = 10;
  static const uint64_t BASE10 = 1000000000000000000ULL; //! the largest 10^x less than LLONG_MAX
  static const uint64_t BASE_CNUM = 18;
  
  enum SIGN{ POSITIVE = 1, NEGATIVE = -1 } sign;
  std::vector< int64_t > body;
  
  void initByString( const std::string &x ) throw () ;
  
  static void eraseTopZero( intX_t &result ) throw ();
  static bool absNoless( const intX_t &l, const intX_t &s ) throw ();
  static void absAdd( intX_t &result, const intX_t &larger, const intX_t &smaller ) throw ();
  static void absSub( intX_t &result, const intX_t &larger, const intX_t &smaller ) throw ();
  static void absMul( intX_t &result, const intX_t &a, const intX_t &b ) throw ();
  static intX_t absDiv( intX_t &result, const intX_t &num, const intX_t &den ) throw ();
  static void shiftRight( intX_t &result, const uint64_t &x ) throw ();
  static void shiftLeft( intX_t &result, const uint64_t &x ) throw ();
  template< class Functor >
  static void bitOperation( intX_t &result, const intX_t &a, const intX_t &b, Functor func ) throw ();
  template< class Functor >
  static void bitOperation( intX_t &result, const intX_t &a, Functor func ) throw ();
  
public:
  intX_t() throw ();

  template< typename INTEGER >
  intX_t( const INTEGER &x ) throw ();
  intX_t( const char *x ) throw ();
  intX_t( const std::string &x ) throw ();
  
  template< typename INTEGER >
  intX_t& operator=( const INTEGER &x ) throw ();
  
  intX_t operator+( const intX_t &x ) const throw ();
  intX_t operator-( const intX_t &x ) const throw ();
  intX_t operator*( const intX_t &x ) const throw ();
  intX_t operator/( const intX_t &x ) const throw ();
  intX_t operator%( const intX_t &x ) const throw ();
  
  intX_t& operator+=( const intX_t &x ) throw ();
  intX_t& operator-=( const intX_t &x ) throw ();
  intX_t& operator*=( const intX_t &x ) throw ();
  intX_t& operator/=( const intX_t &x ) throw ();
  intX_t& operator%=( const intX_t &x ) throw ();
  
  intX_t& operator++() throw ();
  intX_t& operator--() throw ();
  intX_t operator++( int ) throw ();
  intX_t operator--( int ) throw ();
  
  intX_t operator-() const throw ();
  
  intX_t operator&( const intX_t &x ) const throw ();
  intX_t operator|( const intX_t &x ) const throw ();
  intX_t operator^( const intX_t &x ) const throw ();
  intX_t& operator&=( const intX_t &x ) throw ();
  intX_t& operator|=( const intX_t &x ) throw ();
  intX_t& operator^=( const intX_t &x ) throw ();
  
  intX_t operator~() const throw ();
  
  intX_t operator>>( const uint64_t &x ) const throw ();
  intX_t operator<<( const uint64_t &x ) const throw ();
  intX_t& operator>>=( const uint64_t &x ) throw ();
  intX_t& operator<<=( const uint64_t &x ) throw ();

  bool operator==( const intX_t &x ) const throw ();
  bool operator!=( const intX_t &x ) const throw ();
  bool operator<( const intX_t &x ) const throw ();
  bool operator>( const intX_t &x ) const throw ();
  bool operator<=( const intX_t &x ) const throw ();
  bool operator>=( const intX_t &x ) const throw ();
  
  std::string toString() const throw ();
  std::string toHexString() const throw ();
  std::string toBinString() const throw ();
  
  //! little endian
  static void toBinArray( std::vector< unsigned char > &result, const intX_t &a ) throw ();
};

std::ostream& operator<<( std::ostream &stream, const intX_t &x );

}

#include "intX_t.ipp"

#endif
