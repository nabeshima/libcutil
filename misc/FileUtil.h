/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_FILEUTIL_H_
#define _CUTIL_FILEUTIL_H_

#include <string>
#include <vector>

namespace cutil {
class FileUtil {
 public:
  static bool move(const std::string &orig_path, const std::string &new_path);
  static bool remove(const std::string &path);

  static bool changeMode(const std::string &path, mode_t mode);
  static bool makeDirectory(const std::string &path, mode_t mode);

  static bool isExist(const std::string &path);
  static bool isDirectory(const std::string &path);

  static std::vector<std::string> getFileList(const std::string &path);

  static bool setCurrentDirectory(const std::string &path);
  static std::string getCurrentDirectory();

  static uint64_t getFileSize(const std::string &path);
};
}  // namespace cutil

#include "FileUtil.ipp"

#endif /* _CUTIL_FILEUTIL_H_*/
