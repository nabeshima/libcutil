/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "DFBEventThread.h"

namespace cutil {

inline
DFBImage::DFBImage() 
  : provider( NULL ) {}

inline
DFBImage::DFBImage( const char *filename ) {
  open( filename );
}

inline
DFBImage::~DFBImage() {
  close();
}

inline 
bool DFBImage::open( const char *filename ) {
  if ( isOpen() ) {
    return false;
  }

  IDirectFB *dfb = DFBEventThread::getDFB();

  bool ret = ( DFB_OK ==
	       dfb->CreateImageProvider( dfb,
                                         filename,
                                         &provider ) );
  if ( !ret ) {
    provider = NULL;
  }

  return ret;
}

inline
void DFBImage::close() {
  if ( isOpen() ) {
    provider->Release( provider );
    provider = NULL;
  }
}

inline
bool DFBImage::isOpen() {
  return provider != NULL;
}



}
