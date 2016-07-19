/**
 * $Id$
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef CUTIL_PROGRAMOPTIONS_H_
#define CUTIL_PROGRAMOPTIONS_H_

#include <cstddef>
#include <map>
#include <string>

#include "./type_traits.h"

namespace cutil {

/*
 * Parser of program options. It parses h=xxxx and help=xxxx style options
 * xxxx could be numeric, string, or bool.
 */
class ProgramOptions {
  friend std::ostream& operator<<(std::ostream& stream,
                                  const ProgramOptions& po);

 public:
  enum OptionType { STRING, NUMERIC, BOOL };

 private:
  struct Pack {
    OptionType otype;
    std::string description;
    Pack(OptionType otype, std::string description)
        : otype(otype), description(description) {}
  };

  std::string programName;
  std::map<std::string, Pack> optionDescriptions;
  std::map<std::string, std::string> options;

 public:
  ProgramOptions() {}

  template <enum OptionType>
  void addOption(const std::string& command, const std::string& description);

  void parse(int argc, char* argv[]);

  std::string getProgramName() const;

  template <typename T>
  T get(const std::string& key, bool* is_fail = NULL,
        typename enable_if<is_numeric<T>::value || is_string<T>::value ||
                           is_bool<T>::value>::type* = 0) const;
};

// show help
std::ostream& operator<<(std::ostream& stream, const ProgramOptions& po);

}  // namespace cutil

#include "ProgramOptions.ipp"

#endif  // CUTIL_PROGRAMOPTIONS_H_
