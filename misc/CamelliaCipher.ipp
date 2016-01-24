/**
 * $Id: CamelliaCipher.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cutil {

namespace {

inline void XOR(unsigned char *ret, const unsigned char *a,
                const unsigned char *b, int size) {
  for (int i = 0; i < size; ++i) {
    ret[i] = a[i] ^ b[i];
  }
}

inline void COPY(unsigned char *ret, const unsigned char *src, int size) {
  for (int i = 0; i < size; ++i) {
    ret[i] = src[i];
  }
}

const unsigned char SIGMA[] = {
    0xA0, 0x9E, 0x66, 0x7F, 0x3B, 0xCC, 0x90, 0x8B,   //! Sigma1
    0xB6, 0x7A, 0xE8, 0x58, 0x4C, 0xAA, 0x73, 0xB2,   //! Sigma2
    0xC6, 0xEF, 0x37, 0x2F, 0xE9, 0x4F, 0x82, 0xBE,   //! Sigma3
    0x54, 0xFF, 0x53, 0xA5, 0xF1, 0xD3, 0x6F, 0x1C,   //! Sigma4
    0x10, 0xE5, 0x27, 0xFA, 0xDE, 0x68, 0x2D, 0x1D,   //! Sigma5
    0xB0, 0x56, 0x88, 0xC2, 0xB3, 0xE6, 0xC1, 0xFD};  //! Sigma6

const unsigned char stable[] = {
    112, 130, 44,  236, 179, 39,  192, 229, 228, 133, 87,  53,  234, 12,  174,
    65,  35,  239, 107, 147, 69,  25,  165, 33,  237, 14,  79,  78,  29,  101,
    146, 189, 134, 184, 175, 143, 124, 235, 31,  206, 62,  48,  220, 95,  94,
    197, 11,  26,  166, 225, 57,  202, 213, 71,  93,  61,  217, 1,   90,  214,
    81,  86,  108, 77,  139, 13,  154, 102, 251, 204, 176, 45,  116, 18,  43,
    32,  240, 177, 132, 153, 223, 76,  203, 194, 52,  126, 118, 5,   109, 183,
    169, 49,  209, 23,  4,   215, 20,  88,  58,  97,  222, 27,  17,  28,  50,
    15,  156, 22,  83,  24,  242, 34,  254, 68,  207, 178, 195, 181, 122, 145,
    36,  8,   232, 168, 96,  252, 105, 80,  170, 208, 160, 125, 161, 137, 98,
    151, 84,  91,  30,  149, 224, 255, 100, 210, 16,  196, 0,   72,  163, 247,
    117, 219, 138, 3,   230, 218, 9,   63,  221, 148, 135, 92,  131, 2,   205,
    74,  144, 51,  115, 103, 246, 243, 157, 127, 191, 226, 82,  155, 216, 38,
    200, 55,  198, 59,  129, 150, 111, 75,  19,  190, 99,  46,  233, 121, 167,
    140, 159, 110, 188, 142, 41,  245, 249, 182, 47,  253, 180, 89,  120, 152,
    6,   106, 231, 70,  113, 186, 212, 37,  171, 66,  136, 162, 141, 250, 114,
    7,   185, 85,  248, 238, 172, 10,  54,  73,  42,  104, 60,  56,  241, 164,
    64,  40,  211, 123, 187, 201, 67,  193, 21,  227, 173, 244, 119, 199, 128,
    158,

    224, 5,   88,  217, 103, 78,  129, 203, 201, 11,  174, 106, 213, 24,  93,
    130, 70,  223, 214, 39,  138, 50,  75,  66,  219, 28,  158, 156, 58,  202,
    37,  123, 13,  113, 95,  31,  248, 215, 62,  157, 124, 96,  185, 190, 188,
    139, 22,  52,  77,  195, 114, 149, 171, 142, 186, 122, 179, 2,   180, 173,
    162, 172, 216, 154, 23,  26,  53,  204, 247, 153, 97,  90,  232, 36,  86,
    64,  225, 99,  9,   51,  191, 152, 151, 133, 104, 252, 236, 10,  218, 111,
    83,  98,  163, 46,  8,   175, 40,  176, 116, 194, 189, 54,  34,  56,  100,
    30,  57,  44,  166, 48,  229, 68,  253, 136, 159, 101, 135, 107, 244, 35,
    72,  16,  209, 81,  192, 249, 210, 160, 85,  161, 65,  250, 67,  19,  196,
    47,  168, 182, 60,  43,  193, 255, 200, 165, 32,  137, 0,   144, 71,  239,
    234, 183, 21,  6,   205, 181, 18,  126, 187, 41,  15,  184, 7,   4,   155,
    148, 33,  102, 230, 206, 237, 231, 59,  254, 127, 197, 164, 55,  177, 76,
    145, 110, 141, 118, 3,   45,  222, 150, 38,  125, 198, 92,  211, 242, 79,
    25,  63,  220, 121, 29,  82,  235, 243, 109, 94,  251, 105, 178, 240, 49,
    12,  212, 207, 140, 226, 117, 169, 74,  87,  132, 17,  69,  27,  245, 228,
    14,  115, 170, 241, 221, 89,  20,  108, 146, 84,  208, 120, 112, 227, 73,
    128, 80,  167, 246, 119, 147, 134, 131, 42,  199, 91,  233, 238, 143, 1,
    61,

    56,  65,  22,  118, 217, 147, 96,  242, 114, 194, 171, 154, 117, 6,   87,
    160, 145, 247, 181, 201, 162, 140, 210, 144, 246, 7,   167, 39,  142, 178,
    73,  222, 67,  92,  215, 199, 62,  245, 143, 103, 31,  24,  110, 175, 47,
    226, 133, 13,  83,  240, 156, 101, 234, 163, 174, 158, 236, 128, 45,  107,
    168, 43,  54,  166, 197, 134, 77,  51,  253, 102, 88,  150, 58,  9,   149,
    16,  120, 216, 66,  204, 239, 38,  229, 97,  26,  63,  59,  130, 182, 219,
    212, 152, 232, 139, 2,   235, 10,  44,  29,  176, 111, 141, 136, 14,  25,
    135, 78,  11,  169, 12,  121, 17,  127, 34,  231, 89,  225, 218, 61,  200,
    18,  4,   116, 84,  48,  126, 180, 40,  85,  104, 80,  190, 208, 196, 49,
    203, 42,  173, 15,  202, 112, 255, 50,  105, 8,   98,  0,   36,  209, 251,
    186, 237, 69,  129, 115, 109, 132, 159, 238, 74,  195, 46,  193, 1,   230,
    37,  72,  153, 185, 179, 123, 249, 206, 191, 223, 113, 41,  205, 108, 19,
    100, 155, 99,  157, 192, 75,  183, 165, 137, 95,  177, 23,  244, 188, 211,
    70,  207, 55,  94,  71,  148, 250, 252, 91,  151, 254, 90,  172, 60,  76,
    3,   53,  243, 35,  184, 93,  106, 146, 213, 33,  68,  81,  198, 125, 57,
    131, 220, 170, 124, 119, 86,  5,   27,  164, 21,  52,  30,  28,  248, 82,
    32,  20,  233, 189, 221, 228, 161, 224, 138, 241, 214, 122, 187, 227, 64,
    79,

    112, 44,  179, 192, 228, 87,  234, 174, 35,  107, 69,  165, 237, 79,  29,
    146, 134, 175, 124, 31,  62,  220, 94,  11,  166, 57,  213, 93,  217, 90,
    81,  108, 139, 154, 251, 176, 116, 43,  240, 132, 223, 203, 52,  118, 109,
    169, 209, 4,   20,  58,  222, 17,  50,  156, 83,  242, 254, 207, 195, 122,
    36,  232, 96,  105, 170, 160, 161, 98,  84,  30,  224, 100, 16,  0,   163,
    117, 138, 230, 9,   221, 135, 131, 205, 144, 115, 246, 157, 191, 82,  216,
    200, 198, 129, 111, 19,  99,  233, 167, 159, 188, 41,  249, 47,  180, 120,
    6,   231, 113, 212, 171, 136, 141, 114, 185, 248, 172, 54,  42,  60,  241,
    64,  211, 187, 67,  21,  173, 119, 128, 130, 236, 39,  229, 133, 53,  12,
    65,  239, 147, 25,  33,  14,  78,  101, 189, 184, 143, 235, 206, 48,  95,
    197, 26,  225, 202, 71,  61,  1,   214, 86,  77,  13,  102, 204, 45,  18,
    32,  177, 153, 76,  194, 126, 5,   183, 49,  23,  215, 88,  97,  27,  28,
    15,  22,  24,  34,  68,  178, 181, 145, 8,   168, 252, 80,  208, 125, 137,
    151, 91,  149, 255, 210, 196, 72,  247, 219, 3,   218, 63,  148, 92,  2,
    74,  51,  103, 243, 127, 226, 155, 38,  55,  59,  150, 75,  190, 46,  121,
    140, 110, 142, 245, 182, 253, 89,  152, 106, 70,  186, 37,  66,  162, 250,
    7,   85,  238, 10,  73,  104, 56,  164, 40,  123, 201, 193, 227, 244, 199,
    158};
}

inline unsigned char *CamelliaBlock::L() { return e; }

inline unsigned char *CamelliaBlock::R() { return e + 8; }

inline const unsigned char *CamelliaBlock::L() const { return e; }

inline const unsigned char *CamelliaBlock::R() const { return e + 8; }

inline CamelliaBlock &CamelliaBlock::operator<<=(int x) {
  CamelliaBlock tmp(*this);

  int p = x / 8;
  int r = x - p * 8;

  for (int i = 0; i < 16; ++i) {
    e[i] = (tmp[(i + p) % 16] << r) | (tmp[(i + p + 1) % 16] >> (8 - r));
  }

  return *this;
}

unsigned char &CamelliaBlock::operator[](int x) { return e[x]; }

inline CamelliaBlock::CamelliaBlock() {
  for (int i = 0; i < 16; ++i) {
    e[i] = 0;
  }
}

inline CamelliaBlock::CamelliaBlock(const void *data) { set(data); }

inline CamelliaBlock &CamelliaBlock::operator=(const void *data) {
  set(data);
  return *this;
}

inline void CamelliaBlock::set(const void *data) {
  unsigned char *udata = (unsigned char *)data;
  for (int i = 0; i < 16; ++i) {
    e[i] = udata[i];
  }
}

inline void CamelliaBlock::get(void *data) const {
  unsigned char *udata = (unsigned char *)data;
  for (int i = 0; i < 16; ++i) {
    udata[i] = e[i];
  }
}

inline CamelliaKey::CamelliaKey() : size(0) {}

inline CamelliaKey::CamelliaKey(const void *key, int bitsize) {
  setKey(key, bitsize);
}

inline void CamelliaKey::setKey(const void *key, int bitsize) {
  this->size = bitsize;

  unsigned char *ukey = (unsigned char *)key;

  CamelliaBlock KL, KR;

  for (int i = 0; i < 16; ++i) {
    KL[i] = ukey[i];
  }

  if (size == 192) {
    for (int i = 0; i < 8; ++i) {
      KR[i] = ukey[16 + i];
      KR[8 + i] = ~KR[i];
    }
  } else if (size == 256) {
    for (int i = 0; i < 16; ++i) {
      KR[i] = ukey[16 + i];
    }
  }

  CamelliaBlock KA, KB;

  CamelliaBlock Kr_1;
  XOR(Kr_1.e, KL.e, KR.e, 16);

  for (int r = 1; r <= 2; ++r) {
    CamelliaBlock Kr;

    CamelliaCipher::F(Kr.L(), Kr_1.L(), SIGMA + 8 * (r - 1));
    XOR(Kr.L(), Kr_1.R(), Kr.L(), 8);
    COPY(Kr.R(), Kr_1.L(), 8);

    Kr_1 = Kr;
  }

  XOR(Kr_1.e, Kr_1.e, KL.e, 16);

  for (int r = 3; r <= 4; ++r) {
    CamelliaBlock Kr;

    CamelliaCipher::F(Kr.L(), Kr_1.L(), SIGMA + 8 * (r - 1));
    XOR(Kr.L(), Kr_1.R(), Kr.L(), 8);
    COPY(Kr.R(), Kr_1.L(), 8);

    Kr_1 = Kr;
  }

  COPY(KA.e, Kr_1.e, 16);  //! Store KA

  XOR(Kr_1.e, Kr_1.e, KR.e, 16);

  for (int r = 5; r <= 6; ++r) {
    CamelliaBlock Kr;

    CamelliaCipher::F(Kr.L(), Kr_1.L(), SIGMA + 8 * (r - 1));
    XOR(Kr.L(), Kr_1.R(), Kr.L(), 8);
    COPY(Kr.R(), Kr_1.L(), 8);

    Kr_1 = Kr;
  }

  COPY(KB.e, Kr_1.e, 16);  //! Store KB

  if (size == 128) {
    COPY(kw, KL.e, 16);
    COPY(k, KA.e, 16);

    KL <<= 15;  //! KL <<< 15
    COPY(k + 8 * 2, KL.e, 16);

    KA <<= 15;  //! KA <<< 15
    COPY(k + 8 * 4, KA.e, 16);

    KA <<= 15;  //! KA <<< 30
    COPY(kl, KA.e, 16);

    KL <<= 30;  //! KL <<< 45
    COPY(k + 8 * 6, KL.e, 16);

    KA <<= 15;  //! KA <<< 45
    COPY(k + 8 * 8, KA.L(), 8);

    KL <<= 15;  //! KL <<< 60
    COPY(k + 8 * 9, KL.R(), 8);

    KA <<= 15;  //! KA <<< 60
    COPY(k + 8 * 10, KA.e, 16);

    KL <<= 17;  //! KL <<< 77
    COPY(kl + 8 * 2, KL.e, 16);

    KL <<= 17;  //! KL <<< 94
    COPY(k + 8 * 12, KL.e, 16);

    KA <<= 34;  //! KA <<< 94
    COPY(k + 8 * 14, KA.e, 16);

    KL <<= 17;  //! KL <<< 111
    COPY(k + 8 * 16, KL.e, 16);

    KA <<= 17;  //! KA <<< 111
    COPY(kw + 8 * 2, KA.e, 16);
  } else {
    COPY(kw, KL.e, 16);

    COPY(k, KB.e, 16);

    KR <<= 15;  //! KR <<< 15
    COPY(k + 8 * 2, KR.e, 16);

    KA <<= 15;  //! KA <<< 15
    COPY(k + 8 * 4, KA.e, 16);

    KR <<= 15;  //! KR <<< 30
    COPY(kl, KR.e, 16);

    KB <<= 30;  //! KB <<< 30
    COPY(k + 8 * 6, KB.e, 16);

    KL <<= 45;  //! KL <<< 45
    COPY(k + 8 * 8, KL.e, 16);

    KA <<= 30;  //! KA <<< 45
    COPY(k + 8 * 10, KA.e, 16);

    KL <<= 15;  //! KL <<< 60
    COPY(kl + 8 * 2, KL.e, 16);

    KR <<= 30;  //! KR <<< 60
    COPY(k + 8 * 12, KR.e, 16);

    KB <<= 30;  //! KB <<< 60
    COPY(k + 8 * 14, KB.e, 16);

    KL <<= 17;  //! KL <<< 77
    COPY(k + 8 * 16, KL.e, 16);

    KA <<= 32;  //! KA <<< 77
    COPY(kl + 8 * 4, KA.e, 16);

    KR <<= 34;  //! KR <<< 94
    COPY(k + 8 * 18, KR.e, 16);

    KA <<= 17;  //! KA <<< 94
    COPY(k + 8 * 20, KA.e, 16);

    KL <<= 34;  //! KL <<< 111
    COPY(k + 8 * 22, KL.e, 16);

    KB <<= 51;  //! KB <<< 111
    COPY(kw + 8 * 2, KB.e, 16);
  }
}

inline void CamelliaCipher::F(unsigned char *Y, const unsigned char *X,
                              const unsigned char *k) {
  unsigned char buf[8];
  XOR(buf, X, k, 8);
  S(buf);
  P(Y, buf);
}

inline void CamelliaCipher::FL(unsigned char *Y, const unsigned char *X,
                               const unsigned char *kl) {
  unsigned char buf[4];

  for (int i = 0; i < 4; ++i) {
    buf[i] = ((X[i] << 1) | (X[(i + 1) % 4] >> 7)) &
             ((kl[i] << 1) | (kl[(i + 1) % 4] >> 7));
  }
  XOR(Y + 4, buf, X + 4, 4);

  for (int i = 0; i < 4; ++i) {
    buf[i] = Y[4 + i] | kl[4 + i];
  }
  XOR(Y, buf, X, 4);
}

inline void CamelliaCipher::FLinv(unsigned char *X, const unsigned char *Y,
                                  const unsigned char *kl) {
  unsigned char buf[4];

  for (int i = 0; i < 4; ++i) {
    buf[i] = Y[4 + i] | kl[4 + i];
  }
  XOR(X, buf, Y, 4);

  for (int i = 0; i < 4; ++i) {
    buf[i] = ((X[i] << 1) | (X[(i + 1) % 4] >> 7)) &
             ((kl[i] << 1) | (kl[(i + 1) % 4] >> 7));
  }
  XOR(X + 4, buf, Y + 4, 4);
}

inline void CamelliaCipher::S(unsigned char *l) {
  s(0, &l[0]);
  s(1, &l[1]);
  s(2, &l[2]);
  s(3, &l[3]);
  s(1, &l[4]);
  s(2, &l[5]);
  s(3, &l[6]);
  s(0, &l[7]);
}

inline void CamelliaCipher::P(unsigned char *Y, const unsigned char *X) {
  Y[0] = X[0] ^ X[2] ^ X[3] ^ X[5] ^ X[6] ^ X[7];
  Y[1] = X[0] ^ X[1] ^ X[3] ^ X[4] ^ X[6] ^ X[7];
  Y[2] = X[0] ^ X[1] ^ X[2] ^ X[4] ^ X[5] ^ X[7];
  Y[3] = X[1] ^ X[2] ^ X[3] ^ X[4] ^ X[5] ^ X[6];
  Y[4] = X[0] ^ X[1] ^ X[5] ^ X[6] ^ X[7];
  Y[5] = X[1] ^ X[2] ^ X[4] ^ X[6] ^ X[7];
  Y[6] = X[2] ^ X[3] ^ X[4] ^ X[5] ^ X[7];
  Y[7] = X[0] ^ X[3] ^ X[4] ^ X[5] ^ X[6];
}

inline void CamelliaCipher::s(int index, unsigned char *x) {
  *x = stable[index * 256 + *x];
}

inline CamelliaBlock CamelliaCipher::encrypt(const CamelliaBlock &M,
                                             const CamelliaKey &K) {
  const int REND = (K.size == 128) ? 18 : 24;

  CamelliaBlock Mr_1;

  XOR(Mr_1.e, M.e, K.kw, 16);

  for (int r = 1; r <= REND; ++r) {
    CamelliaBlock Mr;

    F(Mr.L(), Mr_1.L(), K.k + 8 * (r - 1));
    XOR(Mr.L(), Mr_1.R(), Mr.L(), 8);
    COPY(Mr.R(), Mr_1.L(), 8);

    if (r == 6 || r == 12 || (K.size != 128 && r == 18)) {
      FL(Mr.L(), Mr.L(), K.kl + 8 * (2 * r / 6 - 2));
      FLinv(Mr.R(), Mr.R(), K.kl + 8 * (2 * r / 6 - 1));
    }

    Mr_1 = Mr;
  }

  CamelliaBlock ret;
  XOR(ret.L(), Mr_1.R(), K.kw + 8 * 2, 8);
  XOR(ret.R(), Mr_1.L(), K.kw + 8 * 3, 8);

  return ret;
}

inline CamelliaBlock CamelliaCipher::decrypt(const CamelliaBlock &C,
                                             const CamelliaKey &K) {
  const int RBEGIN = (K.size == 128) ? 18 : 24;

  CamelliaBlock Cr;

  XOR(Cr.R(), C.L(), K.kw + 8 * 2, 8);
  XOR(Cr.L(), C.R(), K.kw + 8 * 3, 8);

  for (int r = RBEGIN; r >= 1; --r) {
    CamelliaBlock Cr_1;

    F(Cr_1.R(), Cr.R(), K.k + 8 * (r - 1));
    XOR(Cr_1.R(), Cr.L(), Cr_1.R(), 8);
    COPY(Cr_1.L(), Cr.R(), 8);

    if (r == 19 || r == 13 || r == 7) {
      FL(Cr_1.R(), Cr_1.R(), K.kl + 8 * (2 * (r - 1) / 6 - 1));
      FLinv(Cr_1.L(), Cr_1.L(), K.kl + 8 * (2 * (r - 1) / 6 - 2));
    }
    Cr = Cr_1;
  }

  XOR(Cr.e, Cr.e, K.kw, 16);

  return Cr;
}
}
