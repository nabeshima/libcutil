/**
 * $Id: proc_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Process.h"
#include <string>

using namespace std;
using namespace cutil;

static void signalhandler(int sig) {
  //  exit( sig );
  sleep(1);
  cout << "signalhandler " << sig << endl;
}

int main(int argc, char **argv) {
  if (argc == 2) {
    ::signal(SIGINT, signalhandler);

    int i = 0;
    while (true) {
      cout << i++ << endl;
      sleep(1);
      if (i == 3) {
        break;
      }
    }

    return i;
  }

  {
    Process proc;

    proc.open("./proc_test 1");
    cout << "Start " << proc.isLiving() << " " << proc.isRunning() << endl;

    while (proc.isRunning()) {
      cout << "proc " << proc.isLiving() << endl;

      string str;
      proc.stdout() >> str;
      cout << "proc " << str.size() << " " << static_cast<int>(str[0]) << " "
           << str << endl;
      sleep(1);
    }

    cout << "End " << proc.isLiving() << " " << proc.isRunning() << endl;

    proc.close();
  }

  {
    Process proc;

    proc.open("./proc_test 2");
    cout << "Start " << proc.isLiving() << " " << proc.isRunning() << endl;
    char buf[256];
    int num;

    int i = 0;
    while ((num = proc.readOut(buf, 255)) > 0) {
      buf[num] = '\0';
      cout << buf << flush;
      proc.close();
      cout << "close " << endl;
      break;

      i++;
      if (i == 3) {
        break;
      }
    }

    cout << "End " << proc.isRunning() << ", code " << proc.returnCode()
         << endl;
  }

  //   while ( true ) {
  //     cout << "dummy" << endl;
  //     sleep( 1 );
  //   }

  return 0;
}
