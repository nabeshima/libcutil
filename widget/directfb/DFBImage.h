/**
 * $Id: DFBImage.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
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
