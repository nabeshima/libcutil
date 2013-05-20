/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_PROCESS_H_
#define _CUTIL_PROCESS_H_

#include <string>

#include "ProcessException.h"
#include "_Process.h"

namespace cutil {


/*!
  Process class.
  A wrapper of fork, exec and pipe.
*/
class Process : public _Process {
public:
  bool open( const std::string &command )
    throw ( ProcessException );

  virtual void close() 
    throw ( ProcessException );

  virtual void kill( int signum, int timeout_usec )
    throw ( ProcessException, ProcessTimeoutException );

  virtual bool isRunning() 
    throw ( ProcessException );
};


}


#include "Process.ipp"

#endif
