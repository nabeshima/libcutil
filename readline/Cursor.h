/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_CURSOR_H__
#define __CUTIL_CURSOR_H__

#include "CursorException.h"


namespace cutil {

class Cursor {
private:
  int x, y;
  int w, h;
  int foreground, background;
  bool strong, bar, underline, blink, reverse;
  
  void _getPosition() 
    throw ( CursorException );
  void _getWinSize() 
    throw ( CursorException );  

  void _setProperties() throw ();  
  void _resetProperties() throw ();
  
  void _clear() throw ();  // buffer clear
  
  void _show() throw ();
  void _hide() throw ();
  void _move( int x, int y ) throw ();

  Cursor() throw ();
  ~Cursor() throw ();

  
  static Cursor& instance() throw ();

public:
  enum { BLACK, RED, GREEN, YELLOW, BLUE, PURPLE, AQUA, WHITE };

  static void setForeground( int color ) throw ();
  static void setBackground( int color ) throw ();

  static void setStrong( bool onoff ) throw ();
  static void setBar( bool onoff ) throw ();
  static void setUnderline( bool onoff ) throw ();
  static void setBlink( bool onoff ) throw ();
  static void setReverse( bool onoff ) throw ();
  
  static void resetProperties() throw ();
  
  static void move( int x, int y ) throw ();
  static int getX() throw ();
  static int getY() throw ();
  
  static int getWidth() throw ();
  static int getHeight() throw ();
};

}

#include "Cursor.ipp"

#endif
