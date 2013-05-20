/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_DFBFont_H__
#define __CUTIL_DFBFont_H__

extern "C" {
#include <directfb.h>
}

namespace cutil {

class DFBFont {
  friend class DFBSurface;
  
private:
  IDirectFBFont *font;

  //! non-copiable
  DFBFont( const DFBFont& );
  DFBFont& operator=( const DFBFont& );

public:
  DFBFont(); 
  DFBFont( const char *font_filename, int font_height ); 
  ~DFBFont();

  bool open( const char *font_filename, int font_height );
  void close();
  
  bool isOpen();

  int getHeight();
  int getWidth( const char *str );
};

}

#include "DFBFont.ipp"

#endif
