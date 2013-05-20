/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_HASHMAP_H_
#define _CUTIL_HASHMAP_H_

#include <cstddef>
#include <iterator>
#include <string>
#include <map>

namespace cutil {

/*!
  Simple hash function for string.
  The same algorithm as boost.
*/
std::size_t hash( const std::string& value ) throw ();


/*!
  Simple hash map for string pairs.
*/
class HashMap {
public:
  const static int BUCKET_SIZE = 1024 * 1024;
  
  
  //---------------------------- Definition of iterators BEGIN ----------------------------
  class const_iterator;
  
  //! iterator class
  class iterator
    : std::iterator< std::forward_iterator_tag, std::pair< const std::string, std::string > >  {
    friend class HashMap;
    friend class const_iterator;
    
  private:
    HashMap &hashmap;
    int bnum;
    std::map< const std::string, std::string >::iterator mapIt;
    
    iterator( HashMap &hashmap, int bnum, 
              std::map< const std::string, std::string >::iterator mapIt ) throw ();
    iterator& operator=( const HashMap::iterator &opp ) throw ();
    
  public:
    iterator( const HashMap::iterator &opp ) throw ();
    
    iterator& operator++() throw ();
    std::pair< const std::string, std::string >& operator*() throw ();
    std::pair< const std::string, std::string >* operator->() throw ();
    
    bool operator==( const HashMap::iterator &opp ) const throw ();
    bool operator==( const HashMap::const_iterator &opp ) const throw ();
    bool operator!=( const HashMap::iterator &opp ) const throw ();
    bool operator!=( const HashMap::const_iterator &opp ) const throw ();
  };
  
  //! const_iterator class
  class const_iterator
    : std::iterator< std::forward_iterator_tag, std::pair< const std::string, std::string > >  {
    friend class HashMap;
    friend class iterator;
    
  private:
    const HashMap &hashmap;
    int bnum;
    std::map< const std::string, std::string >::const_iterator mapIt;
    
    const_iterator( const HashMap &hashmap, int bnum, 
                    std::map< const std::string, std::string >::const_iterator mapIt ) throw ();
    const_iterator& operator=( const HashMap::iterator &opp ) throw ();
    
  public:
    const_iterator( const HashMap::iterator &opp ) throw ();
    
    const_iterator& operator++() throw ();
    const std::pair< const std::string, std::string >& operator*() const throw ();
    const std::pair< const std::string, std::string >* operator->() const throw ();
    
    bool operator==( const HashMap::iterator& opp ) const throw ();
    bool operator==( const HashMap::const_iterator& opp ) const throw ();
    bool operator!=( const HashMap::iterator& opp ) const throw ();
    bool operator!=( const HashMap::const_iterator& opp ) const throw ();
  };  
  //---------------------------- Definition of iterators END ----------------------------
  
private:
  std::map< const std::string, std::string > *bucket;
  
public:
  HashMap();  
  ~HashMap();
  
  void add( const std::string& key, const std::string& value ) throw ();
  std::string remove( const std::string& key ) throw ();
  void clear() throw ();
  
  HashMap::iterator begin() throw ();
  HashMap::iterator end() throw ();
  HashMap::iterator at( const std::string& key ) throw ();
  
  HashMap::const_iterator begin() const throw ();
  HashMap::const_iterator end() const throw ();
  HashMap::const_iterator at( const std::string& key ) const throw ();
  
  std::string& operator[]( const std::string& key ) throw ();
  const std::string& operator[]( const std::string& key ) const throw ();
};

}

#include "HashMap.ipp"

#endif
