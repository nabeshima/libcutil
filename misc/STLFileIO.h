/**
 * $Id: STLFileIO.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_STLFILEIO_H__
#define __CUTIL_STLFILEIO_H__

#include <string>
#include <vector>

#include "cutil/Kinematics.h"

namespace cutil {

class STLFileIO {
 public:
  static bool write(const std::string &filename,
                    const std::vector<cotave::ColumnVector3> &vertices,
                    const std::vector<cotave::IntVector3> &indices);

  static bool read(const std::string &filenamce,
                   std::vector<cotave::ColumnVector3> *vertices,
                   std::vector<cotave::IntVector3> *indices,
                   bool inversion = false);
};
}  // namespace cutil

#include "STLFileIO.ipp"

#endif
