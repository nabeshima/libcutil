/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _COTAVE_CHOL_H_
#define _COTAVE_CHOL_H_


#include "CMatrix.h"


namespace cotave {

//! コレスキー分解(Cholesky decomposition). mat = L * L'
template< int DIM, typename DOUBLE = double >
class CHOL {
protected:
  CMatrix< DIM, DIM, DOUBLE > mL;
  
public:
  CHOL( const CMatrix< DIM, DIM, DOUBLE > &mat );
  const CMatrix< DIM, DIM, DOUBLE >& L() const;
};

//! 修正コレスキー分解(Modified cholesky decomposition). mat = L * D * L'
template< int DIM, typename DOUBLE = double >
class CHOLm: public CHOL< DIM, DOUBLE > {
private:
  CMatrix< DIM, DIM, DOUBLE > mD;
  
public:
  CHOLm( const CMatrix< DIM, DIM, DOUBLE > &mat );
  const CMatrix< DIM, DIM, DOUBLE >& D() const;
};

}


#include "CHOL.ipp"


#endif
