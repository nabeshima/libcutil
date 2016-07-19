/**
 * $Id$
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef CUTIL_TYPE_TRAITS_H_
#define CUTIL_TYPE_TRAITS_H_

extern "C" {
#include <stdint.h>
}
#include <string>

namespace cutil {

template <bool B, class T = void>
struct enable_if {};

template <class T>
struct enable_if<true, T> {
  typedef T type;
};

template <class T>
struct is_numeric {
  static const bool value = false;
};
template <>
struct is_numeric<int8_t> {
  static const bool value = true;
};
template <>
struct is_numeric<uint8_t> {
  static const bool value = true;
};
template <>
struct is_numeric<int16_t> {
  static const bool value = true;
};
template <>
struct is_numeric<uint16_t> {
  static const bool value = true;
};
template <>
struct is_numeric<int32_t> {
  static const bool value = true;
};
template <>
struct is_numeric<uint32_t> {
  static const bool value = true;
};
template <>
struct is_numeric<int64_t> {
  static const bool value = true;
};
template <>
struct is_numeric<uint64_t> {
  static const bool value = true;
};
template <>
struct is_numeric<float> {
  static const bool value = true;
};
template <>
struct is_numeric<double> {
  static const bool value = true;
};

template <class T>
struct is_bool {
  static const bool value = false;
};
template <>
struct is_numeric<bool> {
  static const bool value = true;
};

template <class T>
struct is_string {
  static const bool value = false;
};
template <>
struct is_string<std::string> {
  static const bool value = true;
};

template <class T>
struct is_literal {
  static const bool value = false;
};
template <>
struct is_literal<std::string> {
  static const bool value = true;
};
template <>
struct is_literal<char> {
  static const bool value = true;
};
template <>
struct is_literal<char*> {
  static const bool value = true;
};
template <>
struct is_literal<const char*> {
  static const bool value = true;
};

template <class T, std::size_t sz>
struct is_literal<T[sz]> {
  static const bool value = false;
};
template <std::size_t sz>
struct is_literal<char[sz]> {
  static const bool value = true;
};
}  // namespace cutil

#endif  // CUTIL_TYPE_TRAITS_H_
