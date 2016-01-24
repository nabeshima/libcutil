/**
 * $Id: DFBWidget.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_DFBWIDGET_H__
#define __CUTIL_DFBWIDGET_H__

extern "C" {
#include <directfb.h>
}

namespace cutil {

class DFBSurface;
class DFBLayer;


class DFBWidget {
  friend class DFBEventThread;

private:
  DFBWindowID id;
  IDirectFBWindow *window;
  DFBSurface *window_surface;
  
  //! non-copiable
  DFBWidget( const DFBWidget& );
  DFBWidget& operator=( const DFBWidget& );
  
public:
  DFBWidget();
  DFBWidget( DFBLayer &layer,
             int x, int y,
             int width, int height );
  virtual ~DFBWidget();
  
  bool open( DFBLayer &layer,
             int x, int y,
             int width, int height );
  void close();
  
  bool isOpen();
  
  DFBSurface& getSurface();
  
  DFBPoint getPosition();
  DFBDimension getSize();
  int getX();
  int getY();
  int getWidth();
  int getHeight();
  
  void setOpacity( unsigned char opacity );
  void setVisible( bool onoff );

  void display();

  void resize( int width, int height );
  void move( int x, int y, bool diff_flag = false );

  void toFront( bool to_top = true );
  void toBack( bool to_bottom = true );
  
  void putFront( const DFBWidget &wid );
  void putBack( const DFBWidget &wid );

  
  //! event callbacks
  virtual void openEvent( int x, int y, 
                          int w, int h );
  virtual void closeEvent();
  virtual void destroyEvent();
  virtual void focusInEvent();
  virtual void focusOutEvent();
  virtual void resizeEvent( int w, int h );
  virtual void moveEvent( int x, int y );
  
  virtual void mouseEnterEvent( int x, int y );
  virtual void mouseLeaveEvent( int x, int y );
  virtual void mouseMoveEvent( int x, int y );

  //   DIBI_LEFT    0x00000000  left mouse button
  //   DIBI_RIGHT   0x00000001  right mouse button
  //   DIBI_MIDDLE  0x00000002  middle mouse button
  //   DIBI_FIRST   DIBI_LEFT   other buttons: DIBI_FIRST + zero based index
  //   DIBI_LAST    0x0000001F  32 buttons maximum
  virtual void mousePressEvent( int button, int x, int y );
  virtual void mouseReleaseEvent( int button, int x, int y );
  virtual void mouseWheelEvent( int step, int x, int y );

  virtual void keyPressEvent( DFBInputDeviceKeySymbol keysym );
  virtual void keyReleaseEvent( DFBInputDeviceKeySymbol keysym );
};


}

#include "DFBWidget.ipp"

#endif
