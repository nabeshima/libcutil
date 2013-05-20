/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>
#include <Octree.h>

using namespace std;
using namespace cutil;


int main() {
  const int LEVEL = 3;
  
  Octree< char, LEVEL > octree;
  
  for ( int i = 0; i < LEVEL; ++i ) {
    octree( i, 1, 7, 5 ) = 'a' + i;
  }
  
  for ( int i = 0; i < LEVEL; ++i ) {
    const std::map< unsigned int, unsigned int >
      &im = octree.indexMap( i );
    
    const std::vector< char >
      &iv = octree.instanceVector( i );

    std::map< unsigned int, unsigned int >::const_iterator
      it = im.begin(),
      endIt = im.end();
    
    while ( it != endIt ) {
      cout << "m:" << it->first 
           << ", lv:" << octree.level( it->first )
           << ", value:" << iv[ it->second ] << endl;
      
      unsigned int m = it->first;
      while ( m != 0 ) {
        cout << "m:" << m
             << ", lv:" << octree.level( m )
             << ", value:" << octree[ m ] << endl;
        m = octree.up( m );
      }
      cout << endl;
      
      ++it;
    }
  }   

  return 0;
}
