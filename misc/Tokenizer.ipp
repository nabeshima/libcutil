/**
 * $Id: Tokenizer.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cutil {

inline Tokenizer::Tokenizer() : seek(0), keep_delim(false), escape(0) {}

inline Tokenizer::Tokenizer(const std::string& orig, const std::string& delim,
                            bool keep_delim, char escape) {
  set(orig, delim, keep_delim, escape);
}

inline Tokenizer::Tokenizer(const Tokenizer& opp)
    : seek(opp.seek),
      delim(opp.delim),
      orig(opp.orig),
      token(opp.token),
      keep_delim(opp.keep_delim),
      escape(opp.escape) {}

inline Tokenizer& Tokenizer::operator=(const Tokenizer& opp) {
  this->seek = opp.seek;
  this->delim = opp.delim;
  this->orig = opp.orig;
  this->token = opp.token;
  this->keep_delim = opp.keep_delim;
  this->escape = opp.escape;
  return *this;
}

inline Tokenizer& Tokenizer::operator++() {
  if (isEmpty()) {
    return *this;
  }

  if (!keep_delim) {  // skip first delimiter
    seek = orig.find_first_not_of(delim, seek);
  }

  if (seek == std::string::npos) {
    token = "";
    return *this;
  }

  std::size_t ep = seek;

  while (true) {
    ep = orig.find_first_of(delim, ep);
    if (ep == std::string::npos) {  // end if delimiter is not found.
      break;
    } else if (ep == seek) {  // delimiter is found at the first character
      ++ep;
      break;
    } else if (escape != 0 &&
               orig.at(ep - 1) ==
                   escape) {  // delimiter is found just after escape character
      ++ep;
      continue;
    } else {
      break;
    }
  }

  token = orig.substr(seek, ep - seek);
  seek = ep;
  return *this;
}

inline std::string& Tokenizer::operator*() { return token; }

inline std::string* Tokenizer::operator->() { return &token; }

inline void Tokenizer::set(const std::string& orig, const std::string& delim,
                           bool keep_delim, char escape) {
  this->seek = 0;
  this->delim = delim;
  this->orig = orig;
  this->token = "";
  this->keep_delim = keep_delim;
  this->escape = escape;
  ++(*this);
}

inline void Tokenizer::clear() {
  this->seek = 0;
  this->token = "";
  ++(*this);
}

inline bool Tokenizer::isEmpty() const {
  return (seek == std::string::npos) && (token == "");
}

inline std::size_t Tokenizer::getSeek() const { return seek; }

inline const std::string& Tokenizer::getOriginal() const { return orig; }

inline const std::string& Tokenizer::getDelimiter() const { return delim; }
}
