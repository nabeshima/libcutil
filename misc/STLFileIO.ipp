/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <inttypes.h>
#include <cstring>

#include <fstream>

namespace cutil {

bool STLFileIO::write( const std::string &filename,
                       const std::vector< cotave::ColumnVector3 > &vertices,
                       const std::vector< cotave::IntVector3 > &indices ) {
  return true;  
}

bool STLFileIO::read( const std::string &filename,
                      std::vector< cotave::ColumnVector3 > &vertices,
                      std::vector< cotave::IntVector3 > &indices,
		      bool inversion ) {

  vertices.clear();
  indices.clear();

  // テキストとバイナリを自動判定.
  std::ifstream inf;
  inf.open( filename.c_str(), std::ios::binary );
  
  char info[ 6 ];
  info[ 5 ] = '\0';
  inf.read( info, 5 );
  
  if ( ::strcmp( info, "solid" ) == 0 ) {
    // テキスト形式
    std::string line;
    std::getline( inf, line ); // コメント

    std::string facet, normal, vertex, outer, loop, endloop, endfacet;
    cotave::ColumnVector3 a, b, c, n;
    
    while ( inf ) {
      inf >> facet >> normal >> n 
          >> outer >> loop
          >> vertex >> a
          >> vertex >> b
          >> vertex >> c
          >> endloop
          >> endfacet;

      if ( facet == "facet" &&
           normal == "normal" &&
           outer == "outer" &&
           loop == "loop" &&
           vertex == "vertex" &&
           endloop == "endloop" &&
           endfacet == "endfacet" ) {
        
        int size = vertices.size();
        vertices.push_back( a );
        vertices.push_back( b );
        vertices.push_back( c );

	if ( inversion ) {
	  indices.push_back( cotave::IntVector3( size, size + 2, size + 1 ) );
	}
	else {
	  indices.push_back( cotave::IntVector3( size, size + 1, size + 2 ) );
	}
      }
    }
  }
  else {
    // バイナリ形式
    inf.seekg( 0 );
    
    uint8_t header[ 80 ];
    inf.read( (char*)header, 80 * sizeof( uint8_t ) );
    
    uint32_t facenum;
    inf.read( (char*)&facenum, sizeof( uint32_t ) );
    
    float data[ 12 ];
    uint16_t padding = 0;
    
    while ( inf ) {
      inf.read( (char*)data, 12 * sizeof( float ) );
      inf.read( (char*)&padding, sizeof( uint16_t ) );
      
      cotave::ColumnVector3 
        n( data[ 0 ], data[ 1 ], data[ 2 ] ),
        a( data[ 3 ], data[ 4 ], data[ 5 ] ),
        b( data[ 6 ], data[ 7 ], data[ 8 ] ),
        c( data[ 9 ], data[ 10 ], data[ 11 ] );

      int size = vertices.size();
      vertices.push_back( a );
      vertices.push_back( b );
      vertices.push_back( c );
      
      if ( inversion ) {
	indices.push_back( cotave::IntVector3( size, size + 2, size + 1 ) );
      }
      else {
	indices.push_back( cotave::IntVector3( size, size + 1, size + 2 ) );
      }
    }
  }
  
  inf.close();

  return true;
}

}
