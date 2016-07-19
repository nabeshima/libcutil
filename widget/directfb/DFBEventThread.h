/**
 * $Id: DFBEventThread.h 3 2013-05-20 13:07:23Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_DFBEVENTTHREAD_H__
#define __CUTIL_DFBEVENTTHREAD_H__

extern "C" {
#include <directfb.h>
}

#include "Thread.h"
#include <map>

namespace cutil {

class DFBWidget;

class DFBEventThread : public LoopThread {
 private:
  DFBEventThread();
  virtual ~DFBEventThread();

  static DFBEventThread& instance();

  IDirectFB* dfb;
  bool initFlag;
  IDirectFBEventBuffer* buffer;
  timeval event_time;

  std::map<DFBWindowID, DFBWidget*> win_map;

  Mutex event_mutex;

 public:
  static IDirectFB* getDFB();

  static void add(DFBWindowID win, DFBWidget* wid);
  static void erase(DFBWindowID win);

  static void lock();
  static void unlock();
  static cutil::Mutex& getMutex();

  virtual void eachLoop();
  timeval lastEventTime();
};
}  // namespace cutil

#include "DFBEventThread.ipp"

#endif
