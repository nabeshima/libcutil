/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "DFBEventThread.h"
#include "DFBImage.h"
#include "DFBFont.h"

namespace cutil {


inline
DFBSurface::DFBSurface( IDirectFBSurface *surface )
  : surface( surface ) {}

inline
DFBSurface::DFBSurface( int width, int height ) {

  DFBSurfaceDescription desc;
  
  desc.flags  = (DFBSurfaceDescriptionFlags)( DSDESC_WIDTH | DSDESC_HEIGHT | DSDESC_CAPS );
  desc.width  = width;
  desc.height = height;
  desc.caps   = DSCAPS_SHARED;
  
  IDirectFB *dfb = DFBEventThread::getDFB();
  dfb->CreateSurface( dfb, &desc, &surface );
}

inline 
DFBSurface::~DFBSurface() {
  surface->Release( surface );
}



inline
DFBPoint DFBSurface::getPosition() {
  DFBPoint p;
  surface->GetPosition( surface, &p.x, &p.y );
  return p;
}

inline
DFBDimension DFBSurface::getSize() {
  DFBDimension d;
  surface->GetSize( surface, &d.w, &d.h );
  return d;
}

inline
int DFBSurface::getX() {
  return getPosition().x;
}

inline
int DFBSurface::getY() {
  return getPosition().y;
}

inline
int DFBSurface::getWidth() {
  return getSize().w;
}

inline
int DFBSurface::getHeight() {
  return getSize().h;
}

inline
void DFBSurface::flip() {
  surface->Flip( surface, NULL, DSFLIP_NONE );
}

inline
void DFBSurface::drawImage( const DFBImage &image ) {
  image.provider->RenderTo( image.provider, surface, NULL );
}

inline
void DFBSurface::drawImage( const DFBImage &image, const DFBRectangle &rect ) {
  image.provider->RenderTo( image.provider, surface, &rect );
}

inline
void DFBSurface::drawString( const char *str, 
			     const DFBFont &font,
			     const DFBColor &color,
			     int x, int y,
			     DFBSurfaceTextFlags flag ) {
  surface->SetFont( surface, font.font );
  surface->SetColor( surface, color.r, color.g, color.b, color.a ); 
  surface->DrawString( surface,
		       str, -1, 
		       x, y, flag );
}


inline
void DFBSurface::drawLine( const DFBColor &color,
                           int x1, int y1,
                           int x2, int y2 ) {
  surface->SetColor( surface, color.r, color.g, color.b, color.a ); 
  surface->DrawLine( surface, x1, y1, x2, y2 );  
}


inline
void DFBSurface::drawRectangle( const DFBColor &color,
                                int x, int y,
                                int width, int height,
                                bool fill_flag ) {
  surface->SetColor( surface, color.r, color.g, color.b, color.a ); 
  if ( fill_flag ) {
    surface->FillRectangle( surface,
                            x, y, 
                            width, height );
  }
  else {
    surface->DrawRectangle( surface,
                            x, y, 
                            width, height );
  }
}


inline
void DFBSurface::drawTriangle( const DFBColor &color,
                               int x1, int y1,
                               int x2, int y2,
                               int x3, int y3,
                               bool fill_flag ) {
  surface->SetColor( surface, color.r, color.g, color.b, color.a ); 
  if ( fill_flag ) {
    surface->FillTriangle( surface,
                           x1, y1, 
                           x2, y2, 
                           x3, y3 );
  }
  else {
    surface->DrawLine( surface, x1, y1, x2, y2 );
    surface->DrawLine( surface, x2, y2, x3, y3 );
    surface->DrawLine( surface, x3, y3, x1, y1 );
  }
}

}
