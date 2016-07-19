/**
 * $Id: HashMap.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cutil {

inline std::size_t hash(const std::string& value) {
  std::string::const_iterator it = value.begin(), endIt = value.end();

  std::size_t seed = 0;
  while (it != endIt) {
    seed ^=
        static_cast<std::size_t>(*it) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    ++it;
  }
  return seed;
}

//---------------------------- Implementation of iterators BEGIN
//----------------------------

inline HashMap::iterator::iterator(
    HashMap* hashmap, int bnum,
    std::map<const std::string, std::string>::iterator mapIt)
    : hashmap(*hashmap), bnum(bnum), mapIt(mapIt) {}

inline HashMap::iterator::iterator(const HashMap::iterator& opp)
    : hashmap(opp.hashmap), bnum(opp.bnum), mapIt(opp.mapIt) {}

inline HashMap::iterator& HashMap::iterator::operator++() {
  ++mapIt;

  if (hashmap.bucket[bnum].end() == mapIt) {
    ++bnum;

    for (; bnum < HashMap::BUCKET_SIZE; ++bnum) {
      mapIt = hashmap.bucket[bnum].begin();
      if (mapIt != hashmap.bucket[bnum].end()) {
        return *this;
      }
    }
  }

  return *this;
}

inline std::pair<const std::string, std::string>& HashMap::iterator::
operator*() {
  return *mapIt;
}

inline std::pair<const std::string, std::string>* HashMap::iterator::
operator->() {
  return &(*mapIt);
}

inline bool HashMap::iterator::operator==(const HashMap::iterator& opp) const {
  return (&hashmap == &opp.hashmap) && (bnum == opp.bnum) &&
         (mapIt == opp.mapIt);
}

inline bool HashMap::iterator::operator==(
    const HashMap::const_iterator& opp) const {
  return (&hashmap == &opp.hashmap) && (bnum == opp.bnum) &&
         (mapIt == opp.mapIt);
}

inline bool HashMap::iterator::operator!=(const HashMap::iterator& opp) const {
  return !(*this == opp);
}

inline bool HashMap::iterator::operator!=(
    const HashMap::const_iterator& opp) const {
  return !(*this == opp);
}

inline HashMap::const_iterator::const_iterator(
    const HashMap& hashmap, int bnum,
    std::map<const std::string, std::string>::const_iterator mapIt)
    : hashmap(hashmap), bnum(bnum), mapIt(mapIt) {}

inline HashMap::const_iterator::const_iterator(const HashMap::iterator& opp)
    : hashmap(opp.hashmap), bnum(opp.bnum), mapIt(opp.mapIt) {}

inline HashMap::const_iterator& HashMap::const_iterator::operator++() {
  ++mapIt;

  if (hashmap.bucket[bnum].end() == mapIt) {
    ++bnum;

    for (; bnum < HashMap::BUCKET_SIZE; ++bnum) {
      mapIt = hashmap.bucket[bnum].begin();
      if (mapIt != hashmap.bucket[bnum].end()) {
        return *this;
      }
    }
  }

  return *this;
}

inline const std::pair<const std::string, std::string>&
    HashMap::const_iterator::operator*() const {
  return *mapIt;
}

inline const std::pair<const std::string, std::string>*
    HashMap::const_iterator::operator->() const {
  return &(*mapIt);
}

inline bool HashMap::const_iterator::operator==(
    const HashMap::iterator& opp) const {
  return (&hashmap == &opp.hashmap) && (bnum == opp.bnum) &&
         (mapIt == opp.mapIt);
}

inline bool HashMap::const_iterator::operator==(
    const HashMap::const_iterator& opp) const {
  return (&hashmap == &opp.hashmap) && (bnum == opp.bnum) &&
         (mapIt == opp.mapIt);
}

inline bool HashMap::const_iterator::operator!=(
    const HashMap::iterator& opp) const {
  return !(*this == opp);
}

inline bool HashMap::const_iterator::operator!=(
    const HashMap::const_iterator& opp) const {
  return !(*this == opp);
}

//---------------------------- Implementation of iterators END
//----------------------------

inline HashMap::HashMap()
    : bucket(new std::map<const std::string, std::string>[ BUCKET_SIZE ]) {}

inline HashMap::~HashMap() { delete[] bucket; }

inline void HashMap::add(const std::string& key, const std::string& value) {
  std::size_t hval = hash(key);
  bucket[hval % BUCKET_SIZE].insert(
      std::pair<const std::string, std::string>(key, value));
}

inline std::string HashMap::remove(const std::string& key) {
  std::map<const std::string, std::string>& btmp =
      bucket[hash(key) % BUCKET_SIZE];
  std::map<const std::string, std::string>::iterator it(btmp.find(key));
  std::string ret = it->second;
  btmp.erase(it);
  return ret;
}

inline void HashMap::clear() {
  for (int i = 0; i < BUCKET_SIZE; ++i) {
    bucket[i].clear();
  }
}

inline HashMap::iterator HashMap::begin() {
  for (int i = 0; i < BUCKET_SIZE; ++i) {
    if (!bucket[i].empty()) {
      return HashMap::iterator(this, i, bucket[i].begin());
    }
  }
  return HashMap::iterator(this, BUCKET_SIZE - 1,
                           bucket[BUCKET_SIZE - 1].end());
}

inline HashMap::const_iterator HashMap::begin() const {
  for (int i = 0; i < BUCKET_SIZE; ++i) {
    if (!bucket[i].empty()) {
      return HashMap::const_iterator(*this, i, bucket[i].begin());
    }
  }
  return HashMap::const_iterator(*this, BUCKET_SIZE - 1,
                                 bucket[BUCKET_SIZE - 1].end());
}

inline HashMap::iterator HashMap::end() {
  return HashMap::iterator(this, BUCKET_SIZE, bucket[BUCKET_SIZE - 1].end());
}

inline HashMap::const_iterator HashMap::end() const {
  return HashMap::const_iterator(*this, BUCKET_SIZE,
                                 bucket[BUCKET_SIZE - 1].end());
}

inline HashMap::iterator HashMap::at(const std::string& key) {
  int bnum = hash(key) % BUCKET_SIZE;
  std::map<const std::string, std::string>& btmp = bucket[bnum];
  std::map<const std::string, std::string>::iterator it(btmp.find(key));
  if (it == btmp.end()) {
    return HashMap::iterator(this, BUCKET_SIZE, bucket[BUCKET_SIZE - 1].end());
  }
  return HashMap::iterator(this, bnum, it);
}

inline HashMap::const_iterator HashMap::at(const std::string& key) const {
  int bnum = hash(key) % BUCKET_SIZE;
  std::map<const std::string, std::string>& btmp = bucket[bnum];
  std::map<const std::string, std::string>::const_iterator it(btmp.find(key));
  if (it == btmp.end()) {
    return HashMap::const_iterator(*this, BUCKET_SIZE,
                                   bucket[BUCKET_SIZE - 1].end());
  }
  return HashMap::const_iterator(*this, bnum, it);
}

inline std::string& HashMap::operator[](const std::string& key) {
  std::map<const std::string, std::string>& btmp =
      bucket[hash(key) % BUCKET_SIZE];
  std::map<const std::string, std::string>::iterator it(btmp.find(key));
  if (it == btmp.end()) {
    btmp.insert(std::pair<const std::string, std::string>(key, ""));
    it = btmp.find(key);
  }
  return it->second;
}

inline const std::string& HashMap::operator[](const std::string& key) const {
  std::map<const std::string, std::string>& btmp =
      bucket[hash(key) % BUCKET_SIZE];
  std::map<const std::string, std::string>::iterator it(btmp.find(key));
  if (it == btmp.end()) {
    btmp.insert(std::pair<const std::string, std::string>(key, ""));
    it = btmp.find(key);
  }
  return it->second;
}
}
