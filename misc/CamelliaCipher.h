/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_CAMELLIACIPHER_H_
#define _CUTIL_CAMELLIACIPHER_H_

namespace cutil {


class CamelliaBlock {
  friend class CamelliaKey;
  friend class CamelliaCipher;
  
private:
  unsigned char e[ 16 ];
  
  unsigned char* L() throw ();
  unsigned char* R() throw ();
  const unsigned char* L() const throw ();
  const unsigned char* R() const throw ();
  
  CamelliaBlock& operator<<=( int x ) throw ();
  unsigned char& operator[]( int x ) throw ();

public:
  CamelliaBlock() throw ();
  CamelliaBlock( const void *data ) throw ();
  CamelliaBlock& operator=( const void *data ) throw (); 
  void set( const void *data ) throw ();
  void get( void *data ) const throw ();
};


class CamelliaKey {
  friend class CamelliaCipher;
  
private:
  int size;
  unsigned char kw[ 8 * 4 ]; //! subkeys
  unsigned char k[ 8 * 24 ];
  unsigned char kl[ 8 * 6 ];
  
public:
  CamelliaKey() throw ();

  /*!
    128, 192 and 256
    are allowable as bitsize.
  */
  CamelliaKey( const void *key, int bitsize ) throw ();

  void setKey( const void *key, int bitsize ) throw ();
};


class CamelliaCipher {
  friend class CamelliaKey;

private:
  static void F( unsigned char *Y,
                 const unsigned char *X,
                 const unsigned char *k ) throw ();
  static void FL( unsigned char *Y,
                  const unsigned char *X,
                  const unsigned char *kl ) throw ();
  static void FLinv( unsigned char *X,
                     const unsigned char *Y,
                     const unsigned char *kl ) throw ();
  static void S( unsigned char *l ) throw ();
  static void P( unsigned char *Y,
                 const unsigned char *X ) throw ();
  
  static void s( int index, unsigned char &x ) throw ();
  
public:
  static CamelliaBlock encrypt( const CamelliaBlock &M, const CamelliaKey &K ) throw ();
  static CamelliaBlock decrypt( const CamelliaBlock &C, const CamelliaKey &K ) throw ();
};

}

#include <CamelliaCipher.ipp>

#endif
