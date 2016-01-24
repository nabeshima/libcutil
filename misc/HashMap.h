/**
 * $Id: HashMap.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_HASHMAP_H_
#define _CUTIL_HASHMAP_H_

#include <cstddef>
#include <iterator>
#include <string>
#include <map>
#include <utility>

namespace cutil {

/*!
  Simple hash function for string.
  The same algorithm as boost.
*/
std::size_t hash(const std::string& value);

/*!
  Simple hash map for string pairs.
*/
class HashMap {
 public:
  static const int BUCKET_SIZE = 1024 * 1024;

  //---------------------------- Definition of iterators BEGIN
  //----------------------------
  class const_iterator;

  //! iterator class
  class iterator : std::iterator<std::forward_iterator_tag,
                                 std::pair<const std::string, std::string> > {
    friend class HashMap;
    friend class const_iterator;

   private:
    HashMap& hashmap;
    int bnum;
    std::map<const std::string, std::string>::iterator mapIt;

    iterator(HashMap* hashmap, int bnum,
             std::map<const std::string, std::string>::iterator mapIt);
    iterator& operator=(const HashMap::iterator& opp);

   public:
    iterator(const HashMap::iterator& opp);  // NOLINT

    iterator& operator++();
    std::pair<const std::string, std::string>& operator*();
    std::pair<const std::string, std::string>* operator->();

    bool operator==(const HashMap::iterator& opp) const;
    bool operator==(const HashMap::const_iterator& opp) const;
    bool operator!=(const HashMap::iterator& opp) const;
    bool operator!=(const HashMap::const_iterator& opp) const;
  };

  //! const_iterator class
  class const_iterator
      : std::iterator<std::forward_iterator_tag,
                      std::pair<const std::string, std::string> > {
    friend class HashMap;
    friend struct iterator;

   private:
    const HashMap& hashmap;
    int bnum;
    std::map<const std::string, std::string>::const_iterator mapIt;

    const_iterator(
        const HashMap& hashmap, int bnum,
        std::map<const std::string, std::string>::const_iterator mapIt);
    const_iterator& operator=(const HashMap::iterator& opp);

   public:
    const_iterator(const HashMap::iterator& opp);  // NOLINT

    const_iterator& operator++();
    const std::pair<const std::string, std::string>& operator*() const;
    const std::pair<const std::string, std::string>* operator->() const;

    bool operator==(const HashMap::iterator& opp) const;
    bool operator==(const HashMap::const_iterator& opp) const;
    bool operator!=(const HashMap::iterator& opp) const;
    bool operator!=(const HashMap::const_iterator& opp) const;
  };
  //---------------------------- Definition of iterators END
  //----------------------------

 private:
  std::map<const std::string, std::string>* bucket;

 public:
  HashMap();
  ~HashMap();

  void add(const std::string& key, const std::string& value);
  std::string remove(const std::string& key);
  void clear();

  HashMap::iterator begin();
  HashMap::iterator end();
  HashMap::iterator at(const std::string& key);

  HashMap::const_iterator begin() const;
  HashMap::const_iterator end() const;
  HashMap::const_iterator at(const std::string& key) const;

  std::string& operator[](const std::string& key);
  const std::string& operator[](const std::string& key) const;
};
}  // namespace cutil

#include "HashMap.ipp"

#endif
