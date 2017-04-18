/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>

#include <ButterworthFilter.h>

using namespace std;
using namespace cotave;

int main() {
  double freq = 0.0;

  while (freq < 50.0) {
    ButterworthFilter bf(2, 1000.0, 10.0);
    double max = 0.0;
    double t = 0.0;

    bf.initialize(0.0);

    while (t < 5.0) {
      double raw = sin(2.0 * M_PI * freq * t);
      bf.update(raw);

      if (max < fabs(bf.getOutput())) {
        max = fabs(bf.getOutput());
      }
      t += 0.001;
    }

    // cout << freq << ", " << max << ", " << (20.0 * log10(max)) << endl;

    freq += 1.0;
  }

  double inputs[1000];
  double outputs[1000];

  for (int i = 0; i < 1000; ++i) {
    inputs[i] = sin(2.0 * M_PI * 10.0 * i / 1000);
  }

  ButterworthFilter::zeroPhaseFilter(outputs, inputs, 1000, 5, 1000, 10);
  ButterworthFilter bf(5, 1000, 10);

  for (int i = 0; i < 1000; ++i) {
    bf.update(inputs[i]);
    cout << inputs[i] << ", " << bf.getOutput() << "," << outputs[i] << endl;
  }

  return 0;
}
