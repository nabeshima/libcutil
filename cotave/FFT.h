/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_FFT_H_
#define _COTAVE_FFT_H_

#include <complex>

namespace cotave {

template <int N>
class _power_of_2 {
 public:
  static const int value = _power_of_2<N - 1>::value * 2;
};

template <>
class _power_of_2<0> {
 public:
  static const int value = 1;
};

/*
 * FFT functions
 * Reference: http://d.hatena.ne.jp/yatt/20080331/1206959305
 * */

template <int EXPONENT_OF_2>
class FFT {
 public:
  static const int DATA_SIZE = _power_of_2<EXPONENT_OF_2>::value;

 private:
  std::complex<double> d[DATA_SIZE];
  bool normalized_flag;

 protected:
  explicit FFT(const std::complex<double> *input);  // for IFFT
  void calculate(bool is_inverse);

 public:
  explicit FFT(const double *input);
  FFT(const FFT<EXPONENT_OF_2> &f);
  FFT<EXPONENT_OF_2> &operator=(const FFT<EXPONENT_OF_2> &f);

  std::complex<double> *data();
  std::complex<double> &elem(int index);

  const std::complex<double> *data() const;
  const std::complex<double> &elem(int index) const;

  std::complex<double> &operator[](int index);
  std::complex<double> &operator()(int index);

  const std::complex<double> &operator()(int index) const;
  const std::complex<double> &operator[](int index) const;

  bool isNormalized() const;
  void normalize();
  void denormalize();

  void powerSpectrum(double *output, bool decibel_flag = false) const;
  void linearSpectrum(double *output) const;
};

template <int EXPONENT_OF_2>
class IFFT : public FFT<EXPONENT_OF_2> {
 private:
  bool isNormalized() const;
  void normalize();
  void denormalize();

  void powerSpectrum(double *output, bool decibel_flag) const;
  void linearSpectrum(double *output) const;

 public:
  explicit IFFT(const std::complex<double> *input);
  IFFT(const IFFT<EXPONENT_OF_2> &f);
  IFFT<EXPONENT_OF_2> &operator=(const IFFT<EXPONENT_OF_2> &f);
};

}  // namespace cotave

#include "FFT.ipp"

#endif /* _COTAVE_FFT_H_ */
