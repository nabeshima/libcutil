/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cstdio>

extern "C" {
#include <sys/ioctl.h>
#include <termios.h>
}

namespace cutil {

inline
void Cursor::_getPosition()
  throw ( CursorException ) {
  termios pre_term_attr, term_attr;
  
  if ( ::tcgetattr( STDIN_FILENO, &term_attr ) < 0 ) {
    throw CursorException( "::tcgetattr error", __EXCEPTION_INFO__ );
  }
  pre_term_attr = term_attr;
  
  term_attr.c_lflag &= ~( ICANON | ECHO );
  term_attr.c_cc[ VMIN ] = 1;
  term_attr.c_cc[ VTIME ] = 0;
  
  if ( ::tcsetattr( STDIN_FILENO, TCSANOW, &term_attr ) < 0 ) {
    throw CursorException( "::tcsetattr error", __EXCEPTION_INFO__ );
  }
  
  ::printf( "\x1b[6n" );
  ::fflush( stdout );
  ::scanf( "\x1b[%d;%dR", &y, &x);

  if ( ::tcsetattr( STDIN_FILENO, TCSANOW, &pre_term_attr ) < 0 ) {
    throw CursorException( "::tcsetattr error", __EXCEPTION_INFO__ );
  }
}

inline
void Cursor::_getWinSize() 
  throw ( CursorException ) {
  winsize ws;
  int rc = ::ioctl( STDOUT_FILENO, TIOCGWINSZ, &ws, sizeof( ws ) );
  if ( rc == -1 ) {
    throw CursorException( "::ioctl TIOCGWINSZ error", __EXCEPTION_INFO__ );
  }
  h = ws.ws_row;
  w = ws.ws_col;
}

inline
void Cursor::_setProperties() throw () {
  ::printf( "\x1b[0;" );
  
  if ( foreground != -1 ) {
    ::printf( ";%d", foreground + 30 );
  }
  if ( background != -1 ) {
    ::printf( ";%d", background + 40 );
  }
  
  if ( strong ) {
    ::printf( ";%d", 1 );
  }
  if ( bar ) {
    ::printf( ";%d", 2 );
  }
  if ( underline ) {
    ::printf( ";%d", 4 );
  }
  if ( blink ) {
    ::printf( ";%d", 5 );
  }
  if ( reverse ) {
    ::printf( ";%d", 7 );
  }
  ::printf( "m" );

  ::printf( "\x1b[38;6m" );
  ::fflush( stdout );
}

inline  
void Cursor::_resetProperties() throw () {
  ::printf( "\x1b[0m" );
  ::fflush( stdout );
  foreground = -1;
  background = -1;
  strong = false;
  bar = false;
  underline = false;
  blink = false;
  reverse = false;
}
  
inline
void Cursor::_clear() throw () {
  ::printf( "\x1b[2J" );
  ::fflush( stdout );
}
  
inline
void Cursor::_show() throw () {
  ::printf( "\x1b[>5l" );
  ::fflush( stdout );
}

inline
void Cursor::_hide() throw () {
  ::printf( "\x1b[>5h" );
  ::fflush( stdout );
}
  
inline
void Cursor::_move( int x, int y ) throw () {
  ::printf( "\x1b[%d;%dH", y, x );
  ::fflush( stdout );
  this->x = x;
  this->y = y;
}

inline
Cursor::Cursor() throw ()
  : foreground( -1 ), background( -1 ),
    strong( false ), bar( false ), underline( false ), blink( false ), reverse( false ) {
  _getWinSize();
  _getPosition();
  _resetProperties();
  _setProperties();
  _show();
}

inline
Cursor::~Cursor() throw () {
  _resetProperties();
}

inline
Cursor& Cursor::instance() throw () {
  static Cursor cursor;
  return cursor;
}


inline
void Cursor::setForeground( int color ) throw () {
  instance().foreground = color;
  instance()._setProperties();
}
inline
void Cursor::setBackground( int color ) throw () {
  instance().background = color;
  instance()._setProperties();
}

inline
void Cursor::setStrong( bool onoff ) throw () {
  instance().strong = onoff;
  instance()._setProperties();
}

inline
void Cursor::setBar( bool onoff ) throw () {
  instance().bar = onoff;
  instance()._setProperties();
}
inline
void Cursor::setUnderline( bool onoff ) throw () {
  instance().underline = onoff;
  instance()._setProperties();
}
inline
void Cursor::setBlink( bool onoff ) throw () {
  instance().blink = onoff;
  instance()._setProperties();
}
inline
void Cursor::setReverse( bool onoff ) throw () {
  instance().reverse = onoff;
  instance()._setProperties();
}

inline
void Cursor::resetProperties() throw () {
  instance()._resetProperties();
}

inline
void Cursor::move( int x, int y ) throw () {
  instance()._move( x, y );
}
inline
int Cursor::getX() throw () {
  instance()._getPosition();
  return instance().x;
}
inline
int Cursor::getY() throw () {
  instance()._getPosition();
  return instance().y;
}


inline
int Cursor::getWidth() throw () {
  instance()._getWinSize();
  return instance().w;
}
inline
int Cursor::getHeight() throw () {
  instance()._getWinSize();
  return instance().h;
}


}
