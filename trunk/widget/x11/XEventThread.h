/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_XEVENTTHREAD_HH__
#define _CUTIL_XEVENTTHREAD_HH__


#include <map>
#include "Thread.h"

extern "C"{
#include <X11/Xlib.h>
#include <X11/Xutil.h>
}

namespace cutil {

class XWidget;


class XEventThread: public LoopThread {  
private:
  XEventThread() throw ();
  virtual ~XEventThread() throw ();
  
  static XEventThread& instance() throw ();
  static bool& xInitStatus() throw ();
  
  Display *dpy;
  int screen;
  
  std::map< Window, XWidget* > win_map;
  
  Mutex event_mutex;
  
public:
  static void XInitThreads() throw ();

  static Display* getDisplay() throw ();
  static int getScreen() throw ();

  static void add( Window win, XWidget *wid ) throw ();
  static void erase( Window win ) throw ();
  
  static void lock() 
    throw ( ThreadException );
  static void unlock()
    throw ( ThreadException );
  static Mutex& getMutex() throw ();
  
  virtual void eachLoop();
};

}

#include "XEventThread.ipp"

#endif
