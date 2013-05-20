/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>
#include <ConfigFileReader.h>

using namespace std;
using namespace cutil;

int main() {
  ConfigFileReader cfr;
  
  cfr.open( "test.cfg" );
  
  while ( !cfr.isEmpty() ) {
    cout << "SECTION <" << cfr.getSection() << ">" << endl;
    
    map< string, string > &kv = *cfr;
    
    map< string, string >::iterator
      it = kv.begin(),
      endIt = kv.end();
    while ( it != endIt ) {
      cout << "key: <" << it->first << ">, value: <" << it->second << ">" << endl;
      ++it;
    }
    
    cout << "px: " << kv[ "px" ] << endl;
    cout << "py: " << kv[ "py" ] << endl;
    cout << "cg: " << kv[ "cg" ] << endl;
    
    ++cfr;
  }
  
  return 0;
}
