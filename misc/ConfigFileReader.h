/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_CONFIGFILEREADER_H_
#define _CUTIL_CONFIGFILEREADER_H_

#include <fstream>
#include <string>
#include <map>
#include <deque>

namespace cutil {

class ConfigFileReader
  : std::iterator< std::forward_iterator_tag, std::map< std::string, std::string > > {
  
private:
  bool eof_flag;
  std::string section;
  std::map< std::string, std::string > config;
  std::deque< std::pair< std::string, std::ifstream::pos_type > > fstack;
  
  ConfigFileReader( const ConfigFileReader &opp ) throw ();  
  ConfigFileReader& operator=( const ConfigFileReader &opp ) throw ();
  
  bool operator==( const ConfigFileReader& opp ) const throw ();
  bool operator!=( const ConfigFileReader& opp ) const throw ();
  
public:
  ConfigFileReader() throw ();
  ConfigFileReader( const std::string &filename ) throw ();
  
  // iterate to load next section
  ConfigFileReader& operator++() throw ();
  std::map< std::string, std::string >& operator*() throw ();
  std::map< std::string, std::string >* operator->() throw ();
  
  // set top filename to parse
  void open( const std::string &filename ) throw ();
  
  // return current section
  const std::string& getSection() const throw ();
  
  // true if parsing has finished 
  bool isEmpty() const throw ();
};

}

#include "ConfigFileReader.ipp"

#endif
