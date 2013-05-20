/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cutil {

inline
std::size_t hash( const std::string& value ) throw (){
  std::string::const_iterator 
    it = value.begin(),
    endIt = value.end();
  
  std::size_t seed = 0;
  while ( it != endIt ) {
    seed ^= static_cast< std::size_t >( *it ) + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 );
    ++it;
  }
  return seed;
}


//---------------------------- Implementation of iterators BEGIN ----------------------------


inline
HashMap::iterator::iterator( HashMap &hashmap, int bnum, 
                             std::map< const std::string, std::string >::iterator mapIt ) throw () 
  : hashmap( hashmap ), bnum( bnum ), mapIt( mapIt ) {}

inline
HashMap::iterator::iterator( const HashMap::iterator &opp ) throw ()
  : hashmap( opp.hashmap ), bnum( opp.bnum ), mapIt( opp.mapIt ) {}

inline
HashMap::iterator& HashMap::iterator::operator++() throw () {
  ++mapIt;
  
  if ( hashmap.bucket[ bnum ].end() == mapIt ) {
    ++bnum;
    
    for ( ; bnum < HashMap::BUCKET_SIZE; ++bnum ) {
      mapIt = hashmap.bucket[ bnum ].begin();
      if ( mapIt != hashmap.bucket[ bnum ].end() ) {
        return *this;
      }
    }
  }

  return *this;
}

inline
std::pair< const std::string, std::string >& HashMap::iterator::operator*() throw () {
  return *mapIt;
}

inline
std::pair< const std::string, std::string >* HashMap::iterator::operator->() throw () {
  return &(*mapIt);
}

inline
bool HashMap::iterator::operator==( const HashMap::iterator& opp ) const throw () {
  return ( &hashmap == &opp.hashmap ) && ( bnum == opp.bnum ) && ( mapIt == opp.mapIt );
}

inline
bool HashMap::iterator::operator==( const HashMap::const_iterator& opp ) const throw () {
  return ( &hashmap == &opp.hashmap ) && ( bnum == opp.bnum ) && ( mapIt == opp.mapIt );
}

inline
bool HashMap::iterator::operator!=( const HashMap::iterator& opp ) const throw () {
  return !( *this == opp );
}

inline
bool HashMap::iterator::operator!=( const HashMap::const_iterator& opp ) const throw () {
  return !( *this == opp );
}


inline
HashMap::const_iterator::const_iterator( const HashMap &hashmap, int bnum, 
                                         std::map< const std::string, std::string >::const_iterator mapIt ) throw () 
  : hashmap( hashmap ), bnum( bnum ), mapIt( mapIt ) {}

inline
HashMap::const_iterator::const_iterator( const HashMap::iterator &opp ) throw ()
  : hashmap( opp.hashmap ), bnum( opp.bnum ), mapIt( opp.mapIt ) {}

inline
HashMap::const_iterator& HashMap::const_iterator::operator++() throw () {
  ++mapIt;
  
  if ( hashmap.bucket[ bnum ].end() == mapIt ) {
    ++bnum;
    
    for ( ; bnum < HashMap::BUCKET_SIZE; ++bnum ) {
      mapIt = hashmap.bucket[ bnum ].begin();
      if ( mapIt != hashmap.bucket[ bnum ].end() ) {
        return *this;
      }
    }
  }

  return *this;
}

inline
const std::pair< const std::string, std::string >& HashMap::const_iterator::operator*() const throw () {
  return *mapIt;
}

inline
const std::pair< const std::string, std::string >* HashMap::const_iterator::operator->() const throw () {
  return &(*mapIt);
}

inline
bool HashMap::const_iterator::operator==( const HashMap::iterator& opp ) const throw () {
  return ( &hashmap == &opp.hashmap ) && ( bnum == opp.bnum ) && ( mapIt == opp.mapIt );
}

inline
bool HashMap::const_iterator::operator==( const HashMap::const_iterator& opp ) const throw () {
  return ( &hashmap == &opp.hashmap ) && ( bnum == opp.bnum ) && ( mapIt == opp.mapIt );
}

inline
bool HashMap::const_iterator::operator!=( const HashMap::iterator& opp ) const throw () {
  return !( *this == opp );
}

inline
bool HashMap::const_iterator::operator!=( const HashMap::const_iterator& opp ) const throw () {
  return !( *this == opp );
}



//---------------------------- Implementation of iterators END ----------------------------



inline
HashMap::HashMap() 
  : bucket( new std::map< const std::string, std::string >[ BUCKET_SIZE ] ) {}

inline
HashMap::~HashMap() {
  delete[] bucket;
}

inline
void HashMap::add( const std::string& key, const std::string& value ) throw () {
  std::size_t hval = hash( key );
  bucket[ hval % BUCKET_SIZE ].insert( std::pair< const std::string, std::string >( key, value ) );
}

inline
std::string HashMap::remove( const std::string& key ) throw () {
  std::map< const std::string, std::string > &btmp = bucket[ hash( key ) % BUCKET_SIZE ];
  std::map< const std::string, std::string >::iterator it = btmp.find( key );
  std::string ret = it->second;
  btmp.erase( it );
  return ret;
}

inline
void HashMap::clear() throw () {
  for ( int i = 0; i < BUCKET_SIZE; ++i ) {
    bucket[ i ].clear();
  }
}

inline
HashMap::iterator HashMap::begin() throw () {
  for ( int i = 0; i < BUCKET_SIZE; ++i ) {
    if ( !bucket[ i ].empty() ) {
      return HashMap::iterator( *this, i, bucket[ i ].begin() );
    }
  }
  return HashMap::iterator( *this, BUCKET_SIZE - 1, bucket[ BUCKET_SIZE - 1 ].end() );
}

inline
HashMap::const_iterator HashMap::begin() const throw () {
  for ( int i = 0; i < BUCKET_SIZE; ++i ) {
    if ( !bucket[ i ].empty() ) {
      return HashMap::const_iterator( *this, i, bucket[ i ].begin() );
    }
  }
  return HashMap::const_iterator( *this, BUCKET_SIZE - 1, bucket[ BUCKET_SIZE - 1 ].end() );
}

inline
HashMap::iterator HashMap::end() throw () {
  return HashMap::iterator( *this, BUCKET_SIZE, bucket[ BUCKET_SIZE - 1 ].end() );
}

inline
HashMap::const_iterator HashMap::end() const throw () {
  return HashMap::const_iterator( *this, BUCKET_SIZE, bucket[ BUCKET_SIZE - 1 ].end() );
}

inline
HashMap::iterator HashMap::at( const std::string& key ) throw () {
  int bnum = hash( key ) % BUCKET_SIZE;
  std::map< const std::string, std::string > &btmp = bucket[ bnum ];
  std::map< const std::string, std::string >::iterator it = btmp.find( key );
  if ( it == btmp.end() ) {
    return HashMap::iterator( *this, BUCKET_SIZE, bucket[ BUCKET_SIZE - 1 ].end() );
  }
  return HashMap::iterator( *this, bnum, it );
}

inline
HashMap::const_iterator HashMap::at( const std::string& key ) const throw () {
  int bnum = hash( key ) % BUCKET_SIZE;
  std::map< const std::string, std::string > &btmp = bucket[ bnum ];
  std::map< const std::string, std::string >::const_iterator it = btmp.find( key );
  if ( it == btmp.end() ) {
    return HashMap::const_iterator( *this, BUCKET_SIZE, bucket[ BUCKET_SIZE - 1 ].end() );
  }
  return HashMap::const_iterator( *this, bnum, it );
}


inline
std::string& HashMap::operator[]( const std::string& key ) throw () {
  std::map< const std::string, std::string > &btmp = bucket[ hash( key ) % BUCKET_SIZE ];
  std::map< const std::string, std::string >::iterator it = btmp.find( key );
  if ( it == btmp.end() ) {
    btmp.insert( std::pair< const std::string, std::string >( key, "" ) );
    it = btmp.find( key );
  }
  return it->second;
}

inline
const std::string& HashMap::operator[]( const std::string& key ) const throw () {
  std::map< const std::string, std::string > &btmp = bucket[ hash( key ) % BUCKET_SIZE ];
  std::map< const std::string, std::string >::iterator it = btmp.find( key );
  if ( it == btmp.end() ) {
    btmp.insert( std::pair< const std::string, std::string >( key, "" ) );
    it = btmp.find( key );
  }
  return it->second;
}

}
