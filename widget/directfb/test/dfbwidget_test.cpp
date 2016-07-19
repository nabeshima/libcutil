/**
 * $Id: dfbwidget_test.cpp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <cmath>
#include <iostream>

#include "DFBFont.h"
#include "DFBImage.h"
#include "DFBSurface.h"
#include "DFBWidget.h"

using namespace std;
using namespace cutil;

class MyDFBWidget : public DFBWidget {
 public:
  MyDFBWidget() : DFBWidget() {}
  virtual ~MyDFBWidget() {}

 protected:
  virtual void openEvent(int x, int y, int w, int h) {
    cout << "open " << x << " " << y << ", " << w << " " << h << endl;
  }
  virtual void closeEvent() { cout << "close" << endl; }
  virtual void destroyEvent() { cout << "destroy" << endl; }
  virtual void focusInEvent() { cout << "focused in" << endl; }
  virtual void focusOutEvent() { cout << "focused out" << endl; }
  virtual void resizeEvent(int width, int height) {
    cout << "resized " << width << "x" << height << endl;
  }
  virtual void moveEvent(int x, int y) {
    cout << "moved " << x << ", " << y << endl;
  }

  virtual void mouseEnterEvent(int x, int y) {
    cout << "mouse entered " << x << " " << y << endl;
  }
  virtual void mouseLeaveEvent(int x, int y) {
    cout << "mouse left " << x << " " << y << endl;
  }
  virtual void mouseMoveEvent(int x, int y) {
    cout << "mouse moved " << x << " " << y << endl;
  }

  virtual void mousePressEvent(int button, int x, int y) {
    cout << "button " << button << " pressed " << x << " " << y << endl;
  }
  virtual void mouseReleaseEvent(int button, int x, int y) {
    cout << "button " << button << " released " << x << " " << y << endl;
  }
  virtual void mouseWheelEvent(int step, int x, int y) {
    cout << "wheel moved " << step << " step " << x << " " << y << endl;
  }

  virtual void keyPressEvent(DFBInputDeviceKeySymbol keysym) {
    cout << "key " << keysym << " pressed" << endl;
  }
  virtual void keyReleaseEvent(DFBInputDeviceKeySymbol keysym) {
    cout << "key " << keysym << " released" << endl;
  }
};

int main() {
  {
    DFBLayer layer;

    cout << "LAYER: " << layer.getWidth() << "x" << layer.getHeight() << endl;

    DFBFont goth_font;
    goth_font.open("resources/kochi-gothic.ttf", 24);

    cout << "FONT: size " << goth_font.getHeight() << ", nihoss"
         << goth_font.getWidth("nihoss") << endl;

    DFBImage image;
    image.open("resources/tux.png");

    DFBColor color;
    color.a = 255;
    color.r = 0;
    color.g = 255;
    color.b = 255;

    DFBSurface surface(layer.getWidth(), layer.getHeight());

    surface.drawImage(image);

    surface.drawString("This is test.", goth_font, color, 10, 20);

    color.a = 128;
    color.r = 255;
    color.g = 0;
    color.b = 255;

    surface.drawString("日本語をテストします.", goth_font, color, 10, 60);

    surface.drawString("テストです。", goth_font, color, 10, 100);

    layer.setBackground(surface);

    ::sleep(1);

    //-------------- mini window
    MyDFBWidget wid1;
    wid1.open(layer, 100, 200, 200, 400);

    DFBSurface &dsf = wid1.getSurface();
    wid1.setVisible(true);

    int count = 0;
    while (count < 1000) {
      int code = (count % 255 < 128) ? (count % 255) : (255 - (count % 255));
      code *= 2;

      wid1.setOpacity(code);
      wid1.resize(code, code);
      dsf.drawImage(image);

      dsf.drawString("小さいウィンドウ.", goth_font, color, 10, 30);

      wid1.display();

      count += 30;

      ::usleep(100000);
    }
  }

  return 0;
}
