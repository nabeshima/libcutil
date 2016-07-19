/**
 * $Id: crc_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cstdio>
#include <cstring>
#include <iomanip>
#include <iostream>

#include "CRC.h"

using namespace std;
using namespace cutil;

int main() {
  const char key[] = "abcdefghijklmn";
  const char key3[] = "abcdefghijklmn\0\0\0\0";
  const char key2[] = "abcdefghijklmnnnnnnnnnn";

  unsigned char c[4];
  for (int i = 0; i < 4; ++i) {
    c[i] = 0;
  }

  CRC<CRC_32C> crc;
  crc.push(key, 14);
  crc.crc(c);

  cout << "CRC ";
  for (int i = 0; i < 4; ++i) {
    cout << std::hex << std::setw(2) << std::setfill('0')
         << static_cast<int>(c[i]);
  }
  cout << endl;

  crc.reset();
  crc.push(key3, 18);
  crc.crc(c);

  cout << "CRC ";
  for (int i = 0; i < 4; ++i) {
    cout << std::hex << std::setw(2) << std::setfill('0')
         << static_cast<int>(c[i]);
  }
  cout << endl;

  crc.reset();
  crc.push(key, ::strlen(key));
  crc.push(&c[0], 1);
  crc.push(&c[1], 1);
  crc.push(&c[2], 1);
  crc.push(&c[3], 1);
  crc.crc(c);

  for (int i = 0; i < 4; ++i) {
    cout << std::hex << std::setw(2) << std::setfill('0')
         << static_cast<int>(c[i]);
  }
  cout << endl;

  crc.reset();
  crc.push(key2, ::strlen(key2));
  crc.crc(c);

  cout << "CRC ";
  for (int i = 0; i < 4; ++i) {
    cout << std::hex << std::setw(2) << std::setfill('0')
         << static_cast<int>(c[i]);
  }
  cout << endl;

  //  printf("%x\n", crc32((const unsigned char*)key, ::strlen( key )));

  {
    uint8_t test[] = {0x58, 0x85, 0x01, 0x23};
    CRC<CRC_8_ATM> crc8atm;
    crc8atm.push(test, 4);
    uint8_t result;
    crc8atm.crc(&result);
    cout << "CRC-8-ATM results should be 0x7b, result is " << std::hex
         << std::setw(2) << std::setfill('0') << static_cast<int>(result)
         << endl;

    CRC<CRC_8_ATM>::crc(&result, test, 4);
    cout << "CRC-8-ATM results should be 0x7b, result is " << std::hex
         << std::setw(2) << std::setfill('0') << static_cast<int>(result)
         << endl;
  }

  {
    uint8_t test[] = {0x03, 0x41, 0x42, 0x43};
    CRC<CRC_16_CCITT> crc16ccitt;
    crc16ccitt.push(test, 4);
    uint8_t result[2];
    crc16ccitt.crc(result);
    cout << "CRC-16-CCITT results should be 0x26 0x88, result is " << std::hex
         << std::setw(2) << std::setfill('0') << static_cast<int>(result[0])
         << " " << static_cast<int>(result[1]) << endl;

    CRC<CRC_16_CCITT>::crc(result, test, 4);

    cout << "CRC-16-CCITT results should be 0x26 0x88, result is " << std::hex
         << std::setw(2) << std::setfill('0') << static_cast<int>(result[0])
         << " " << static_cast<int>(result[1]) << endl;
  }

  {
    uint8_t test[] = {0x03, 0x41, 0x42, 0x43, 0x00, 0x00, 0x00, 0x00};
    CRC<CRC_32> crc32;
    crc32.push(test, 4);
    uint8_t result[4];
    crc32.crc(result);

    cout << "CRC32 results should be 0x6F 0x33 0xAA 0xA8, result is "
         << std::hex << std::setw(2) << std::setfill('0')
         << static_cast<int>(result[0]) << " " << static_cast<int>(result[1])
         << " " << static_cast<int>(result[2]) << " "
         << static_cast<int>(result[3]) << endl;

    CRC<CRC_32>::crc(result, test, 4);

    cout << "CRC32 results should be 0x6F 0x33 0xAA 0xA8, result is "
         << std::hex << std::setw(2) << std::setfill('0')
         << static_cast<int>(result[0]) << " " << static_cast<int>(result[1])
         << " " << static_cast<int>(result[2]) << " "
         << static_cast<int>(result[3]) << endl;

    for (int i = 0; i < 4; ++i) {
      test[4 + i] = result[3 - i];
    }

    cout << "is the result valid?: " << CRC<CRC_32>::check(result, test, 4)
         << endl;

    CRC<CRC_32>::crc(result, test, 8);
    cout << "CRC32 check should be 0x21 0x44 0xDF 0x1C, result is " << std::hex
         << std::setw(2) << std::setfill('0') << static_cast<int>(result[0])
         << " " << static_cast<int>(result[1]) << " "
         << static_cast<int>(result[2]) << " " << static_cast<int>(result[3])
         << endl;
  }

  return 0;
}
