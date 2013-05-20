/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL__PROCESS_H_
#define _CUTIL__PROCESS_H_


#include <cstdio>

#include <string>

#include "IOStream.h"


namespace cutil {


/*!
  Process class.
  A wrapper of fork, exec and pipe.
*/

class _Process {
protected:
  pid_t pid;
  int status;
  int
    pin,
    pout,
    perr;
  
private:
  //! Holder for IOStream
  enum IOHolderType { STDIN, STDOUT, STDERR };
  
  template< IOHolderType type >
  class IOHolder {
  private:
    _Process &proc;
    
  public:
    IOHolder( _Process &proc );
    int write( const void *data, int size );
    int read( void *data, int size );
    bool isOpen();
  };
  
  IOHolder< STDIN > pin_holder;
  IOHolder< STDOUT > pout_holder;
  IOHolder< STDERR > perr_holder;
  
  OStream< IOHolder< STDIN > > pin_stream;
  IStream< IOHolder< STDOUT > > pout_stream;
  IStream< IOHolder< STDERR > > perr_stream;

protected:  
  void pin_close() throw ();
  void pout_close() throw ();
  void perr_close() throw ();
  
public:
  _Process() throw ();
  virtual ~_Process() throw ();
  
  bool open( const std::string &command ) throw ();
  virtual void close();
  
  virtual void kill( int signum, int timeout_usec );
  bool isLiving() throw ();
  virtual bool isRunning();
  
  int returnCode() throw ();
  
  int writeIn( const void *data, int size );   //! return wrote byte
  int readOut( void *data, int size );         //! return read byte
  int readErr( void *data, int size );         //! return read byte  
  
  std::ostream& stdin() throw ();  
  std::istream& stdout() throw ();
  std::istream& stderr() throw ();
};


}

#include "_Process.ipp"

#endif
