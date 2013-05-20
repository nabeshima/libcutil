/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_RSACIPHER_H_
#define _CUTIL_RSACIPHER_H_

#include "intX_t.h"

namespace cutil {

class RSAKey {
  friend class RSACipher;
  
private:
  intX_t n;
  intX_t ed;
  
public:
  RSAKey() throw ();
  RSAKey( const intX_t &n,
          const intX_t &ed ) throw ();  //! public/private key

  void setKey( const intX_t &n,
               const intX_t &ed ) throw (); //! public/private key
  
  static void generate( intX_t &n,             //! public/private key
                        intX_t &e,             //! public
                        intX_t &d,             //! private
                        int security_bits ) throw ();   
};

class RSACipher {
public:
  static intX_t cipher( const intX_t &X, const RSAKey &K ) throw ();
  static intX_t encrypt( const intX_t &M, const RSAKey &K ) throw ();
  static intX_t decrypt( const intX_t &C, const RSAKey &K ) throw ();
};

}

#include "RSACipher.ipp"

#endif
