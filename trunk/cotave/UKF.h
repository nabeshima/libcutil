/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_UKF_H_
#define _COTAVE_UKF_H_


#include "CMatrix.h"


namespace cotave {

/*!
  \brief UKF: Unscented Kalman Filter
  
  M: 状態ベクトルの次元
  N: 観測ベクトルの次元
*/
template< int M, int N >
class UKF {
private:
  static const double kappa;
  static const double w0;
  static const double wi;
  static const double lambda;
  
  CColumnVector< M > mx;
  CMatrix< M, M > mL;
  CMatrix< M, M > mD;
  
public:

  /*!
    \brief UKFの初期設定
    
    x0: 状態変数の平均の初期値
    P0: 状態変数の分散の初期値
*/
  UKF( const CColumnVector< M > &x0, 
       const CMatrix< M, M > &P0 );
  
  /*!
    \brief UKFの更新
    
    EVOLUTION_FUNC: 発展関数オブジェクト
    OBSERVE_FUNC  : 観測関数オブジェクト
    
    Q: 発展ノイズの共分散行列. 対角成分. 対角行列を仮定.
    R: 観測ノイズの共分散行列. 対角成分. 対角行列を仮定.
  */
  template< typename EVOLUTION_FUNC,
	    typename OBSERVE_FUNC >
  void update( const EVOLUTION_FUNC &F,
               const OBSERVE_FUNC &H,
               const CColumnVector< M > &Q,
               const CColumnVector< N > &R,
               const CColumnVector< N > &observed_y );
  
  //! 状態の推定値
  const CColumnVector< M >& x() const;
};

}


#include "UKF.ipp"


#endif
