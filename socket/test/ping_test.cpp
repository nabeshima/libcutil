/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "PingClient.h"

using namespace std;
using namespace cutil;

int main() {
  
  try {
    vector< Address > res;
    //    Address::resolve( res, "127.0.0.1" );
    Address::resolve( res, "::1" );
    //    Address::resolve( "133.243.238.163", res );
    
    for ( unsigned int i = 0; i < res.size(); ++i ) {
      cout << res[ i ].isIPv4() << " " << res[ i ].isIPv6() << ", " 
           << res[ i ].toString() << "\t" << res[ i ].getPort() << endl;
    }

    cout << PingClient::ping( res[ 0 ], 1000000 ) << " localhost" << endl;
  }
  catch ( const Exception &e ) {
    cout << e.what() << " from " << e.where() << endl;
    e.printStackTrace();
  }

  try {
    vector< Address > res;
    Address::resolve( res, "yahoo.com", false );

    for ( unsigned int i = 0; i < res.size(); ++i ) {
      cout << res[ i ].isIPv4() << " " << res[ i ].isIPv6() << ", " 
           << res[ i ].toString() << "\t" << res[ i ].getPort() << endl;
    }


    cout << PingClient::ping( res[ 0 ], 1000000 ) << " yahoo.com" << endl;
  }
  catch ( const Exception &e ) {
    cout << e.what() << " from " << e.where() << endl;
  }

  try {
    vector< Address > res;
    Address::resolve( res, "gglevv.jp", false );

    for ( unsigned int i = 0; i < res.size(); ++i ) {
      cout << res[ i ].isIPv4() << " " << res[ i ].isIPv6() << ", " 
           << res[ i ].toString() << "\t" << res[ i ].getPort() << endl;
    }

    cout << PingClient::ping( res[ 0 ], 1000000 ) << " gglevv.jp" << endl;
  }
  catch ( const Exception &e ) {
    cout << e.what() << " from " << e.where() << endl;
  }

  try {
    vector< Address > res;
    Address::resolve( res, "google.jp", false );

    for ( unsigned int i = 0; i < res.size(); ++i ) {
      cout << res[ i ].isIPv4() << " " << res[ i ].isIPv6() << ", " 
           << res[ i ].toString() << "\t" << res[ i ].getPort() << endl;
    }


    cout << PingClient::ping( res[ 0 ], 1000000 ) << " google.com" << endl;
  }
  catch ( const Exception &e ) {
    cout << e.what() << " from " << e.where() << endl;
  }
  
  return 0;
}
