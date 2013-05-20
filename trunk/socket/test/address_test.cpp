/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Address.h"

#include <vector>
#include <list>

using namespace std;
using namespace cutil;

class Dummy {
private:
  Address address;
public:
  void set() throw ( Exception ) {

    list< Address > res;
    try {
      Address::resolve( res, "yahoo.com", false );
      address = res.front();
      address.setPort( 23333 );
    }
    catch ( const Exception &e ) {
      cout << e.what() << " from " << e.where() << endl;
      e.printStackTrace();
    }
  }
};

int main( int argc, char **argv ) {
  {
    try {
      Dummy dummy;
      dummy.set();
      
      Address address;
      
      {
        vector< Address > res;
        
        Address::resolve( res, "::1" );
        for ( unsigned int i = 0; i < res.size(); ++i ) {
          cout << res[ i ].isIPv4() << " " << res[ i ].isIPv6() << ", " 
               << res[ i ].toString() << "\t" << res[ i ].getPort() << endl;
        }
        
        if ( res.size() > 0 ) {
          address = res.back();
          cout << address.getPort() << endl;
          address.setPort( 5000 );
          cout << address.getPort() << endl;
        }
        
      }

      {
        vector< Address > res;
        
        // Address::resolve( res, "yahoo.com", false );
        Address::resolve( res, "gglevv.com", false );
        for ( unsigned int i = 0; i < res.size(); ++i ) {
          cout << res[ i ].isIPv4() << " " << res[ i ].isIPv6() << ", " 
               << res[ i ].toString() << "\t" << res[ i ].getPort() << endl;
        }
      }

    }
    catch ( const Exception &e ) {
      cout << e.what() << " from " << e.where() << endl;
      e.printStackTrace();
    }
  }
  
  return 0;
}
