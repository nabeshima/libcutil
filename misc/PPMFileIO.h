/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_PPMFILEIO_H__
#define __CUTIL_PPMFILEIO_H__

#include <string>

namespace cutil {

class PPMFileIO {
public:
  static bool write( const std::string &filename,
                     const void *image_data,
                     int width, int height );

  static bool readHeader( const std::string &filename,
                          int &width, int &height );
  
  static bool read( const std::string &filename,
                    void *image_data,
                    int &width, int &height );
};

}

#include "PPMFileIO.ipp"

#endif
