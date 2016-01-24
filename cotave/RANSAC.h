/**
 * $Id: RANSAC.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_RANSAC_H_
#define _COTAVE_RANSAC_H_

#include <vector>

namespace cotave {

//! ロバスト推定 RANSAC
class RANSAC {
 public:
  /*!
    実行

    関数内部でdataの順番が入れ替わる.
  */
  template <class ESTIMATOR, typename DATA_TYPE, typename ERROR_FUNC>
  static bool run(ESTIMATOR *result, std::vector<DATA_TYPE> *data,
                  unsigned int sampling_number, unsigned int estimation_times,
                  const ERROR_FUNC &error, double error_threshold = 0.0);
};
}  // namespace cotave

#include "RANSAC.ipp"

#endif
