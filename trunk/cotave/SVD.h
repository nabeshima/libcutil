/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_SVD_H_
#define _COTAVE_SVD_H_


#include "CMatrix.h"


namespace cotave {

//! 特異値分解(Singular Value Decomposition). mat = U * S * V'
template< int ROWS, int COLS, typename DOUBLE = double >
class SVD {
private:
  CMatrix< ROWS, ROWS, DOUBLE > mU;
  CMatrix< ROWS, COLS, DOUBLE > mS;
  CMatrix< COLS, COLS, DOUBLE > mV;
  
public:
  SVD( const CMatrix< ROWS, COLS, DOUBLE > &mat );
  const CMatrix< ROWS, ROWS, DOUBLE >& U() const;
  const CMatrix< ROWS, COLS, DOUBLE >& S() const;
  const CMatrix< COLS, COLS, DOUBLE >& V() const;
};


}


#include "SVD.ipp"


#endif
