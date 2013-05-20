/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cutil {

inline
Tokenizer::Tokenizer() throw () 
  : seek( 0 ) {}

inline
Tokenizer::Tokenizer( const std::string &orig, const std::string &delim ) throw () {
  set( orig, delim );
}

inline  
Tokenizer::Tokenizer( const Tokenizer &opp ) throw () 
  : seek( opp.seek ), delim( opp.delim ), orig( opp.orig ), token( opp.token ) {}

inline
Tokenizer& Tokenizer::operator=( const Tokenizer &opp ) throw () {
  this->seek = opp.seek;
  this->delim = opp.delim;
  this->orig = opp.orig;
  this->token = opp.token;
  return *this;
}

inline
Tokenizer& Tokenizer::operator++() throw () {
  if ( !isEmpty() ) {
    seek = orig.find_first_not_of( delim, seek );

    if ( seek != std::string::npos ) {
      std::size_t ep = orig.find_first_of( delim, seek );
      token = orig.substr( seek, ep - seek );
      seek = ep;    
    }
    else {
      token = "";      
    }
  }
  
  return *this;
}

inline
std::string& Tokenizer::operator*() throw () {
  return token;
}

inline
std::string* Tokenizer::operator->() throw () {
  return &token;
}

inline
void Tokenizer::set( const std::string &orig, const std::string &delim ) throw () {
  this->seek = 0;
  this->delim = delim;
  this->orig = orig;
  this->token = "";
  ++(*this);
}

inline
void Tokenizer::clear() throw () {
  this->seek = 0;
  this->token = "";
  ++(*this);
}

inline
bool Tokenizer::isEmpty() const throw () {
  return ( seek == std::string::npos ) && ( token == "" );
}

inline
std::size_t Tokenizer::getSeek() const throw () {
  return seek;
}

inline
const std::string& Tokenizer::getOriginal() const throw () {
  return orig;
}

inline
const std::string& Tokenizer::getDelimiter() const throw () {
  return delim;
}

}
