/**
 * $Id: XEventThread.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "XWidget.h"

namespace cutil {

//! XEventThread instance is allocated at firstcall add() or getDisplay()

inline XEventThread& XEventThread::instance() {
  static XEventThread evthread;
  return evthread;
}

inline bool& XEventThread::xInitStatus() {
  static bool xinitstatus = false;
  return xinitstatus;
}

inline void XEventThread::XInitThreads() { xInitStatus() = true; }

inline XEventThread::XEventThread() {
  if (!xInitStatus()) {
    xInitStatus() = true;
    ::XInitThreads();
  }

  dpy = XOpenDisplay(NULL);
  screen = DefaultScreen(dpy);

  this->setInterval(10000);
  this->start();
}

inline XEventThread::~XEventThread() {  //! after main returned
  this->stop();
  this->join();

  XCloseDisplay(dpy);
}

inline Display* XEventThread::getDisplay() { return instance().dpy; }

inline int XEventThread::getScreen() { return instance().screen; }

inline void XEventThread::add(Window win, XWidget* wid) {
  instance().win_map.insert(std::pair<Window, XWidget*>(win, wid));
}

inline void XEventThread::erase(Window win) { instance().win_map.erase(win); }

inline bool XEventThread::lock() { return instance().event_mutex.lock(); }

inline bool XEventThread::unlock() { return instance().event_mutex.unlock(); }

inline Mutex& XEventThread::getMutex() { return instance().event_mutex; }

inline void XEventThread::eachLoop() {
  XEvent event;

  while (XPending(dpy)) {
    XNextEvent(dpy, &event);

    Lock lk(&event_mutex);

    std::map<Window, XWidget*>::iterator it = win_map.find(event.xany.window);

    if (it != win_map.end()) {
      XWidget* wid = it->second;

      switch (event.type) {
        case MapNotify:
          wid->showEvent();
          break;
        case UnmapNotify:
          wid->hideEvent();
          break;
        case Expose:
          wid->exposeEvent();
          break;
        case FocusIn:
          wid->focusInEvent();
          break;
        case FocusOut:
          wid->focusOutEvent();
          break;
        case ConfigureNotify:
          if (wid->getWidth() != event.xconfigurerequest.width ||
              wid->getHeight() != event.xconfigurerequest.height) {
            wid->resizeEventBase(event.xconfigurerequest.width,
                                 event.xconfigurerequest.height);
          }
          if (wid->getX() != event.xconfigurerequest.x ||
              wid->getY() != event.xconfigurerequest.y) {
            wid->moveEventBase(event.xconfigurerequest.x,
                               event.xconfigurerequest.y);
          }
          break;
        case EnterNotify:
          wid->mouseEnterEvent(event.xcrossing.x, event.xcrossing.y);
          break;
        case LeaveNotify:
          wid->mouseLeaveEvent(event.xcrossing.x, event.xcrossing.y);
          break;
        case MotionNotify:
          wid->mouseMoveEvent(event.xmotion.x, event.xmotion.y);
          break;
        case ButtonPress:
          wid->mousePressEvent(event.xbutton.button, event.xbutton.x,
                               event.xbutton.y);
          break;
        case ButtonRelease:
          wid->mouseReleaseEvent(event.xbutton.button, event.xbutton.x,
                                 event.xbutton.y);
          break;
        case KeyPress: {
          char buf[11];
          KeySym keysym;
          XLookupString(&event.xkey, buf, 10, &keysym, NULL);
          wid->keyPressEvent(keysym);
        } break;
        case KeyRelease: {
          char buf[11];
          KeySym keysym;
          XLookupString(&event.xkey, buf, 10, &keysym, NULL);
          wid->keyReleaseEvent(keysym);
        } break;
          //           switch( keysym ){
          //           case XK_Escape:
          //           case XK_BackSpace:
          //           case XK_Tab:
          //           case XK_Return:
          //           case XK_Delete:
          //           case XK_Insert:
          //           case XK_KP_Enter:
          //           case XK_Control_L:
          //           case XK_Control_R:
          //           case XK_Shift_L:
          //           case XK_Shift_R:
          //           case XK_Alt_L:
          //           case XK_Alt_R:
          //           case XK_Down:
          //           case XK_Up:
          //           case XK_Left:
          //           case XK_Right:
          //           }
      }
    }
  }
}
}
