/**
 * $Id: Tokenizer.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cutil {

inline Tokenizer::Tokenizer() : seek(0) {}

inline Tokenizer::Tokenizer(const std::string& orig, const std::string& delim) {
  set(orig, delim);
}

inline Tokenizer::Tokenizer(const Tokenizer& opp)
    : seek(opp.seek), delim(opp.delim), orig(opp.orig), token(opp.token) {}

inline Tokenizer& Tokenizer::operator=(const Tokenizer& opp) {
  this->seek = opp.seek;
  this->delim = opp.delim;
  this->orig = opp.orig;
  this->token = opp.token;
  return *this;
}

inline Tokenizer& Tokenizer::operator++() {
  if (!isEmpty()) {
    seek = orig.find_first_not_of(delim, seek);

    if (seek != std::string::npos) {
      std::size_t ep = orig.find_first_of(delim, seek);
      token = orig.substr(seek, ep - seek);
      seek = ep;
    } else {
      token = "";
    }
  }

  return *this;
}

inline std::string& Tokenizer::operator*() { return token; }

inline std::string* Tokenizer::operator->() { return &token; }

inline void Tokenizer::set(const std::string& orig, const std::string& delim) {
  this->seek = 0;
  this->delim = delim;
  this->orig = orig;
  this->token = "";
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
