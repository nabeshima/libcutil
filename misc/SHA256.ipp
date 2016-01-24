/**
 * $Id: SHA256.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cutil {

namespace {

const uint32_t K[64] = {
    0x428A2F98, 0x71374491, 0xB5C0FBCF, 0xE9B5dBA5, 0x3956C25B, 0x59F111F1,
    0x923F82A4, 0xAB1C5Ed5, 0xd807AA98, 0x12835B01, 0x243185BE, 0x550C7dC3,
    0x72BE5d74, 0x80dEB1FE, 0x9BdC06A7, 0xC19BF174,

    0xE49B69C1, 0xEFBE4786, 0x0FC19dC6, 0x240CA1CC, 0x2dE92C6F, 0x4A7484AA,
    0x5CB0A9dC, 0x76F988dA, 0x983E5152, 0xA831C66d, 0xB00327C8, 0xBF597FC7,
    0xC6E00BF3, 0xd5A79147, 0x06CA6351, 0x14292967,

    0x27B70A85, 0x2E1B2138, 0x4d2C6dFC, 0x53380d13, 0x650A7354, 0x766A0ABB,
    0x81C2C92E, 0x92722C85, 0xA2BFE8A1, 0xA81A664B, 0xC24B8B70, 0xC76C51A3,
    0xd192E819, 0xd6990624, 0xF40E3585, 0x106AA070,

    0x19A4C116, 0x1E376C08, 0x2748774C, 0x34B0BCB5, 0x391C0CB3, 0x4Ed8AA4A,
    0x5B9CCA4F, 0x682E6FF3, 0x748F82EE, 0x78A5636F, 0x84C87814, 0x8CC70208,
    0x90BEFFFA, 0xA4506CEB, 0xBEF9A3F7, 0xC67178F2};

inline uint32_t SHR(uint32_t n, uint32_t x) { return x >> n; }

inline uint32_t ROTR(uint32_t n, uint32_t x) {
  return (x >> n | (x << (32 - n)));
}

inline uint32_t Ch(uint32_t x, uint32_t y, uint32_t z) {
  return (x & y) ^ (~x & z);
}

inline uint32_t Maj(uint32_t x, uint32_t y, uint32_t z) {
  return (x & y) ^ (y & z) ^ (z & x);
}

inline uint32_t S256_0(uint32_t x) {
  return ROTR(2, x) ^ ROTR(13, x) ^ ROTR(22, x);
}

inline uint32_t S256_1(uint32_t x) {
  return ROTR(6, x) ^ ROTR(11, x) ^ ROTR(25, x);
}

inline uint32_t s256_0(uint32_t x) {
  return ROTR(7, x) ^ ROTR(18, x) ^ SHR(3, x);
}

inline uint32_t s256_1(uint32_t x) {
  return ROTR(17, x) ^ ROTR(19, x) ^ SHR(10, x);
}
}

inline void SHA256::calc() {
  uint32_t W[64];

  for (int i = 0; i < 16; ++i) {
    W[i] = 0;
    for (int j = 0; j < 4; ++j) {
      W[i] <<= 8;
      W[i] += data[i * 4 + j];
      data[i * 4 + j] = 0;
    }
  }
  for (int i = 16; i < 64; ++i) {
    W[i] = s256_1(W[i - 2]) + W[i - 7] + s256_0(W[i - 15]) + W[i - 16];
  }

  uint32_t h[8];  //! 0a, 1b, 2c, 3d, 4e, 5f, 6g, 7h
  for (int i = 0; i < 8; ++i) {
    h[i] = H[i];
  }

  for (int i = 0; i < 64; ++i) {
    uint32_t T1 = h[7] + S256_1(h[4]) + Ch(h[4], h[5], h[6]) + K[i] + W[i],
             T2 = S256_0(h[0]) + Maj(h[0], h[1], h[2]);
    h[7] = h[6];
    h[6] = h[5];
    h[5] = h[4];
    h[4] = h[3] + T1;
    h[3] = h[2];
    h[2] = h[1];
    h[1] = h[0];
    h[0] = T1 + T2;
  }

  for (int i = 0; i < 8; ++i) {
    H[i] += h[i];
  }
}

inline void SHA256::end() {
  if (dnum < 64 - 9) {
    data[dnum] = 0x80;
    for (int i = dnum + 1; i < 64 - 8; ++i) {
      data[i] = 0;
    }

    count = (count * 64 + dnum) * 8;
    for (int i = 7; i >= 0; --i) {
      data[64 - 8 + i] = (count & 0xFF);
      count >>= 8;
    }

    calc();
  } else {
    data[dnum] = 0x80;
    for (int i = dnum + 1; i < 64; ++i) {
      data[i] = 0;
    }
    calc();

    for (int i = 0; i < 64 - 8; ++i) {
      data[i] = 0;
    }

    count = (count * 64 + dnum) * 8;
    for (int i = 7; i >= 0; --i) {
      data[64 - 8 + i] = (count & 0xFF);
      count >>= 8;
    }
    calc();
  }
}

inline SHA256::SHA256() { reset(); }

inline void SHA256::reset() {
  H[0] = 0x6a09e667;
  H[1] = 0xbb67ae85;
  H[2] = 0x3c6ef372;
  H[3] = 0xa54ff53a;
  H[4] = 0x510e527f;
  H[5] = 0x9b05688c;
  H[6] = 0x1f83d9ab;
  H[7] = 0x5be0cd19;

  count = 0;
  dnum = 0;
  for (int i = 0; i < 64; ++i) {
    data[i] = 0;
  }
}

inline void SHA256::push(const void *msg, uint64_t msg_byte) {
  const unsigned char *umsg = (unsigned char *)msg;

  for (uint64_t i = 0; i < msg_byte; ++i) {
    data[dnum++] = umsg[i];

    if (dnum % 64 == 0) {
      calc();
      ++count;
      dnum = 0;

      for (int i = 0; i < 64; ++i) {
        data[i] = 0;
      }
    }
  }
}

inline void SHA256::hash(void *result) const {
  unsigned char *uresult = (unsigned char *)result;

  SHA256 tmp(*this);
  tmp.end();

  for (int i = 0; i < 8; ++i) {
    uint32_t val = tmp.H[i];

    for (int j = 3; j >= 0; --j) {
      uresult[i * 4 + j] = (val & 0xFF);
      val >>= 8;
    }
  }
}

inline void SHA256::hash(void *result, const void *msg, uint64_t msg_byte) {
  SHA256 tmp;
  tmp.push(msg, msg_byte);
  tmp.hash(result);
}
}
