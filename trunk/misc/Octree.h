/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_OCTREE_H_
#define _CUTIL_OCTREE_H_

#include <map>
#include <vector>


namespace cutil {
  
/*!
  octreeコンテナ.
  
  モートン順序を利用する.
  座標は各軸0から最大値2^MAX_LEVEL-1まで.
  MAX_LEVELは8まで.
*/

template< class T, unsigned int MAX_LEVEL = 8 >
class Octree {
private:
  //! 連想配列用. モートン番号と実配列のindexとのマップ. レベルごとに作る.
  std::map< unsigned int, unsigned int > indices[ MAX_LEVEL ];
  
  //! 実体.
  std::vector< T > instances[ MAX_LEVEL ];
  
public:
  //! モートン番号がどのレベルの番号かを返す.
  static int level( unsigned int morton_number );
  
  //! 座標から, モートン番号を返す.
  static unsigned int mortonNumber( unsigned char ix, unsigned char iy, unsigned char iz );

  //! モートン番号から, 座標に戻す.
  static void position( unsigned int morton_number, unsigned char &ix, unsigned char &iy, unsigned char &iz );
  
  //! モートン番号を上方向にレベルシフトする.
  static unsigned int up( unsigned int morton_number );
  
  //! モートン番号を下方向にレベルシフトする. これに[0, 7]を足して使う.
  static unsigned int down( unsigned int morton_number );
  
  //! 同じレベルの2つのモートン番号で示される領域を含む領域のモートン番号を返す.
  static unsigned int enclose( unsigned int morton_numberA, unsigned int morton_numberB );
  
  //! 実体の解放
  void clear();
  
  //! 実体の割り当て
  T& assignInstance( unsigned int morton_number );  
  
  //! 実体の割り当て
  T& assignInstance( unsigned int level, unsigned char ix, unsigned char iy, unsigned char iz );  
  
  T& operator[]( unsigned int morton_number );
  T& operator()( unsigned int morton_number );
  T& operator()( unsigned int level, unsigned char ix, unsigned char iy, unsigned char iz );
  
  //! 実体の要素数.
  int size( int level = -1 ) const;

  //! キーとindexのマップを取得.
  const std::map< unsigned int, unsigned int >& indexMap( unsigned int level ) const;
  
  //! 実体のvectorを取得.
  const std::vector< T >& instanceVector( unsigned int level ) const;
};

}

#include "Octree.ipp"

#endif
