/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>
#include <Tokenizer.h>

using namespace std;
using namespace cutil;

int main() {
  
  Tokenizer tok;
  
  tok.set( "   swarfwa,fw2afeにんご　\"\"a,fff, :s:k,,,,  ,,,,. ", ": ," );
  
  while ( !tok.isEmpty() ) {
    cout << *tok << endl;
    ++tok;
  }
  
  tok.set( "   swarfwa,fw2afeにんご　\"\"a,fff, :s:k,,,,  ,,,,. ,,\",,,\"gg", ": ," );
  
  while ( !tok.isEmpty() ) {
    cout << *tok << endl;
    ++tok;
  }

  
  tok.set( ", ,    ", ": ," );
  
  while ( !tok.isEmpty() ) {
    cout << *tok << endl;
    ++tok;
  }


  tok.set( "", ": ," );
  
  while ( !tok.isEmpty() ) {
    cout << *tok << endl;
    ++tok;
  }
  

  return 0;
}
