/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_RungeKutta_H_
#define _COTAVE_RungeKutta_H_

namespace cotave {

//! ルンゲ=クッタの数値積分
class RungeKutta {
public:
  template< typename GRAD_FUNC, 
            typename PARAM_TYPE >
  static PARAM_TYPE step( const GRAD_FUNC &f,
                          PARAM_TYPE y,
                          double x, 
                          double x_step );
};

}

#include "RungeKutta.ipp"

#endif
