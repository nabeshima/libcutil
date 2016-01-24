/**
 * $Id: PPMFileIO.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_PPMFILEIO_H__
#define __CUTIL_PPMFILEIO_H__

#include <string>

namespace cutil {

class PPMFileIO {
 public:
  static bool write(const std::string &filename, const void *image_data,
                    int width, int height);

  static bool readHeader(const std::string &filename, int *width, int *height);

  static bool read(const std::string &filename, void *image_data, int *width,
                   int *height);
};
}  // namespace cutil

#include "PPMFileIO.ipp"

#endif
