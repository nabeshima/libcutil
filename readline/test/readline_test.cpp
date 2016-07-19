/**
 * $Id: readline_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "Cursor.h"
#include "Readline.h"
#include <string>

using namespace std;
using namespace cutil;

int main() {
  cout << "window: " << Cursor::getWidth() << " x " << Cursor::getHeight()
       << endl;
  cout << "cursor: " << Cursor::getX() << ", " << Cursor::getY() << endl;

  Cursor::setForeground(Cursor::RED);
  cout << "RED" << endl;
  Cursor::setForeground(Cursor::GREEN);
  cout << "GREEN" << endl;
  Cursor::setForeground(Cursor::YELLOW);
  cout << "YELLOW" << endl;
  Cursor::setForeground(Cursor::BLUE);
  cout << "BLUE" << endl;
  Cursor::setForeground(Cursor::PURPLE);
  cout << "PURPLE" << endl;
  Cursor::setForeground(Cursor::AQUA);
  cout << "AQUA" << endl;

  Cursor::resetProperties();
  Cursor::setReverse(true);
  cout << "WHITE" << endl;

  Cursor::resetProperties();
  Cursor::setUnderline(true);
  cout << "Underline test" << endl;

  Cursor::resetProperties();
  Cursor::setForeground(Cursor::BLUE);
  cout << "Cursor test" << endl;

  Cursor::setBlink(true);
  cout << "Blink test" << endl;
  Cursor::setBlink(false);
  Cursor::setForeground(Cursor::YELLOW);
  Cursor::setBackground(Cursor::BLACK);
  cout << "Blink test" << endl;
  Cursor::setReverse(true);
  cout << "Reverse test" << endl;
  Cursor::resetProperties();

  Readline::addCommand("command1");
  Readline::addCommand("command2");
  Readline::addCommand("cmd");
  Readline::addCommand("cmd3");
  Readline::addCommand("set");
  Readline::addCommand("get");

  Readline::setPrompt("> ");
  Readline::setHistoryFile("/tmp/.readline_history");

  int count = 0;

  while (count < 3) {
    string input;
    bool ret = Readline::get(&input, 2000000);
    cout << "readline get input " << ret << endl;
    if (ret) {
      cout << "input: " << input << endl;
    } else {
      cout << endl << "timeout exception." << endl;
      ++count;
    }
  }

  return 0;
}
