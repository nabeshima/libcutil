/**
 * $Id$
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Tokenizer.h"
#include "lexical_cast.h"

namespace cutil {

template <enum ProgramOptions::OptionType OT>
inline void ProgramOptions::addOption(const std::string& command,
                                      const std::string& description) {
  optionDescriptions.insert(std::make_pair(command, Pack(OT, description)));
}

inline void ProgramOptions::parse(int argc, char* argv[]) {
  programName = std::string(argv[0]);

  for (int i = 1; i < argc; ++i) {
    Tokenizer tok;
    tok.set(argv[i], "=");
    std::string key, value;
    if (!tok.isEmpty()) {
      key = *tok;
      ++tok;
      if (!tok.isEmpty()) {
        value = *tok;
      }
    }
    if (key.size() != 0) {
      std::map<std::string, Pack>::iterator it = optionDescriptions.find(key);
      if (it != optionDescriptions.end()) {
        options.insert(std::make_pair(key, value));
      }
    }
  }
}

inline std::string ProgramOptions::getProgramName() const {
  return programName;
}

template <>
inline std::string ProgramOptions::get<std::string>(
    const std::string& key, bool* is_fail,
    typename enable_if<true>::type*) const {
  std::map<std::string, std::string>::const_iterator it = options.find(key);
  if (it != options.end()) {
    if (is_fail != NULL) {
      *is_fail = false;
    }
    return it->second;
  }
  if (is_fail != NULL) {
    *is_fail = true;
  }
  return "";
}

template <typename T>
inline T ProgramOptions::get(
    const std::string& key, bool* is_fail,
    typename enable_if<is_numeric<T>::value || is_string<T>::value ||
                       is_bool<T>::value>::type*) const {
  std::string value = get<std::string>(key, is_fail);
  if (is_fail != NULL && *is_fail == true) {
    return 0;
  }

  return lexical_cast<T>(value, is_fail);
}

template <>
inline bool ProgramOptions::get<bool>(const std::string& key, bool* is_fail,
                                      typename enable_if<true>::type*) const {
  std::string value = get<std::string>(key, is_fail);
  if (is_fail != NULL && *is_fail == true) {
    return false;
  }

  if (value == "true" || value == "True" || value == "TRUE") {
    if (is_fail != NULL) {
      *is_fail = false;
    }
    return true;
  } else if (value == "false" || value == "False" || value == "FALSE") {
    if (is_fail != NULL) {
      *is_fail = false;
    }
    return false;
  }

  if (is_fail != NULL) {
    *is_fail = true;
  }
  return false;
}

inline std::ostream& operator<<(std::ostream& stream,
                                const ProgramOptions& po) {
  std::map<std::string, ProgramOptions::Pack>::const_iterator it =
      po.optionDescriptions.begin();
  while (it != po.optionDescriptions.end()) {
    stream << it->first;
    switch (it->second.otype) {
      case ProgramOptions::NUMERIC:
        stream << ": <numeric>, ";
        break;
      case ProgramOptions::STRING:
        stream << ": <string>, ";
        break;
      case ProgramOptions::BOOL:
        stream << ": <bool>, ";
        break;
    }
    stream << it->second.description << std::endl;
    ++it;
  }
  return stream;
}
}  // namespace cutil
