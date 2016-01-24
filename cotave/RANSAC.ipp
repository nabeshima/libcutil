/**
 * $Id: RANSAC.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <tr1/random>

namespace cotave {

template <class ESTIMATOR, typename DATA_TYPE, typename ERROR_FUNC>
bool RANSAC::run(ESTIMATOR *result, std::vector<DATA_TYPE> *data,
                 unsigned int sampling_number, unsigned int estimation_times,
                 const ERROR_FUNC &error, double error_threshold) {
  ESTIMATOR &_result = *result;
  std::vector<DATA_TYPE> &_data = *data;

  if (_data.size() < sampling_number) {
    return false;
  }

  // スレショルドを3シグマに.
  bool auto_threshold = (error_threshold <= 0.0);

  // 乱数生成器
  std::tr1::mt19937 rand(static_cast<unsigned long>(::time(0)));

  // パラメータ推定器
  ESTIMATOR *estimator = new ESTIMATOR[estimation_times];
  unsigned int vote[estimation_times];

  for (unsigned int et = 0; et < estimation_times; ++et) {
    for (unsigned int sp = 0; sp < sampling_number; ++sp) {
      int index = rand() % (_data.size() - sp);
      DATA_TYPE tmp(_data[index]);
      estimator[et].accumulate(tmp);
      _data[index] = _data[_data.size() - 1 - sp];
      _data[_data.size() - 1 - sp] = tmp;
    }

    estimator[et].calculate();

    if (auto_threshold) {
      error_threshold = estimator[et].error() * 1.73205081;
    }

    vote[et] = 0;
    for (unsigned int i = 0; i < _data.size(); ++i) {
      if (error(estimator[et], _data[i]) < error_threshold) {
        ++vote[et];
      }
    }

    //    std::cout << "eth " << et << ": "<< error_threshold << ", vote: " <<
    //    vote[ et ] << std::endl;
  }

  unsigned int max_vote = 0;
  ESTIMATOR *max_et;

  for (unsigned int et = 0; et < estimation_times; ++et) {
    if (vote[et] > max_vote) {
      max_vote = vote[et];
      max_et = &estimator[et];
    }
  }

  if (max_vote < sampling_number) {
    delete[] estimator;
    return false;
  }

  if (auto_threshold) {
    error_threshold = max_et->error() * 1.73205081;
  }

  for (unsigned int i = 0; i < _data.size(); ++i) {
    if (error(*max_et, _data[i]) < error_threshold) {
      _result.accumulate(_data[i]);
    }
  }
  _result.calculate();

  delete[] estimator;
  return true;
}
}
