/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <fstream>
#include <sstream>

#include <iostream>

namespace cutil {

inline
bool PPMFileIO::write( const std::string &filename,
                       const void *image_data,
                       int width, int height ) {
  
  // image size
  const char *uimage_data = (const char*)image_data;
  int image_size = width * height * 3;
  
  // create header
  std::stringstream header_sstrm;
  header_sstrm << "P6\n"
               << width << " " << height << '\n'
               << "255\n";
  
  std::ofstream fout;
  fout.open( filename.c_str(), std::ios::binary );
  
  if ( !fout ) {
    return false;
  }
  
  std::string header = header_sstrm.str();
  
  fout.write( header.c_str(), header.size() );
  fout.write( uimage_data, image_size );
  
  fout.close();

  return true;
}

inline
bool PPMFileIO::readHeader( const std::string &filename,
                            int &width, int &height ) {
  
  // open file
  std::ifstream fin;
  fin.open( filename.c_str(), std::ios::binary );
  
  if ( !fin ) {
    return false;
  }

    
  // read header
  std::string p6;
  int w = 0;
  int h = 0;
  int max_val = 0;
  
  fin >> p6;

  while ( fin ) {
    std::string line;
    std::getline( fin, line );
    
    if ( line == "" || line[ 0 ] == '#' ) {
      continue;
    }
    else {
      std::stringstream sstrm( line );
      if ( w == 0 ) {
        sstrm >> w >> h;
      }
      else if ( max_val == 0 ) {
        sstrm >> max_val;
        break;
      } 
    }
  }
  
  if ( !fin ||
       p6 != "P6" ||
       w <= 0 ||
       h <= 0 ||
       max_val != 255 ) {
    return false;
  }
  
  fin.close();
  width = w;
  height = h;

  return true;
}

inline
bool PPMFileIO::read( const std::string &filename,
                      void *image_data,
                      int &width, int &height ) {
  
  std::ifstream fin;
  fin.open( filename.c_str(), std::ios::binary );
  
  if ( !fin ) {
    return false;
  }

  // read header
  std::string p6;
  int w = 0;
  int h = 0;
  int max_val = 0;
  
  fin >> p6;
  
  while ( fin ) {
    std::string line;
    std::getline( fin, line );
    
    if ( line == "" || line[ 0 ] == '#' ) {
      continue;
    }
    else {
      std::stringstream sstrm( line );
      if ( w == 0 ) {
        sstrm >> w >> h;
      }
      else if ( max_val == 0 ) {
        sstrm >> max_val;
        break;
      } 
    }
  }
  
  if ( !fin ||
       p6 != "P6" ||
       w <= 0 ||
       h <= 0 ||
       max_val != 255 ) {
    return false;
  }

  char c = fin.peek();
  if ( c == '\r' ) {
    fin.get( c );
    c = fin.peek();
  }
  
  if ( c == '\n' ) {
    fin.get( c );
  }

  char *uimage_data = (char*)image_data;
  int image_size = w * h * 3;
  fin.read( uimage_data, image_size );

  fin.close();
  width = w;
  height = h;

  return true;
}

}
