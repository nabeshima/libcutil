/**
 * $Id: CamelliaCipher.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
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
  unsigned char e[16];

  unsigned char *L();
  unsigned char *R();
  const unsigned char *L() const;
  const unsigned char *R() const;

  CamelliaBlock &operator<<=(int x);
  unsigned char &operator[](int x);

 public:
  CamelliaBlock();
  explicit CamelliaBlock(const void *data);
  CamelliaBlock &operator=(const void *data);
  void set(const void *data);
  void get(void *data) const;
};

class CamelliaKey {
  friend class CamelliaCipher;

 private:
  int size;
  unsigned char kw[8 * 4];  //! subkeys
  unsigned char k[8 * 24];
  unsigned char kl[8 * 6];

 public:
  CamelliaKey();

  /*!
    128, 192 and 256
    are allowable as bitsize.
  */
  CamelliaKey(const void *key, int bitsize);

  void setKey(const void *key, int bitsize);
};

class CamelliaCipher {
  friend class CamelliaKey;

 private:
  static void F(unsigned char *Y, const unsigned char *X,
                const unsigned char *k);
  static void FL(unsigned char *Y, const unsigned char *X,
                 const unsigned char *kl);
  static void FLinv(unsigned char *X, const unsigned char *Y,
                    const unsigned char *kl);
  static void S(unsigned char *l);
  static void P(unsigned char *Y, const unsigned char *X);

  static void s(int index, unsigned char *x);

 public:
  static CamelliaBlock encrypt(const CamelliaBlock &M, const CamelliaKey &K);
  static CamelliaBlock decrypt(const CamelliaBlock &C, const CamelliaKey &K);
};
}  // namespace cutil

#include <CamelliaCipher.ipp>

#endif
