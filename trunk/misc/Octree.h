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
  octree����ƥ�.
  
  �⡼�ȥ��������Ѥ���.
  ��ɸ�ϳƼ�0���������2^MAX_LEVEL-1�ޤ�.
  MAX_LEVEL��8�ޤ�.
*/

template< class T, unsigned int MAX_LEVEL = 8 >
class Octree {
private:
  //! Ϣ��������. �⡼�ȥ��ֹ�ȼ������index�ȤΥޥå�. ��٥뤴�Ȥ˺��.
  std::map< unsigned int, unsigned int > indices[ MAX_LEVEL ];
  
  //! ����.
  std::vector< T > instances[ MAX_LEVEL ];
  
public:
  //! �⡼�ȥ��ֹ椬�ɤΥ�٥���ֹ椫���֤�.
  static int level( unsigned int morton_number );
  
  //! ��ɸ����, �⡼�ȥ��ֹ���֤�.
  static unsigned int mortonNumber( unsigned char ix, unsigned char iy, unsigned char iz );

  //! �⡼�ȥ��ֹ椫��, ��ɸ���᤹.
  static void position( unsigned int morton_number, unsigned char &ix, unsigned char &iy, unsigned char &iz );
  
  //! �⡼�ȥ��ֹ��������˥�٥륷�եȤ���.
  static unsigned int up( unsigned int morton_number );
  
  //! �⡼�ȥ��ֹ�������˥�٥륷�եȤ���. �����[0, 7]��­���ƻȤ�.
  static unsigned int down( unsigned int morton_number );
  
  //! Ʊ����٥��2�ĤΥ⡼�ȥ��ֹ�Ǽ�������ΰ��ޤ��ΰ�Υ⡼�ȥ��ֹ���֤�.
  static unsigned int enclose( unsigned int morton_numberA, unsigned int morton_numberB );
  
  //! ���Τβ���
  void clear();
  
  //! ���Τγ������
  T& assignInstance( unsigned int morton_number );  
  
  //! ���Τγ������
  T& assignInstance( unsigned int level, unsigned char ix, unsigned char iy, unsigned char iz );  
  
  T& operator[]( unsigned int morton_number );
  T& operator()( unsigned int morton_number );
  T& operator()( unsigned int level, unsigned char ix, unsigned char iy, unsigned char iz );
  
  //! ���Τ����ǿ�.
  int size( int level = -1 ) const;

  //! ������index�Υޥåפ����.
  const std::map< unsigned int, unsigned int >& indexMap( unsigned int level ) const;
  
  //! ���Τ�vector�����.
  const std::vector< T >& instanceVector( unsigned int level ) const;
};

}

#include "Octree.ipp"

#endif
