/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_XWIDGET_HH__
#define _CUTIL_XWIDGET_HH__


#include "XWidgetException.h"

extern "C"{
#include <X11/Xlib.h>
#include <X11/Xutil.h>
}


namespace cutil {

/*! \class XWidget
  描画するための最小のプログラム.
  Xlibを使ったウィンドウクラス.
  Windowsで使う場合は作り替える必要有.
*/

class XWidget {
  friend class XEventThread;
  
private:  
  bool open_flag;
  bool resizable_flag;
  
  std::string title;
  
  XImage *img;
  unsigned char *framebuf;

  int x, y, width, height, size, bufsize;

  //! non-copiable
  XWidget( const XWidget& );
  XWidget& operator=( const XWidget& );
  
protected:
  XVisualInfo vi;
  Window win;
  
  virtual XVisualInfo chooseVisual();
  
public:
  XWidget() throw ();
  virtual ~XWidget();

  bool isOpen() const throw ();
  
  void open( int width, int height, const std::string &title = "" ) throw ();
  void close() throw ();
  
  int getX() const throw ();
  int getY() const throw ();
  int getWidth() const throw ();
  int getHeight() const throw ();

  //! resizabilityの変更
  void setResizable( bool onoff ) throw ();

  //! ウィジェットのサイズ変更
  void resize( int width, int height ) throw ();
    
  //! ウィジェットの移動
  void move( int x, int y ) throw ();
  
  //! image_bitは24と32のみ. 24なら,内部で1pixelあたり24bitの画像を32bitに変換する.
  void display( const unsigned char *image, int image_bits = 24 ) 
    throw ( XWidgetException );
  
private:
  void resizeEventBase( int width, int height )
    throw ( XWidgetException );
  void moveEventBase( int x, int y )
    throw ( XWidgetException );

protected:
  //! Xserverからのイベントスロット
  virtual void showEvent();
  virtual void hideEvent();
  virtual void exposeEvent();
  virtual void focusInEvent();
  virtual void focusOutEvent();
  virtual void resizeEvent( int width, int height );
  virtual void moveEvent( int x, int y );
  
  virtual void mouseEnterEvent( int x, int y );
  virtual void mouseLeaveEvent( int x, int y );
  virtual void mouseMoveEvent( int x, int y );

  virtual void mousePressEvent( int button, int x, int y );
  virtual void mouseReleaseEvent( int button, int x, int y );
  
  virtual void keyPressEvent( KeySym keysym );
  virtual void keyReleaseEvent( KeySym keysym );

public:
  static void convert32to24( const unsigned char *buf32, unsigned char *buf24, int WxH_size ) throw ();
  static void convert24to32( const unsigned char *buf24, unsigned char *buf32, int WxH_size ) throw ();
  //! glReadPixelsはy軸が逆の上,RGBでなくBGRになっている.
  static void swapRGBandAxis24( const unsigned char *src, unsigned char *dest,
				int width, int height ) throw ();
  static void swapRGBandAxis32( const unsigned char *src, unsigned char *dest,
				const int width, int height ) throw ();
  static void swapRGB24( unsigned char *src, int width, int height ) throw ();
};

}

#include "XWidget.ipp"

#endif
