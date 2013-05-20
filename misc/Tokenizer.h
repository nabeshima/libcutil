/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_TOKENIZER_H_
#define _CUTIL_TOKENIZER_H_

#include <vector>
#include <string>

namespace cutil {

class Tokenizer
  : std::iterator< std::forward_iterator_tag, std::string > {
  
private:
  std::size_t seek;
  std::string delim;
  std::string orig;
  std::string token;
  
  bool operator==( const Tokenizer& opp ) const throw ();
  bool operator!=( const Tokenizer& opp ) const throw ();
  
public:
  Tokenizer() throw ();
  Tokenizer( const std::string &orig, const std::string &delim = " ," ) throw ();
  
  Tokenizer( const Tokenizer &opp ) throw ();  
  Tokenizer& operator=( const Tokenizer &opp ) throw ();
  
  // iterate to parse next token
  Tokenizer& operator++() throw ();
  std::string& operator*() throw ();
  std::string* operator->() throw ();
  
  // set string to tokenize
  void set( const std::string &orig, const std::string &delim = " ," ) throw ();
  
  // reset to tokenize string from head
  void clear() throw ();
  
  // true if tokenization has finished 
  bool isEmpty() const throw ();
  
  std::size_t getSeek() const throw ();
  const std::string& getOriginal() const throw ();
  const std::string& getDelimiter() const throw ();
};

}

#include "Tokenizer.ipp"

#endif
