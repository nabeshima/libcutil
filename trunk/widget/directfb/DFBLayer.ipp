/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "DFBSurface.h"

namespace cutil {


inline
DFBLayer::DFBLayer() {
  //! get device information
  IDirectFB *dfb = DFBEventThread::getDFB();

  DFBGraphicsDeviceDescription gdesc;
  dfb->GetDeviceDescription( dfb, &gdesc );
  
  //! get layer
  dfb->GetDisplayLayer( dfb, DLID_PRIMARY, &layer );
  
  layer->SetCooperativeLevel( layer, DLSCL_ADMINISTRATIVE );
  
  if ( !( ( gdesc.blitting_flags & DSBLIT_BLEND_ALPHACHANNEL ) &&
	  ( gdesc.blitting_flags & DSBLIT_BLEND_COLORALPHA ) ) ) {
    layer_config.flags = DLCONF_BUFFERMODE;
    layer_config.buffermode = DLBM_BACKSYSTEM;
    
    layer->SetConfiguration( layer, &layer_config );
  }
  
  layer->GetConfiguration( layer, &layer_config );
  
  //! to get cursor motion.
  layer->EnableCursor( layer, 1 );

  //! to hide cursor
  layer->SetCursorOpacity( layer, 50 );
}

inline
DFBLayer::~DFBLayer() {
  layer->Release( layer );
}

inline
int DFBLayer::getWidth() {
  return layer_config.width;
}

inline
int DFBLayer::getHeight() {
  return layer_config.height;
}

inline
void DFBLayer::setBackground( const DFBSurface &surface ) {
  layer->SetBackgroundImage( layer, surface.surface );
  layer->SetBackgroundMode( layer, DLBM_IMAGE );
}

}
