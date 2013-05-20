/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "DFBWidget.h"

namespace cutil {


inline
DFBEventThread::DFBEventThread() 
  : initFlag( false ),
    buffer( NULL ) {
  
  DirectFBInit( NULL, NULL );
  DirectFBCreate( &dfb );  
  
  this->setInterval( 10000 );
  this->start();
}

inline
DFBEventThread::~DFBEventThread() {
  this->stop();
  this->join();
  
  dfb->Release( dfb );

  if ( initFlag ) {
    buffer->Release( buffer );
  }
}

inline
DFBEventThread& DFBEventThread::instance() {
  static DFBEventThread evthread;
  return evthread;
}

inline
IDirectFB* DFBEventThread::getDFB() {
  return instance().dfb;
}

inline
void DFBEventThread::add( DFBWindowID win, DFBWidget *wid ) {
  instance().win_map.insert( std::pair< DFBWindowID, DFBWidget* >( win, wid ) );

  if ( !instance().initFlag ) {
    wid->window->CreateEventBuffer( wid->window, &instance().buffer );
    instance().initFlag = true;
  }
  else {
    wid->window->AttachEventBuffer( wid->window, instance().buffer );
  }
}

inline
void DFBEventThread::erase( DFBWindowID win ) {
  instance().win_map.erase( win );
}


inline
void DFBEventThread::lock() {
  instance().event_mutex.lock();
}

inline
void DFBEventThread::unlock() {
  instance().event_mutex.unlock();
}

inline
Mutex& DFBEventThread::getMutex() {
  return instance().event_mutex;
}

inline  
void DFBEventThread::eachLoop() {
  
  if ( buffer == NULL ) {
    return;
  }
  
  DFBWindowEvent event;

  buffer->WaitForEventWithTimeout( buffer, 0, 10 );

  //! we can use other necessary events by the same way as this sequence.

  while ( DFB_OK == buffer->GetEvent( buffer, DFB_EVENT( &event ) ) ) {
    Lock lk( event_mutex );
    
    std::map< DFBWindowID, DFBWidget* >::iterator
      it = win_map.find( event.window_id );
    
    if ( it != win_map.end() ) {
      DFBWidget* wid = it->second;
      event_time = event.timestamp;
      
      switch ( event.type ) {
      case DWET_POSITION_SIZE:
        wid->openEvent( event.x, event.y, 
                        event.w, event.h );
        break;
      case DWET_CLOSE:
        wid->closeEvent();
        break;
      case DWET_DESTROYED:
        wid->destroyEvent();
        break;
      case DWET_GOTFOCUS:
        wid->focusInEvent();
        break;
      case DWET_LOSTFOCUS:
        wid->focusOutEvent();
        break;
      case DWET_SIZE:
        wid->resizeEvent( event.w, event.h );
        break;
      case DWET_POSITION:
        wid->moveEvent( event.x, event.y );
        break;
      case DWET_ENTER:
        wid->mouseEnterEvent( event.cx, event.cy );
        break;
      case DWET_LEAVE:
        wid->mouseLeaveEvent( event.cx, event.cy );
        break;
      case DWET_MOTION:
        wid->mouseMoveEvent( event.cx,
                             event.cy );
        break;
      case DWET_BUTTONDOWN:
        wid->mousePressEvent( event.button,
                              event.cx,
                              event.cy );
        break;
      case DWET_BUTTONUP:
        wid->mouseReleaseEvent( event.button,
                                event.cx,
                                event.cy );
        break;
      case DWET_WHEEL:
        wid->mouseWheelEvent( event.step,
                              event.cx,
                              event.cy );
        break;
      case DWET_KEYDOWN:
        wid->keyPressEvent( event.key_symbol );
        break;
      case DWET_KEYUP:
        wid->keyReleaseEvent( event.key_symbol );
        break;
      case DWET_NONE:
      case DWET_ALL:
        break;
      }
    }
  }
}

inline
timeval DFBEventThread::lastEventTime() {
  return event_time;
}

}
