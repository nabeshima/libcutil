/**
 * $Id$
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "ProgramOptions.h"

#include <iostream>
#include <string>

using namespace std;
using namespace cutil;

class MyClass {};

int main(int argc, char *argv[]) {
  ProgramOptions po;
  cout << "1" << endl;

  po.addOption<ProgramOptions::STRING>("--help", "Show help");
  po.addOption<ProgramOptions::STRING>("--log", "parameters ");

  cout << "2" << endl;
  po.addOption<ProgramOptions::NUMERIC>("--port", "Set port number");

  cout << "3" << endl;
  po.addOption<ProgramOptions::BOOL>("--debug", "Show error messages");

  po.parse(argc, argv);

  cout << po.getProgramName() << endl;
  cout << "100" << endl;

  bool err;
  string help = po.get<string>("--help", &err);
  if (!err) {
    cout << "HELP" << endl << po;
  }

  cout << "--port, " << po.get<int>("--port") << endl;
  cout << "--debug, " << po.get<bool>("--debug") << endl;
  cout << "--log, " << po.get<string>("--log") << endl;

  // MyClass a = po.get<MyClass>("--debug");

  return 0;
}
