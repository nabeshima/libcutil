/**
 * $Id: Tokenizer.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_TOKENIZER_H_
#define _CUTIL_TOKENIZER_H_

#include <string>
#include <vector>

namespace cutil {

class Tokenizer : std::iterator<std::forward_iterator_tag, std::string> {
 private:
  std::size_t seek;
  std::string delim;
  std::string orig;
  std::string token;
  bool keep_delim;
  char escape;

  bool operator==(const Tokenizer& opp) const;
  bool operator!=(const Tokenizer& opp) const;

 public:
  Tokenizer();
  explicit Tokenizer(const std::string& orig, const std::string& delim,
                     bool keep_delim = false, char escape = 0);

  Tokenizer(const Tokenizer& opp);
  Tokenizer& operator=(const Tokenizer& opp);

  // iterate to parse next token
  Tokenizer& operator++();
  std::string& operator*();
  std::string* operator->();

  // set string to tokenize
  void set(const std::string& orig, const std::string& delim = " ,",
           bool keep_delim = false, char escape = 0);

  // reset to tokenize string from head
  void clear();

  // true if tokenization has finished
  bool isEmpty() const;

  std::size_t getSeek() const;
  const std::string& getOriginal() const;
  const std::string& getDelimiter() const;
};
}  // namespace cutil

#include "Tokenizer.ipp"

#endif
