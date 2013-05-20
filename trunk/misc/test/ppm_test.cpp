/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "PPMFileIO.h"

#include <iostream>

using namespace std;
using namespace cutil;

int main() {
  {
    int W = 360;
    int H = 240;
  
    unsigned char image[ W * H * 3 ];
  
    for ( int i = 0; i < H; ++i ) {
      for ( int j = 0; j < W / 3; ++j ) {
        image[ ( i * W + j ) * 3 + 0 ] = 255;
        image[ ( i * W + j ) * 3 + 1 ] = 0;
        image[ ( i * W + j ) * 3 + 2 ] = 0;
      }
      for ( int j = W / 3; j < 2 * W / 3; ++j ) {
        image[ ( i * W + j ) * 3 + 0 ] = 0;
        image[ ( i * W + j ) * 3 + 1 ] = 255;
        image[ ( i * W + j ) * 3 + 2 ] = 0;
      }
      for ( int j = 2 * W / 3; j < W; ++j ) {
        image[ ( i * W + j ) * 3 + 0 ] = 0;
        image[ ( i * W + j ) * 3 + 1 ] = 0;
        image[ ( i * W + j ) * 3 + 2 ] = 255;
      }
    }
  
    PPMFileIO::write( "tmp.ppm~", image, W, H );
  }
  

  {
    int W;
    int H;
  
    unsigned char image[ 400 * 300 * 3 ];

    if ( !PPMFileIO::read( "tmp.ppm~", image, W, H ) ) {
      cerr << "cannot read ppm file" << endl;
      return -1;
    }

    if ( W != 360 || H != 240 ) {
      cerr << "bad header of ppm file" << endl;
      cout << "W: " << W << ", H: " << H << endl;
      return -1;
    }
    
    for ( int i = 0; i < H; ++i ) {
      for ( int j = 0; j < W / 3; ++j ) {
        if ( image[ ( i * W + j ) * 3 + 0 ] != 255 ||
             image[ ( i * W + j ) * 3 + 1 ] != 0 ||
             image[ ( i * W + j ) * 3 + 2 ] != 0 ) {
          cerr << "bad data of ppm file" << endl;
        }
      }
      for ( int j = W / 3; j < 2 * W / 3; ++j ) {
        if ( image[ ( i * W + j ) * 3 + 0 ] != 0 ||
             image[ ( i * W + j ) * 3 + 1 ] != 255 ||
             image[ ( i * W + j ) * 3 + 2 ] != 0 ) {
          cerr << "bad data of ppm file" << endl;
        }
      }
      for ( int j = 2 * W / 3; j < W; ++j ) {
        if ( image[ ( i * W + j ) * 3 + 0 ] != 0 ||
             image[ ( i * W + j ) * 3 + 1 ] != 0 ||
             image[ ( i * W + j ) * 3 + 2 ] != 255 ) {
          cerr << "bad data of ppm file" << endl;
        }
      }
    }
  }
  
  return 0;
}
