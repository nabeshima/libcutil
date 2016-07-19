/**
 * $Id: Cursor.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cstdio>

extern "C" {
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>
}

namespace cutil {

inline bool Cursor::_getPosition() {
  termios pre_term_attr, term_attr;

  if (::tcgetattr(STDIN_FILENO, &term_attr) < 0) {
    x = -1;
    y = -1;
    return false;
  }
  pre_term_attr = term_attr;

  term_attr.c_lflag &= ~(ICANON | ECHO);
  term_attr.c_cc[VMIN] = 1;
  term_attr.c_cc[VTIME] = 0;

  if (::tcsetattr(STDIN_FILENO, TCSANOW, &term_attr) < 0) {
    x = -1;
    y = -1;
    return false;
  }

  ::printf("\x1b[6n");
  ::fflush(stdout);
  ::scanf("\x1b[%d;%dR", &y, &x);

  if (::tcsetattr(STDIN_FILENO, TCSANOW, &pre_term_attr) < 0) {
    x = -1;
    y = -1;
    return false;
  }
  return true;
}

inline bool Cursor::_getWinSize() {
  winsize ws;
  int rc = ::ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws, sizeof(ws));
  if (rc == -1) {
    w = -1;
    h = -1;
    return false;
  }
  w = ws.ws_col;
  h = ws.ws_row;
  return true;
}

inline void Cursor::_setProperties() {
  ::printf("\x1b[0;");

  if (foreground != -1) {
    ::printf(";%d", foreground + 30);
  }
  if (background != -1) {
    ::printf(";%d", background + 40);
  }

  if (strong) {
    ::printf(";%d", 1);
  }
  if (bar) {
    ::printf(";%d", 2);
  }
  if (underline) {
    ::printf(";%d", 4);
  }
  if (blink) {
    ::printf(";%d", 5);
  }
  if (reverse) {
    ::printf(";%d", 7);
  }
  ::printf("m");

  ::printf("\x1b[38;6m");
  ::fflush(stdout);
}

inline void Cursor::_resetProperties() {
  ::printf("\x1b[0m");
  ::fflush(stdout);
  foreground = -1;
  background = -1;
  strong = false;
  bar = false;
  underline = false;
  blink = false;
  reverse = false;
}

inline void Cursor::_clear() {
  ::printf("\x1b[2J");
  ::fflush(stdout);
}

inline void Cursor::_show() {
  ::printf("\x1b[>5l");
  ::fflush(stdout);
}

inline void Cursor::_hide() {
  ::printf("\x1b[>5h");
  ::fflush(stdout);
}

inline void Cursor::_move(int x, int y) {
  ::printf("\x1b[%d;%dH", y, x);
  ::fflush(stdout);
  this->x = x;
  this->y = y;
}

inline Cursor::Cursor()
    : x(-1),
      y(-1),
      w(-1),
      h(-1),
      foreground(-1),
      background(-1),
      strong(false),
      bar(false),
      underline(false),
      blink(false),
      reverse(false) {
  _getWinSize();
  _getPosition();
  _resetProperties();
  _setProperties();
  _show();
}

inline Cursor::~Cursor() { _resetProperties(); }

inline Cursor& Cursor::instance() {
  static Cursor cursor;
  return cursor;
}

inline void Cursor::setForeground(int color) {
  instance().foreground = color;
  instance()._setProperties();
}
inline void Cursor::setBackground(int color) {
  instance().background = color;
  instance()._setProperties();
}

inline void Cursor::setStrong(bool onoff) {
  instance().strong = onoff;
  instance()._setProperties();
}

inline void Cursor::setBar(bool onoff) {
  instance().bar = onoff;
  instance()._setProperties();
}
inline void Cursor::setUnderline(bool onoff) {
  instance().underline = onoff;
  instance()._setProperties();
}
inline void Cursor::setBlink(bool onoff) {
  instance().blink = onoff;
  instance()._setProperties();
}
inline void Cursor::setReverse(bool onoff) {
  instance().reverse = onoff;
  instance()._setProperties();
}

inline void Cursor::resetProperties() { instance()._resetProperties(); }

inline void Cursor::move(int x, int y) { instance()._move(x, y); }
inline int Cursor::getX() {
  instance()._getPosition();
  return instance().x;
}
inline int Cursor::getY() {
  instance()._getPosition();
  return instance().y;
}

inline int Cursor::getWidth() {
  instance()._getWinSize();
  return instance().w;
}
inline int Cursor::getHeight() {
  instance()._getWinSize();
  return instance().h;
}
}
