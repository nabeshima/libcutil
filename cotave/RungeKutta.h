/**
 * $Id: RungeKutta.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_RungeKutta_H_
#define _COTAVE_RungeKutta_H_

namespace cotave {

//! ルンゲ=クッタの数値積分
class RungeKutta {
 public:
  template <typename GRAD_FUNC, typename PARAM_TYPE>
  static PARAM_TYPE step(const GRAD_FUNC &f, PARAM_TYPE y, double x,
                         double x_step);
};
}

#include "RungeKutta.ipp"

#endif
