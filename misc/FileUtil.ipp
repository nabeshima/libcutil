/**
 * $Id$
 * Copyright (c) 2017 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cstddef>
#include <dirent.h>
#include <sys/param.h>
#include <sys/stat.h>

namespace cutil {

inline bool FileUtil::move(const std::string &orig_path,
                           const std::string &new_path) {
  return (rename(orig_path.c_str(), new_path.c_str()) == 0);
}

inline bool FileUtil::remove(const std::string &path) {
  return (remove(path.c_str()) == 0);
}

inline bool FileUtil::changeMode(const std::string &path, mode_t mode) {
  return (chmod(path.c_str(), mode) == 0);
}

inline bool FileUtil::makeDirectory(const std::string &path, mode_t mode) {
  return (mkdir(path.c_str(), mode) == 0);
}

inline bool FileUtil::isExist(const std::string &path) {
  struct stat st;
  return (stat(path.c_str(), &st) == 0);
}

inline bool FileUtil::isDirectory(const std::string &path) {
  struct stat st;
  return (stat(path.c_str(), &st) == 0 && S_ISDIR(st.st_mode) != 0);
}

inline std::vector<std::string> FileUtil::getFileList(const std::string &path) {
  std::vector<std::string> ret;

  DIR *dir = opendir(path.c_str());
  if (dir == NULL) {
    return ret;
  }

  for (struct dirent *dp = readdir(dir); dp != NULL; dp = readdir(dir)) {
    ret.push_back(dp->d_name);
  }
  closedir(dir);

  return ret;
}

inline bool FileUtil::setCurrentDirectory(const std::string &path) {
  return (chdir(path.c_str()) == 0);
}

inline std::string FileUtil::getCurrentDirectory() {
  char buf[MAXPATHLEN];
  char *ret = getcwd(buf, MAXPATHLEN);
  if (ret != NULL) {
    return std::string(ret);
  } else {
    return "";
  }
}

inline uint64_t FileUtil::getFileSize(const std::string &path) {
  struct stat st;
  if (stat(path.c_str(), &st) == 0) {
    return st.st_size;
  }
  return 0;
}

}  // namespace cutil
