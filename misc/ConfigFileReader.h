/**
 * $Id: ConfigFileReader.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_CONFIGFILEREADER_H_
#define _CUTIL_CONFIGFILEREADER_H_

#include <fstream>
#include <string>
#include <map>
#include <deque>
#include <utility>

namespace cutil {

class ConfigFileReader : std::iterator<std::forward_iterator_tag,
                                       std::map<std::string, std::string> > {
 private:
  bool eof_flag;
  std::string section;
  std::map<std::string, std::string> config;
  std::deque<std::pair<std::string, std::ifstream::pos_type> > fstack;

  ConfigFileReader(const ConfigFileReader& opp);
  ConfigFileReader& operator=(const ConfigFileReader& opp);

  bool operator==(const ConfigFileReader& opp) const;
  bool operator!=(const ConfigFileReader& opp) const;

 public:
  ConfigFileReader();
  explicit ConfigFileReader(const std::string& filename);

  // iterate to load next section
  ConfigFileReader& operator++();
  std::map<std::string, std::string>& operator*();
  std::map<std::string, std::string>* operator->();

  // set top filename to parse
  void open(const std::string& filename);

  // return current section
  const std::string& getSection() const;

  // true if parsing has finished
  bool isEmpty() const;
};
}  // namespace cutil

#include "ConfigFileReader.ipp"

#endif
