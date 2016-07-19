/**
 * $Id: DFBFont.ipp 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include "DFBEventThread.h"

namespace cutil {

inline DFBFont::DFBFont() : font(NULL) {}

inline DFBFont::DFBFont(const char *font_filename, int font_height) {
  open(font_filename, font_height);
}

inline DFBFont::~DFBFont() { close(); }

inline bool DFBFont::open(const char *font_filename, int font_height) {
  if (isOpen()) {
    return false;
  }

  DFBFontDescription desc;

  desc.flags = DFDESC_HEIGHT;
  desc.height = font_height;

  IDirectFB *dfb = DFBEventThread::getDFB();

  bool ret = (DFB_OK == dfb->CreateFont(dfb, font_filename, &desc, &font));
  if (!ret) {
    font = NULL;
  } else {
    font->SetEncoding(font, DTEID_UTF8);
  }

  return ret;
}

inline void DFBFont::close() {
  if (isOpen()) {
    font->Release(font);
    font = NULL;
  }
}

inline bool DFBFont::isOpen() { return font != NULL; }

inline int DFBFont::getHeight() {
  if (!isOpen()) {
    return 0;
  }
  int fontheight;
  font->GetHeight(font, &fontheight);
  return fontheight;
}

inline int DFBFont::getWidth(const char *str) {
  if (!isOpen()) {
    return 0;
  }
  int strwidth;
  font->GetStringWidth(font, str, -1, &strwidth);
  return strwidth;
}
}
