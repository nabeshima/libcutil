/**
 * $Id: XEventThread.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_XEVENTTHREAD_HH__
#define _CUTIL_XEVENTTHREAD_HH__

#include "Thread.h"

extern "C" {
#include <X11/Xlib.h>
#include <X11/Xutil.h>
}

#include <map>

namespace cutil {

class XWidget;

class XEventThread : public LoopThread {
 private:
  XEventThread();
  virtual ~XEventThread();

  static XEventThread& instance();
  static bool& xInitStatus();

  Display* dpy;
  int screen;

  std::map<Window, XWidget*> win_map;

  Mutex event_mutex;

 public:
  static void XInitThreads();

  static Display* getDisplay();
  static int getScreen();

  static void add(Window win, XWidget* wid);
  static void erase(Window win);

  static bool lock();
  static bool unlock();
  static Mutex& getMutex();

  virtual void eachLoop();
};
}  // namespace cutil

#include "XEventThread.ipp"

#endif
