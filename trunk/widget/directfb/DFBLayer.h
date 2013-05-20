/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_DFBLAYER_H__
#define __CUTIL_DFBLAYER_H__

extern "C" {
#include <directfb.h>
}

namespace cutil {

class DFBSurface;
  
class DFBLayer {
  friend class DFBWidget;

private:
  IDirectFBDisplayLayer *layer;
  DFBDisplayLayerConfig layer_config;
  
  //! non-copiable
  DFBLayer( const DFBLayer& );
  DFBLayer& operator=( const DFBLayer& );
    
public:
  DFBLayer();
  ~DFBLayer();
  
  int getWidth();
  int getHeight();

  void setBackground( const DFBSurface &surface );
};

}

#include "DFBLayer.ipp"

#endif
