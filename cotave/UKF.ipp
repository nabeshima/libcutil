/**
 * $Id: UKF.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "CHOL.h"

namespace cotave {

template <int M, int N>
const double UKF<M, N>::kappa = 3.0 - M;  // Gaussian assumption.

template <int M, int N>
const double UKF<M, N>::w0 = kappa / (M + kappa);

template <int M, int N>
const double UKF<M, N>::wi = 0.5 / (M + kappa);

template <int M, int N>
const double UKF<M, N>::lambda = ::sqrt(M + kappa);

template <int M, int N>
UKF<M, N>::UKF(const CColumnVector<M> &x0, const CMatrix<M, M> &P0)
    : mx(x0) {
  CHOLm<M> cholm(P0);
  mL = cholm.L();
  mD = cholm.D();
  for (int i = 0; i < M; ++i) {
    mD(i, i) = ::sqrt(mD(i, i));
  }
}

template <int M, int N>
template <typename EVOLUTION_FUNC, typename OBSERVE_FUNC>
inline void UKF<M, N>::update(const EVOLUTION_FUNC &F, const OBSERVE_FUNC &H,
                              const CColumnVector<M> &Q,
                              const CColumnVector<N> &R,
                              const CColumnVector<N> &observed_y) {
  //-------- state estimation ------------
  {
    // Calculate sigma points (chi)
    CMatrix<M, M> S(mL * mD);

    CColumnVector<M> chi[2 * M + 1];
    chi[2 * M] = mx;
    for (int i = 0; i < M; ++i) {
      CColumnVector<M> tmp(lambda * S.col(i));

      chi[2 * i] = F(mx + tmp);
      chi[2 * i + 1] = F(mx - tmp);
    }

    // Update state mean
    mx = w0 * chi[2 * M];
    for (int i = 0; i < 2 * M; ++i) {
      mx += wi * chi[i];
    }

    // Update state covariance
    CMatrix<M, M> P(Q.data(), true);
    P += w0 * (chi[2 * M] - mx) * (chi[2 * M] - mx).transpose();
    for (int i = 0; i < 2 * M; ++i) {
      P += wi * (chi[i] - mx) * (chi[i] - mx).transpose();
    }

    CHOLm<M> cholm(P);
    mL = cholm.L();
    mD = cholm.D();
    for (int i = 0; i < M; ++i) {
      mD(i, i) = ::sqrt(mD(i, i));
    }
  }

  //-------- measurement estimation ------------
  for (int j = 0; j < N; ++j) {
    // Calculate sigma points (chi)
    CMatrix<M, M> S(mL * mD);

    CColumnVector<M> chi[2 * M];
    for (int i = 0; i < M; ++i) {
      CColumnVector<M> tmp(lambda * S.col(i));

      chi[2 * i] = (mx + tmp);
      chi[2 * i + 1] = (mx - tmp);
    }

    // Estimate projected points (eta)
    CColumnVector<N> my(H(mx));
    CColumnVector<N> eta[2 * M];
    for (int i = 0; i < 2 * M; ++i) {
      eta[i] = H(chi[i]);
    }

    // Calculate observation covariance
    double Py = R(j);
    for (int i = 0; i < 2 * M; ++i) {
      Py += wi * (eta[i] - my).absolute2();
    }

    // Calculate mutual covariance
    CMatrix<M, N> Pxy(0.0);
    for (int i = 0; i < 2 * M; ++i) {
      Pxy += wi * (chi[i] - mx) * (eta[i] - my).transpose();
    }

    // Calculate Kalman gain
    CMatrix<M, N> K(Pxy / Py);

    // Update state mean
    mx += K * (observed_y - my);

    // Update state covariance
    CMatrix<M, M> P(mL * mD * mD * mL.transpose() - Py * K * K.transpose());

    CHOLm<M> cholm(P);
    mL = cholm.L();
    mD = cholm.D();
    for (int i = 0; i < M; ++i) {
      mD(i, i) = ::sqrt(mD(i, i));
    }
  }
}

template <int M, int N>
inline const CColumnVector<M> &UKF<M, N>::x() const {
  return mx;
}
}
