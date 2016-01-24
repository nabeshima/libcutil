/**
 * $Id: cipher_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <climits>
#include <iostream>
#include <iomanip>

using namespace std;

#include <CamelliaCipher.h>

using namespace cutil;

int main() {
  {
    const char* txt = "0123456789abcdef";
    const char* key = "abcdefghijklmnopqrstuvwxyzABCDEF";

    cout << "src " << txt << endl;

    CamelliaBlock M(txt);
    CamelliaKey K(key, 256);

    CamelliaBlock C = CamelliaCipher::encrypt(M, K);

    unsigned char buf[17];
    buf[16] = '\0';
    C.get(buf);
    cout << "enc " << buf << endl;

    M = CamelliaCipher::decrypt(C, K);
    M.get(buf);
    cout << "dec " << buf << endl;
  }

  {
    const char* txt = "123456789abcdeffedcba987654321";
    uint64_t data[2] = {ULONG_MAX, ULONG_MAX / 2};

    cout << endl << "src " << std::dec << data[0] << " " << data[1] << endl;

    CamelliaBlock M(data);
    CamelliaKey K(txt, 256);

    CamelliaBlock C = CamelliaCipher::encrypt(M, K);
    C.get(data);
    cout << "enc " << data[0] << " " << data[1] << endl;

    M = CamelliaCipher::decrypt(C, K);
    M.get(data);
    cout << "dec " << data[0] << " " << data[1] << endl;
  }

  {
    cout << endl << "#128bit" << endl;

    unsigned char key[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                             0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

    CamelliaBlock M(key);
    CamelliaKey K(key, 128);

    unsigned char data[16];

    unsigned char ans[16] = {0x67, 0x67, 0x31, 0x38, 0x54, 0x96, 0x69, 0x73,
                             0x08, 0x57, 0x06, 0x56, 0x48, 0xea, 0xbe, 0x43};

    CamelliaBlock C = CamelliaCipher::encrypt(M, K);
    C.get(data);
    cout << "enc ";
    for (int i = 0; i < 16; ++i) {
      cout << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(data[i]);
    }
    cout << endl;

    CamelliaBlock C2(ans);
    C2.get(data);
    cout << "ans ";
    for (int i = 0; i < 16; ++i) {
      cout << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(data[i]);
    }
    cout << endl;

    M = CamelliaCipher::decrypt(C, K);
    M.get(data);
    cout << "dec ";
    for (int i = 0; i < 16; ++i) {
      cout << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(data[i]);
    }
    cout << endl;
  }

  {
    cout << endl << "#192bit" << endl;

    unsigned char key[24] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                             0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
                             0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77};

    unsigned char txt[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                             0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

    CamelliaBlock M(txt);
    CamelliaKey K(key, 192);

    unsigned char data[16];

    unsigned char ans[16] = {0xb4, 0x99, 0x34, 0x01, 0xb3, 0xe9, 0x96, 0xf8,
                             0x4e, 0xe5, 0xce, 0xe7, 0xd7, 0x9b, 0x09, 0xb9};

    CamelliaBlock C = CamelliaCipher::encrypt(M, K);
    C.get(data);
    cout << "enc ";
    for (int i = 0; i < 16; ++i) {
      cout << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(data[i]);
    }
    cout << endl;

    CamelliaBlock C2(ans);
    C2.get(data);
    cout << "ans ";
    for (int i = 0; i < 16; ++i) {
      cout << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(data[i]);
    }
    cout << endl;

    M = CamelliaCipher::decrypt(C, K);
    M.get(data);
    cout << "dec ";
    for (int i = 0; i < 16; ++i) {
      cout << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(data[i]);
    }
    cout << endl;
  }

  {
    cout << endl << "#256bit" << endl;

    unsigned char key[32] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                             0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10,
                             0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
                             0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

    unsigned char txt[16] = {0x01, 0x23, 0x45, 0x67, 0x89, 0xab, 0xcd, 0xef,
                             0xfe, 0xdc, 0xba, 0x98, 0x76, 0x54, 0x32, 0x10};

    CamelliaBlock M(txt);
    CamelliaKey K(key, 256);

    unsigned char data[16];

    unsigned char ans[16] = {0x9a, 0xcc, 0x23, 0x7d, 0xff, 0x16, 0xd7, 0x6c,
                             0x20, 0xef, 0x7c, 0x91, 0x9e, 0x3a, 0x75, 0x09};

    CamelliaBlock C = CamelliaCipher::encrypt(M, K);
    C.get(data);
    cout << "enc ";
    for (int i = 0; i < 16; ++i) {
      cout << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(data[i]);
    }
    cout << endl;

    CamelliaBlock C2(ans);
    C2.get(data);
    cout << "ans ";
    for (int i = 0; i < 16; ++i) {
      cout << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(data[i]);
    }
    cout << endl;

    M = CamelliaCipher::decrypt(C, K);
    M.get(data);
    cout << "dec ";
    for (int i = 0; i < 16; ++i) {
      cout << std::hex << std::setw(2) << std::setfill('0')
           << static_cast<int>(data[i]);
    }
    cout << endl;
  }

  return 0;
}
