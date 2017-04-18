/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <FFT.h>

#include <iostream>

using namespace std;
using namespace cotave;

int main() {
  const int exponent = 9;
  const int size = (1 << exponent);

  double data[size];

  double amp[3] = {1.0, 10.0, 2.0};
  double freq[3] = {5.0, 20.0, 100.0};
  double phase[3] = {M_PI / 6.0, M_PI / 2.0, M_PI / 3.0};

  for (int i = 0; i < size; ++i) {
    data[i] = 0.0;
    for (int j = 0; j < 3; ++j) {
      data[i] += amp[j] * sin(2.0 * M_PI * freq[j] * i / size + phase[j]);
    }
  }

  cout << "size: " << size << endl;

  FFT<exponent> fft(data);

  cout << "exponent: " << exponent << endl;
  cout << "DATA_SIZE: " << FFT<exponent>::DATA_SIZE << endl;

  std::complex<double> fft_data[FFT<exponent>::DATA_SIZE];
  std::complex<double> reg_data[FFT<exponent>::DATA_SIZE];
  double power_spectrum_data[FFT<exponent>::DATA_SIZE];
  double power_spectrum_dB_data[FFT<exponent>::DATA_SIZE];
  double linear_spectrum_data[FFT<exponent>::DATA_SIZE];

  for (int i = 0; i < FFT<exponent>::DATA_SIZE; ++i) {
    fft_data[i] = fft(i);
  }

  fft.normalize();

  for (int i = 0; i < FFT<exponent>::DATA_SIZE; ++i) {
    reg_data[i] = fft(i);
  }

  fft.powerSpectrum(power_spectrum_data);
  fft.powerSpectrum(power_spectrum_dB_data, true);
  fft.linearSpectrum(linear_spectrum_data);

  fft.denormalize();

  IFFT<exponent> ifft(fft.data());

  for (int i = 0; i < size; ++i) {
    cout << data[i] << ", " << fft(i) << ", " << ifft(i).real() << ", "
         << reg_data[i] << ", " << power_spectrum_data[i] << ", "
         << linear_spectrum_data[i] << ", " << power_spectrum_dB_data[i]
         << endl;
  }

  return 0;
}
