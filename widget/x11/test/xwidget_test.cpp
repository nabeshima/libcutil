/**
 * $Id: xwidget_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <iostream>

#include "XWidget.h"
#include "Thread.h"

using namespace std;
using namespace cutil;

class MyXWidget: public XWidget {
public:
  MyXWidget() throw ()
  : XWidget() {}
  virtual ~MyXWidget() throw () {}

protected:
  virtual void showEvent() {
    cout << "shown" << endl; 
  }
  virtual void hideEvent() {
    cout << "hidden" << endl; 
  }
  virtual void exposeEvent() {
    cout << "exposed" << endl; 
  }
  virtual void focusInEvent() {
    cout << "focused in" << endl; 
  }
  virtual void focusOutEvent() {
    cout << "focused out" << endl; 
  }
  virtual void resizeEvent( int width, int height ) {
    cout << "resized " << width << "x" << height << endl;
  }
  virtual void moveEvent( int x, int y ) {
    cout << "moved " << x << ", " << y << endl;
  }
  
  virtual void mouseEnterEvent( int x, int y ) {
    cout << "mouse entered " << x << " " << y << endl;
  }
  virtual void mouseLeaveEvent( int x, int y ) {
    cout << "mouse left " << x << " " << y << endl;
  }
  virtual void mouseMoveEvent( int x, int y ) {
    cout << "mouse moved " << x << " " << y << endl;
  }

  virtual void mousePressEvent( int button, int x, int y ) {
    cout << "button " << button << " pressed " << x << " " << y << endl;
  }
  virtual void mouseReleaseEvent( int button, int x, int y ) {
    cout << "button " << button << " released " << x << " " << y << endl;
  }

  virtual void keyPressEvent( KeySym keysym ) {
    cout << "key " << keysym << " pressed" << endl;
  }
  virtual void keyReleaseEvent( KeySym keysym ) {
    cout << "key " << keysym << " released" << endl;
  }
};


void thread_func() {
  MyXWidget xwid2;
  xwid2.open( 160, 120, "xwidget_test2" );

  sleep( 10 );
  
  xwid2.close();
}

class MyThread: public LoopThread {
private:
  XWidget &xwid;

  int count;
  
public:
  MyThread( XWidget &xwid )
    : xwid( xwid ), count( 0 ) {}

  virtual void eachLoop() {
    int w = xwid.getWidth();
    int h = xwid.getHeight();
    int ds = 3;
    
    unsigned char *buf = new unsigned char[ w * h * ds ];
    
    for ( int i = 0; i < w / 3; ++i ) {
      for ( int j = 0; j < h; ++j ) { 
        for ( int k = 0; k < ds; ++k ) { 
          buf[ ( j * w + i ) * ds + k ] = ( ( ( count + k ) % 3 ) == 0 )? 255: 0;
        }
      }
    }
    for ( int i = w / 3; i < w * 2 / 3; ++i ) {
      for ( int j = 0; j < h; ++j ) {
        for ( int k = 0; k < ds; ++k ) { 
          buf[ ( j * w + i ) * ds + k ] = ( ( ( count + k + 1 ) % 3 ) == 0 )? 255: 0;
        }
      }
    }
    for ( int i = w * 2 / 3; i < w; ++i ) {
      for ( int j = 0; j < h; ++j ) {
        for ( int k = 0; k < ds; ++k ) { 
          buf[ ( j * w + i ) * ds + k ] = ( ( ( count + k + 2 ) % 3 ) == 0 )? 255: 0;
        }
      }
    }
    
    ++count;


    xwid.display( buf );

    //     cout << "resize" << endl;
    //     xwid.resize( xwid.getWidth() + ( ( count % 2 )? -10: 10 ),
    //                  xwid.getHeight() + ( ( count % 2 )? -10: 10 ) );
    //     cout << "move" << endl;
    //     xwid.move( xwid.getWidth() + ( ( count % 2 )? -10: 10 ),
    //                  xwid.getHeight() + ( ( count % 2 )? -10: 10 ) );
    
    delete[] buf;
  }
};



int main() {
  MyXWidget xwid1;
  //  xwid1.setResizable( false );
  xwid1.open( 320, 240, "xwidget_test" );
  xwid1.setResizable( false );

  MyThread th( xwid1 );
  th.setInterval( 300000 );
  th.start();

  
  Thread thread1;
  thread1.start( thread_func );

  thread1.join();
  
  th.stop();

  xwid1.close();
  return 0;
}
