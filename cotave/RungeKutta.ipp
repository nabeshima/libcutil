/**
 * $Id: RungeKutta.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cotave {

template <typename GRAD_FUNC, typename PARAM_TYPE>
PARAM_TYPE RungeKutta::step(const GRAD_FUNC &f, PARAM_TYPE y, double x,
                            double x_step) {
  const double xs = x_step * 0.5;
  const PARAM_TYPE k1 = f(y, x), k2 = f(y + xs * k1, x + xs),
                   k3 = f(y + xs * k2, x + xs),
                   k4 = f(y + x_step * k3, x + x_step);

  return y + (k1 + k2 + k2 + k3 + k3 + k4) * x_step / 6.0;
}
}
