/**
 * $Id: token_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <Tokenizer.h>
#include <iostream>

using namespace std;
using namespace cutil;

int main() {
  {
    Tokenizer tok;

    tok.set("   swarfwa,fw2afeにんご　\"\"a,fff, :s:k,,,,  ,,,,. ", ": ,");

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }

    tok.set("   swarfwa,fw2afeにんご　\"\"a,fff, :s:k,,,,  ,,,,. ,,\",,,\"gg",
            ": ,");

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }

    tok.set(", ,    ", ": ,");

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }

    tok.set("", ": ,", true);

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }

    tok.set(",,a.,,", ": ,", true);

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }

    tok.set(" ,b.,,", ": ,", false);

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }
  }

  {
    Tokenizer tok;

    tok.set("   swarfwa,fw2afeにんご　\"\"a,fff, :s:k,,,,  ,,,,. ", ": ,", true,
            '\\');

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }

    tok.set("   swarfwa,fw2afeにんご　\"\"a,fff, :s:k,,,,  ,,,,. ,,\",,,\"gg",
            ": ,", false, '\\');

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }

    tok.set(", ,    ", ": ,", true);

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }

    cout << "skip test skip delim" << endl;

    tok.set("\\\\,\\,pgggg:, ", ": ,", false);

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }

    cout << "skip test keep delim" << endl;

    tok.set("\\\\,\\,pgggg:, ", ": ,", true);

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }
    cout << "escape test skip delim" << endl;

    tok.set("\\\\,\\,pgggg:, ", ": ,", false, '\\');

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }

    cout << "escape test keep delim" << endl;

    tok.set("\\\\,\\,pgggg:, ", ": ,", true, '\\');

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }

    tok.set("", ": ,", true);

    while (!tok.isEmpty()) {
      cout << *tok << endl;
      ++tok;
    }
  }

  return 0;
}
