/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace {
inline void _bit_reverse(std::complex<double> *output, int size) {
  for (int i = 0; i < size; ++i) {
    unsigned int k = 0;
    unsigned int b = (size >> 1);
    unsigned int a = 1;

    while (b >= a) {
      if (b & i) {
        k |= a;
      }
      if (a & i) {
        k |= b;
      }
      b >>= 1;
      a <<= 1;
    }
    if (i < k) {
      std::complex<double> tmp = output[k];
      output[k] = output[i];
      output[i] = tmp;
    }
  }
}
}

namespace cotave {

template <int EXPONENT_OF_2>
inline FFT<EXPONENT_OF_2>::FFT(const std::complex<double> *input)
    : normalized_flag(false) {
  for (int i = 0; i < DATA_SIZE; ++i) {
    elem(i) = input[i];
  }
  calculate(true);
}

template <int EXPONENT_OF_2>
inline void FFT<EXPONENT_OF_2>::calculate(bool is_inverse) {
  _bit_reverse(data(), DATA_SIZE);

  std::complex<double> W, ww, t;

  for (int m = 2; m <= DATA_SIZE; m *= 2) {
    double theta = -2.0 * M_PI / m;
    std::complex<double> w = std::complex<double>(cos(theta), sin(theta));
    if (is_inverse) {
      w = std::complex<double>(1.0, 0) / w;
    }

    for (int i = 0; i < DATA_SIZE; i += m) {
      std::complex<double> ww(1.0, 0);
      int half = m / 2;

      for (int j = 0; j < half; ++j) {
        int a = i + j;
        int b = i + j + half;

        std::complex<double> t = ww * elem(b);
        elem(b) = elem(a) - t;
        elem(a) = elem(a) + t;

        ww *= w;
      }
    }
  }
}

template <int EXPONENT_OF_2>
inline FFT<EXPONENT_OF_2>::FFT(const double *input) : normalized_flag(false) {
  for (int i = 0; i < DATA_SIZE; ++i) {
    elem(i).real(input[i]);
    elem(i).imag(0.0);
  }
  calculate(false);
}

template <int EXPONENT_OF_2>
inline FFT<EXPONENT_OF_2>::FFT(const FFT<EXPONENT_OF_2> &f)
    : normalized_flag(f.normalized_flag) {
  for (int i = 0; i < DATA_SIZE; ++i) {
    elem(i) = f(i);
  }
}

template <int EXPONENT_OF_2>
inline FFT<EXPONENT_OF_2> &FFT<EXPONENT_OF_2>::operator=(
    const FFT<EXPONENT_OF_2> &f) {
  normalized_flag = f.normalized_flag;
  for (int i = 0; i < DATA_SIZE; ++i) {
    elem(i) = f(i);
  }
  return *this;
}

template <int EXPONENT_OF_2>
inline std::complex<double> *FFT<EXPONENT_OF_2>::data() {
  return d;
}
template <int EXPONENT_OF_2>
inline std::complex<double> &FFT<EXPONENT_OF_2>::elem(int index) {
  return d[index];
}

template <int EXPONENT_OF_2>
inline const std::complex<double> *FFT<EXPONENT_OF_2>::data() const {
  return d;
}

template <int EXPONENT_OF_2>
inline const std::complex<double> &FFT<EXPONENT_OF_2>::elem(int index) const {
  return d[index];
}

template <int EXPONENT_OF_2>
inline std::complex<double> &FFT<EXPONENT_OF_2>::operator[](int index) {
  return elem(index);
}

template <int EXPONENT_OF_2>
inline std::complex<double> &FFT<EXPONENT_OF_2>::operator()(int index) {
  return elem(index);
}

template <int EXPONENT_OF_2>
inline const std::complex<double> &FFT<EXPONENT_OF_2>::operator()(
    int index) const {
  return elem(index);
}

template <int EXPONENT_OF_2>
inline const std::complex<double> &FFT<EXPONENT_OF_2>::operator[](
    int index) const {
  return elem(index);
}

template <int EXPONENT_OF_2>
inline bool FFT<EXPONENT_OF_2>::isNormalized() const {
  return normalized_flag;
}

template <int EXPONENT_OF_2>
inline void FFT<EXPONENT_OF_2>::normalize() {
  if (!normalized_flag) {
    double a = 2.0 / DATA_SIZE;
    for (int i = 1; i < DATA_SIZE; ++i) {
      elem(i) *= a;
    }
    elem(0) /= 2.0;
    normalized_flag = true;
  }
}

template <int EXPONENT_OF_2>
inline void FFT<EXPONENT_OF_2>::denormalize() {
  if (normalized_flag) {
    double b = static_cast<double>(DATA_SIZE) / 2.0;
    for (int i = 1; i < DATA_SIZE; ++i) {
      elem(i) *= b;
    }
    elem(0) *= 2.0;
    normalized_flag = false;
  }
}

template <int EXPONENT_OF_2>
inline void FFT<EXPONENT_OF_2>::powerSpectrum(double *output,
                                              bool decibel_flag) const {
  FFT<EXPONENT_OF_2> tmp(*this);
  tmp.normalize();

  if (!decibel_flag) {
    for (int i = 0; i < DATA_SIZE; ++i) {
      output[i] = (tmp(i) * std::conj(tmp(i))).real();
    }
  } else {
    for (int i = 0; i < DATA_SIZE; ++i) {
      output[i] = 10.0 * log10((tmp(i) * std::conj(tmp(i))).real());
    }
  }
}

template <int EXPONENT_OF_2>
inline void FFT<EXPONENT_OF_2>::linearSpectrum(double *output) const {
  FFT<EXPONENT_OF_2> tmp(*this);
  tmp.normalize();

  for (int i = 0; i < DATA_SIZE; ++i) {
    output[i] = sqrt((tmp(i) * std::conj(tmp(i))).real());
  }
}

template <int EXPONENT_OF_2>
inline IFFT<EXPONENT_OF_2>::IFFT(const std::complex<double> *input)
    : FFT<EXPONENT_OF_2>(input) {
  double a = 1.0 / FFT<EXPONENT_OF_2>::DATA_SIZE;
  for (int i = 0; i < FFT<EXPONENT_OF_2>::DATA_SIZE; ++i) {
    FFT<EXPONENT_OF_2>::elem(i) *= a;
  }
}

template <int EXPONENT_OF_2>
inline IFFT<EXPONENT_OF_2>::IFFT(const IFFT<EXPONENT_OF_2> &f)
    : FFT<EXPONENT_OF_2>(f) {}

template <int EXPONENT_OF_2>
inline IFFT<EXPONENT_OF_2> &IFFT<EXPONENT_OF_2>::operator=(
    const IFFT<EXPONENT_OF_2> &f) {
  return FFT<EXPONENT_OF_2>::operator=(f);
}
}
