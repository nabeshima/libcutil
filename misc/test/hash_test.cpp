/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>

#include <HashMap.h>

using namespace std;
using namespace cutil;

class Test {
private:
  const HashMap &hmap;
public:
  Test( const HashMap &hmap )
    : hmap( hmap ) {}

  void func() {
    //     hmap.add( "key1", "test 1   g" );
    //     hmap.add( "key13", "test 13" );
    //     hmap.add( "key1000001222", "test 1000001222 2" );  
    
    {
      HashMap::const_iterator
        it = hmap.begin(),
        endIt = hmap.end();
      
      while ( it != endIt ) {
        cout << (*it).first << ", " << it->second << endl;
        ++it;
      }
    }   

    cout << hmap[ "key1" ] << endl;
    cout << hmap[ "key13" ] << endl;
    cout << hmap[ "key1000001222" ] << endl;

    cout << hmap[ "key1000022" ] << endl;

    //    hmap[ "key1000022" ] = "1.2rrr";
    cout << hmap[ "key1000022" ] << endl;
    //    hmap[ "key1000022" ] = "changed";
    cout << hmap[ "key1000022" ] << endl;

    HashMap::const_iterator
      cit = hmap.at( "key1000022" );
  
    if ( cit != hmap.end() ) {
      //      cit->second = "iterator test";
      cout << hmap[ "key1000022" ] << endl;
    }

  }
};

int main() {
  cout << "Is this good? " << hash( "Is this good?" ) << endl;
  cout << "is this good? " << hash( "is this good?" ) << endl;
  cout << "is  this good? " << hash( "is  this good?" ) << endl;
  
  
  HashMap hashmap;
  
  hashmap.add( "key1", "test 1   g" );
  hashmap.add( "key13", "test 13" );
  hashmap.add( "key1000001222", "test 1000001222 2" );  
  
  {
    HashMap::const_iterator
      it = hashmap.begin(),
      endIt = hashmap.end();

    while ( it != endIt ) {
      cout << (*it).first << ", " << it->second << endl;
      ++it;
    }
  }   

  cout << hashmap[ "key1" ] << endl;
  cout << hashmap[ "key13" ] << endl;
  cout << hashmap[ "key1000001222" ] << endl;

  cout << hashmap[ "key1000022" ] << endl;

  hashmap[ "key1000022" ] = "1.2rrr";
  cout << hashmap[ "key1000022" ] << endl;
  hashmap[ "key1000022" ] = "changed";
  cout << hashmap[ "key1000022" ] << endl;

  HashMap::iterator
    cit = hashmap.at( "key1000022" );
  
  if ( cit != hashmap.end() ) {
    cit->second = "iterator test";
    cout << hashmap[ "key1000022" ] << endl;
  }
  
  Test test( hashmap );
  test.func();

  cout << "Done." << endl;

  {
    HashMap::const_iterator
      it = hashmap.begin(),
      endIt = hashmap.end();

    while ( it != endIt ) {
      cout << (*it).first << ", " << it->second << endl;
      ++it;
    }
  }

  return 0;
}
