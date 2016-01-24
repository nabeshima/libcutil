/**
 * $Id: DFBSurface.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_DFBSurface_H__
#define __CUTIL_DFBSurface_H__

extern "C" {
#include <directfb.h>
}

namespace cutil {

class DFBImage;
class DFBFont;

class DFBSurface {
  friend class DFBLayer;
  friend class DFBWidget;

private:
  IDirectFBSurface *surface;
  
  DFBSurface( IDirectFBSurface *surface ); 
  
  //! non-copiable
  DFBSurface( const DFBSurface& );
  DFBSurface& operator=( const DFBSurface& );

public:
  DFBSurface( int width, int height ); 
  ~DFBSurface();
  
  DFBPoint getPosition();
  DFBDimension getSize();
  int getX();
  int getY();
  int getWidth();
  int getHeight();
  
  //! flip back buffer and front buffer.
  void flip();

  void drawImage( const DFBImage &image );
  void drawImage( const DFBImage &image, const DFBRectangle &rect );


  //   DSTF_LEFT                  0x00000000              left aligned
  //   DSTF_CENTER                0x00000001              horizontally centered
  //   DSTF_RIGHT                 0x00000002              right aligned
  //   DSTF_TOP                   0x00000004              y specifies the top instead of the baseline
  //   DSTF_BOTTOM                0x00000008              y specifies the bottom instead of the baseline
  //   DSTF_OUTLINE               0x00000010              enables outline rendering if loaded font supports it
  //   DSTF_TOPLEFT               DSTF_TOP | DSTF_LEFT            
  //   DSTF_TOPCENTER             DSTF_TOP | DSTF_CENTER          
  //   DSTF_TOPRIGHT              DSTF_TOP | DSTF_RIGHT           
  //   DSTF_BOTTOMLEFT            DSTF_BOTTOM | DSTF_LEFT                 
  //   DSTF_BOTTOMCENTER          DSTF_BOTTOM | DSTF_CENTER               
  //   DSTF_BOTTOMRIGHT           DSTF_BOTTOM | DSTF_RIGHT

  void drawString( const char *str, 
                   const DFBFont &font,
                   const DFBColor &color,
                   int x, int y,
                   DFBSurfaceTextFlags flag = DSTF_LEFT );

  void drawLine( const DFBColor &color,
                 int x1, int y1,
                 int x2, int y2 );

  void drawRectangle( const DFBColor &color,
                      int x, int y,
                      int width, int height,
                      bool fill_flag = false );

  
  void drawTriangle( const DFBColor &color,
                     int x1, int y1,
                     int x2, int y2,
                     int x3, int y3,
                     bool fill_flag = false );
};

}

#include "DFBSurface.ipp"

#endif
