/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_DFBIMAGE_H__
#define __CUTIL_DFBIMAGE_H__

extern "C" {
#include <directfb.h>
}

namespace cutil {

class DFBImage {
  friend class DFBSurface;

private:
  IDirectFBImageProvider *provider;
  
  //! non-copiable
  DFBImage( const DFBImage& );
  DFBImage& operator=( const DFBImage& );
  
public:
  DFBImage(); 
  DFBImage( const char *filename ); 
  ~DFBImage();

  bool open( const char *filename );
  void close();
  
  bool isOpen();
};

}

#include "DFBImage.ipp"

#endif
