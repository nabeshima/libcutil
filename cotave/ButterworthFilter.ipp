/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Polynomial.h"

#include <cmath>

namespace cotave {

ButterworthFilter::ButterworthFilter(unsigned int order,
                                     double sampling_frequency,
                                     double cutoff_frequency)
    : order(order),
      x_coeff(new double[order + 1]),
      y_coeff(new double[order + 1]),
      x(new double[order + 1]),
      y(new double[order + 1]) {
  for (int i = 0; i < order + 1; ++i) {
    x_coeff[i] = 0.0;
    y_coeff[i] = 0.0;
    x[i] = 0.0;
    y[i] = 0.0;
  }

  // Poles are placed on the unit circle. All poles are stable (the real parts
  // are negative). Polynomials for s
  const double one[1] = {1.0};
  Polynomial<double> den(one, 1);

  int k_size = (order % 2 == 0) ? order / 2 : (order - 1) / 2;
  for (int k = 0; k < k_size; ++k) {
    const double val[3] = {
        1.0, 2.0 * cos((double)(order - 2 * k - 1) * M_PI / (2.0 * order)),
        1.0};

    den *= Polynomial<double>(val, 3);
  }
  if (order % 2 == 1) {
    const double val[2] = {1.0, 1.0};
    den *= Polynomial<double>(val, 2);
  }

  // Bilinear transform s <-  { (1 -  z^{-1}) / (1 + z^{-1} ) } / a
  // a = tan(M_PI * cutoff_frequency / sampling_frequency),

  double a = tan(M_PI * cutoff_frequency / sampling_frequency);

  Polynomial<double> ret_den;

  const double init_z1[2] = {1.0, -1.0};
  Polynomial<double> z1(init_z1, 2);

  const double init_z2[2] = {1.0 * a, 1.0 * a};
  Polynomial<double> z2(init_z2, 2);

  for (int i = 0; i < den.size(); ++i) {
    Polynomial<double> poly_z(one, 1);
    for (int k = 0; k < i; ++k) {
      poly_z *= z1;
    }
    for (int k = i; k < den.size() - 1; ++k) {
      poly_z *= z2;
    }

    ret_den += den(i) * poly_z;
  }

  Polynomial<double> ret_num(one, 1);
  for (int i = 1; i < den.size(); ++i) {
    ret_num *= z2;
  }

  ret_num /= ret_den(0);
  ret_den /= ret_den(0);

  for (int i = 0; i < order + 1; ++i) {
    x_coeff[i] = ret_num(i);
    y_coeff[i] = -ret_den(i);
  }
}

ButterworthFilter::~ButterworthFilter() {
  delete[] x_coeff;
  delete[] y_coeff;
  delete[] x;
  delete[] y;
}

void ButterworthFilter::initialize(double input) {
  for (int i = 0; i < order + 1; ++i) {
    x[i] = input;
    y[i] = input;
  }
}

void ButterworthFilter::update(double input) {
  for (int i = order; i > 0; --i) {
    x[i] = x[i - 1];
    y[i] = y[i - 1];
  }
  x[0] = input;

  double y0 = 0.0;
  for (int i = 0; i < order + 1; ++i) {
    y0 += x_coeff[i] * x[i];
  }
  for (int i = 1; i < order + 1; ++i) {
    y0 += y_coeff[i] * y[i];
  }

  y[0] = y0;
}

double ButterworthFilter::getOutput() const { return y[0]; }

unsigned int ButterworthFilter::getOrder() const { return order; }

double ButterworthFilter::getXCoeff(unsigned int i) const {
  if (i <= order) {
    return x_coeff[i];
  } else {
    return 0.0;
  }
}

double ButterworthFilter::getYCoeff(unsigned int i) const {
  if (i <= order) {
    return y_coeff[i];
  } else {
    return 0.0;
  }
}

void ButterworthFilter::zeroPhaseFilter(double *outputs, const double *inputs,
                                        unsigned int size, unsigned int order,
                                        double sampling_frequency,
                                        double cutoff_frequency) {
  ButterworthFilter bf(order, sampling_frequency, cutoff_frequency);

  // forward filtering
  bf.initialize(inputs[0]);
  for (int i = 1; i < size; ++i) {
    bf.update(inputs[i]);
    outputs[i] = bf.getOutput();
  }

  // backward filtering
  bf.initialize(outputs[size - 1]);
  for (int i = size - 2; i >= 0; --i) {
    bf.update(outputs[i]);
    outputs[i] = bf.getOutput();
  }
}

}  // namespace cotave
