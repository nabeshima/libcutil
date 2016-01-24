/**
 * $Id: Cursor.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_CURSOR_H__
#define __CUTIL_CURSOR_H__

namespace cutil {

class Cursor {
 private:
  int x, y;
  int w, h;
  int foreground, background;
  bool strong, bar, underline, blink, reverse;

  bool _getPosition();
  bool _getWinSize();

  void _setProperties();
  void _resetProperties();

  void _clear();  // buffer clear

  void _show();
  void _hide();
  void _move(int x, int y);

  Cursor();
  ~Cursor();

  static Cursor& instance();

 public:
  enum { BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, AQUA, WHITE };

  static void setForeground(int color);
  static void setBackground(int color);

  static void setStrong(bool onoff);
  static void setBar(bool onoff);
  static void setUnderline(bool onoff);
  static void setBlink(bool onoff);
  static void setReverse(bool onoff);

  static void resetProperties();

  static void move(int x, int y);
  static int getX();
  static int getY();

  static int getWidth();
  static int getHeight();
};
}  // namespace cutil

#include "Cursor.ipp"

#endif
