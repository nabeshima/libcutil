/**
 * $Id: XWidget.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cstring>

#include "XEventThread.h"

namespace cutil {

inline XVisualInfo XWidget::chooseVisual() {
  XVisualInfo vinfo;
  XMatchVisualInfo(XEventThread::getDisplay(), XEventThread::getScreen(), 24,
                   TrueColor, &vinfo);
  return vinfo;
}

inline XWidget::XWidget()
    : open_flag(false),
      resizable_flag(true),
      img(NULL),
      framebuf(NULL),
      x(0),
      y(0),
      width(0),
      height(0),
      size(0),
      bufsize(0) {}

inline XWidget::~XWidget() { close(); }

inline bool XWidget::isOpen() const { return open_flag; }

inline void XWidget::open(int width, int height, const std::string &title) {
  if (isOpen()) {
    return;
  }

  this->width = width;
  this->height = height;
  this->size = width * height;
  this->bufsize = size * 4;
  this->title = title;

  Display *dpy = XEventThread::getDisplay();
  int screen = XEventThread::getScreen();

  //! Visualの選択
  vi = this->chooseVisual();
  Visual *vis = vi.visual;

  //! カラーマップを作る
  Colormap cmap = XCreateColormap(dpy, RootWindow(dpy, screen), vis, AllocNone);

  //! ウィンドウ属性
  XSetWindowAttributes xattr;
  xattr.colormap = cmap;
  xattr.event_mask = ExposureMask;
  xattr.background_pixel = BlackPixel(dpy, screen);
  xattr.override_redirect = True;

  //! 属性マスク
  // ウィンドウマネージャによる介入を許可
  //  unsigned long attribmask =
  //  CWEventMask|CWColormap|CWBackPixel|CWOverrideRedirect ;
  unsigned long attribmask = CWColormap | CWBackPixel;

  //! ウィンドウ生成
  win = XCreateWindow(dpy, RootWindow(dpy, screen), 0, 0, width, height, 1,
                      CopyFromParent, InputOutput, vis, attribmask, &xattr);

  //! ウィンドウの情報取得
  XWindowAttributes attr;
  XGetWindowAttributes(dpy, win, &attr);
  x = attr.x;
  y = attr.y;

  //! ウィンドウの名前を設定.
  if (title.size() != 0) {
    XStoreName(dpy, win, title.c_str());
  }

  //! リサイズ可否を設定.
  XSizeHints sizeHints;
  if (!resizable_flag) {
    sizeHints.flags = PMinSize | PMaxSize;
    sizeHints.min_width = sizeHints.max_width = width;
    sizeHints.min_height = sizeHints.max_height = height;
  } else {
    sizeHints.flags = USSize;
    sizeHints.width = width;
    sizeHints.height = height;
  }
  XSetWMNormalHints(dpy, win, &sizeHints);

  //! どのEventを待つか.
  XSelectInput(dpy, win, ExposureMask | ButtonPressMask | ButtonReleaseMask |
                             EnterWindowMask | LeaveWindowMask |
                             PointerMotionMask | FocusChangeMask |
                             KeyPressMask | KeyReleaseMask |
                             SubstructureNotifyMask | StructureNotifyMask |
                             SubstructureRedirectMask);

  //! イベントループのロック
  {
    Lock lk(&XEventThread::getMutex());

    //! Event threadに登録.
    XEventThread::add(win, this);
  }

  //! ウィンドウ描画
  XMapWindow(dpy, win);

  //! 開くまで待つ
  XSync(dpy, win);

  open_flag = true;
}

inline void XWidget::close() {
  if (!isOpen()) {
    return;
  }

  {
    Lock lk(&XEventThread::getMutex());

    //! Event threadから削除.
    XEventThread::erase(win);
  }

  if (NULL != framebuf) {
    XDestroyImage(img);  // 内部でdelete[] framebuf;される.
    framebuf = NULL;
  }

  Display *dpy = XEventThread::getDisplay();

  XUnmapWindow(dpy, win);    //! 非表示
  XDestroyWindow(dpy, win);  //! 消去

  open_flag = false;
}

inline int XWidget::getX() const { return x; }

inline int XWidget::getY() const { return y; }

inline int XWidget::getWidth() const { return width; }

inline int XWidget::getHeight() const { return height; }

inline void XWidget::setResizable(bool onoff) {
  if (!isOpen()) {
    resizable_flag = onoff;
    return;
  }

  XSizeHints sizeHints;

  if (!onoff) {
    sizeHints.flags = PMinSize | PMaxSize;
    sizeHints.min_width = sizeHints.max_width = width;
    sizeHints.min_height = sizeHints.max_height = height;
  } else {
    sizeHints.flags = USSize;
    sizeHints.width = width;
    sizeHints.height = height;
  }

  XSetWMNormalHints(XEventThread::getDisplay(), win, &sizeHints);

  resizable_flag = onoff;
}

inline void XWidget::resize(int width, int height) {
  if (!isOpen()) {
    return;
  }

  XResizeWindow(XEventThread::getDisplay(), win, width, height);
}

inline void XWidget::move(int x, int y) {
  if (!isOpen()) {
    return;
  }

  XMoveWindow(XEventThread::getDisplay(), win, x, y);
}

inline bool XWidget::display(const unsigned char *image, int image_bits) {
  if (!isOpen()) {
    return false;
  }

  Display *dpy = XEventThread::getDisplay();

  if (NULL == framebuf) {  //! 初めてdisplayを呼んだら,イメージを作る
    //! フレームバッファ作成
    framebuf = new unsigned char[bufsize];

    //! direct map
    img = XCreateImage(dpy, vi.visual, vi.depth, ZPixmap, 0, (char *)framebuf,
                       width, height, 8, 0);
    if (!XInitImage(img)) {
      return false;
    }
  }

  if (image_bits == 24) {
    convert24to32(image, framebuf, size);
  } else if (image_bits == 32) {
    memcpy(framebuf, image, bufsize);
  } else {  //! unsupported image_bits;
    return false;
  }

  XPutImage(dpy, win, DefaultGC(dpy, XEventThread::getScreen()), img, 0, 0, 0,
            0, width, height);
  return true;
}

inline bool XWidget::resizeEventBase(int width, int height) {
  if (width != this->width || height != this->height) {
    this->width = width;
    this->height = height;
    size = width * height;
    bufsize = size * 4;

    if (NULL != framebuf) {
      XDestroyImage(img);  // 内部でdelete[] framebuf;される.

      //! フレームバッファ作成
      framebuf = new unsigned char[bufsize];

      //! direct map
      img = XCreateImage(XEventThread::getDisplay(), vi.visual, vi.depth,
                         ZPixmap, 0, (char *)framebuf, width, height, 8, 0);

      if (!XInitImage(img)) {
        return false;
      }
    }
  }

  resizeEvent(width, height);
  return true;
}

inline void XWidget::moveEventBase(int x, int y) {
  this->x = x;
  this->y = y;

  moveEvent(x, y);
}

inline void XWidget::showEvent() {}
inline void XWidget::hideEvent() {}
inline void XWidget::exposeEvent() {}
inline void XWidget::focusInEvent() {}
inline void XWidget::focusOutEvent() {}
inline void XWidget::resizeEvent(int, int) {}
inline void XWidget::moveEvent(int, int) {}

inline void XWidget::mouseEnterEvent(int, int) {}
inline void XWidget::mouseLeaveEvent(int, int) {}
inline void XWidget::mouseMoveEvent(int, int) {}

inline void XWidget::mousePressEvent(int, int, int) {}
inline void XWidget::mouseReleaseEvent(int, int, int) {}

inline void XWidget::keyPressEvent(KeySym) {}
inline void XWidget::keyReleaseEvent(KeySym) {}

inline void XWidget::convert32to24(const unsigned char *buf32,
                                   unsigned char *buf24, int WxH_size) {
  const unsigned char *b32 = buf32;
  unsigned char *b24 = buf24;

  for (int i = 0; i < WxH_size; ++i) {
    *(b24 + 0) = *(b32 + 0);
    *(b24 + 1) = *(b32 + 1);
    *(b24 + 2) = *(b32 + 2);
    b24 += 3;
    b32 += 4;
  }
}

inline void XWidget::convert24to32(const unsigned char *buf24,
                                   unsigned char *buf32, int WxH_size) {
  const unsigned char *b24 = buf24;
  unsigned char *b32 = buf32;

  for (int i = 0; i < WxH_size; ++i) {
    *(b32 + 0) = *(b24 + 0);
    *(b32 + 1) = *(b24 + 1);
    *(b32 + 2) = *(b24 + 2);
    b24 += 3;
    b32 += 4;
  }
}

//! glReadPixelsはy軸が逆の上,RGBでなくBGRになっている.
inline void XWidget::swapRGBandAxis24(const unsigned char *src,
                                      unsigned char *dest, int width,
                                      int height) {
  int W = width * 3;

  const unsigned char *s = src;
  unsigned char *d = dest + (height - 1) * W;

  for (int i = 0; i < height; ++i) {
    const unsigned char *ss = s;
    unsigned char *dd = d;

    for (int j = 0; j < width; ++j) {
      *(dd + 0) = *(ss + 2);
      *(dd + 1) = *(ss + 1);
      *(dd + 2) = *(ss + 0);
      dd += 3;
      ss += 3;
    }

    s += W;
    d -= W;
  }
}

inline void XWidget::swapRGBandAxis32(const unsigned char *src,
                                      unsigned char *dest, int width,
                                      int height) {
  int W = width * 4;

  const unsigned char *s = src;
  unsigned char *d = dest + (height - 1) * W;

  for (int i = 0; i < height; ++i) {
    const unsigned char *ss = s;
    unsigned char *dd = d;

    for (int j = 0; j < width; ++j) {
      *(dd + 0) = *(ss + 2);
      *(dd + 1) = *(ss + 1);
      *(dd + 2) = *(ss + 0);
      dd += 4;
      ss += 4;
    }

    s += W;
    d -= W;
  }
}

inline void XWidget::swapRGB24(unsigned char *src, int width, int height) {
  int W = width * 3;
  unsigned char *s = src;

  for (int i = 0; i < height; ++i) {
    unsigned char *ss = s;

    for (int j = 0; j < width; ++j) {
      unsigned char tmp = *(ss + 0);
      *(ss + 0) = *(ss + 2);
      *(ss + 2) = tmp;
      ss += 3;
    }

    s += W;
  }
}
}
