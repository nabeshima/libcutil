/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "DFBLayer.h"

namespace cutil {


inline
DFBWidget::DFBWidget()
  : id( 0 ),
    window( NULL ),
    window_surface( NULL ) {}

inline
DFBWidget::DFBWidget( DFBLayer &layer,
                      int x, int y,
                      int width, int height ) {
  open( layer, x, y, width, height );
}

inline
DFBWidget::~DFBWidget() {
  close();
}


inline
bool DFBWidget::open( DFBLayer &layer,
                      int x, int y,
                      int width, int height ) {

  if ( isOpen() ) {
    return false;
  }
  
  DFBWindowDescription  desc;
  
  desc.flags = (DFBWindowDescriptionFlags)( DWDESC_POSX | DWDESC_POSY |
                                           DWDESC_WIDTH | DWDESC_HEIGHT );
  
  desc.posx   = x;
  desc.posy   = y;
  desc.width  = width;
  desc.height = height;
  

  bool ret = ( DFB_OK ==
               layer.layer->CreateWindow( layer.layer, &desc, &window ) );
  
  if ( !ret ) {
    window = NULL;
    window_surface = NULL;
  }
  else {
    IDirectFBSurface *surface;
    window->GetSurface( window, &surface );
    window_surface = new DFBSurface( surface );

    window->SetOpacity( window, 0 );

    window->GetID( window, &id );    
    DFBEventThread::add( id, this );
  }
  
  return ret;
}


inline
void DFBWidget::close() {
  if ( isOpen() ) {
    DFBEventThread::erase( id );
    delete window_surface;
    window->Release( window );
    window_surface = NULL;
    window = NULL;
  }
}

inline
bool DFBWidget::isOpen() {
  return window != NULL;
}

inline
DFBSurface& DFBWidget::getSurface() {
  return *window_surface;
}

inline
DFBPoint DFBWidget::getPosition() {
  DFBPoint p;
  if ( !isOpen() ) {
    return p;
  }
  window->GetPosition( window, &p.x, &p.y );
  return p;
}

inline
DFBDimension DFBWidget::getSize() {
  DFBDimension d;
  if ( !isOpen() ) {
    return d;
  }
  window->GetSize( window, &d.w, &d.h );
  return d;
}

inline
int DFBWidget::getX() {
  return getPosition().x;
}

inline
int DFBWidget::getY() {
  return getPosition().y;
}

inline
int DFBWidget::getWidth() {
  return getSize().w;
}

inline
int DFBWidget::getHeight() {
  return getSize().h;
}

inline
void DFBWidget::setOpacity( unsigned char opacity ) {
  if ( !isOpen() ) {
    return;
  }
  
  window->SetOpacity( window, opacity );
}

inline
void DFBWidget::setVisible( bool onoff ) {
  setOpacity( onoff? 255: 0 );
}

inline
void DFBWidget::display() {
  window_surface->flip();
}

inline
void DFBWidget::resize( int width, int height ) {
  if ( !isOpen() ) {
    return;
  }
  window->Resize( window, width, height );
}

inline
void DFBWidget::move( int x, int y, bool diff_flag ) {
  if ( !isOpen() ) {
    return;
  }
  if ( diff_flag ) {
    window->Move( window, x, y );
  }
  else {
    window->MoveTo( window, x, y );
  }
}

inline
void DFBWidget::toFront( bool to_top ) {
  if ( !isOpen() ) {
    return;
  }
  window->Raise( window );
}

inline
void DFBWidget::toBack( bool to_bottom ) {
  if ( !isOpen() ) {
    return;
  }
  window->Lower( window );
}
  
inline
void DFBWidget::putFront( const DFBWidget &wid ) {
  if ( !isOpen() ) {
    return;
  }
  window->PutAtop( window, wid.window );
}

inline
void DFBWidget::putBack( const DFBWidget &wid ) {
  if ( !isOpen() ) {
    return;
  }
  window->PutBelow( window, wid.window );
}
  

inline
void DFBWidget::openEvent( int, int, 
                           int, int ) {}
inline
void DFBWidget::closeEvent() {}
inline
void DFBWidget::destroyEvent() {}
inline
void DFBWidget::focusInEvent() {}
inline
void DFBWidget::focusOutEvent() {}
inline
void DFBWidget::resizeEvent( int, int ) {}
inline
void DFBWidget::moveEvent( int, int ) {}

inline
void DFBWidget::mouseEnterEvent( int, int ) {}
inline
void DFBWidget::mouseLeaveEvent( int, int ) {}
inline
void DFBWidget::mouseMoveEvent( int, int ) {}

inline
void DFBWidget::mousePressEvent( int, int, int ) {}
inline
void DFBWidget::mouseReleaseEvent( int, int, int ) {}
inline
void DFBWidget::mouseWheelEvent( int, int, int ) {}

inline
void DFBWidget::keyPressEvent( DFBInputDeviceKeySymbol ) {}
inline
void DFBWidget::keyReleaseEvent( DFBInputDeviceKeySymbol ) {} 

}
