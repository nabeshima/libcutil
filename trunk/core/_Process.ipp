/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

extern "C" {
#include <sys/wait.h>
}
#include <cstdlib>
#include <cerrno>
#include <csignal>

namespace cutil {

namespace {

const int READ = 0;
const int WRITE = 1;

inline
void closePipe( int *pp ) throw () {
  ::close( pp[ READ ] );
  ::close( pp[ WRITE ] );
}

}


template< _Process::IOHolderType type >
inline
_Process::IOHolder< type >::IOHolder( _Process &proc )
  : proc( proc ) {}

template< _Process::IOHolderType type >
inline
int _Process::IOHolder< type >::write( const void *data, int size ) {
  switch ( type ) {
  case STDIN:
    return proc.writeIn( data, size );
  case STDOUT:
  case STDERR:
    return 0;
  }
}

template< _Process::IOHolderType type >
inline
int _Process::IOHolder< type >::read( void *data, int size ) {
  switch ( type ) {
  case STDIN:
    return 0;
  case STDOUT:
    return proc.readOut( data, size );
  case STDERR:
    return proc.readErr( data, size );
  }
}

template< _Process::IOHolderType type >
inline
bool _Process::IOHolder< type >::isOpen() {
  return proc.isLiving();
}



inline
_Process::_Process() throw ()
  : pid( -1 ), status( 0 ), 
    pin( -1 ), pout( -1 ), perr( -1 ), 
    pin_holder( *this ),
    pout_holder( *this ),
    perr_holder( *this ),
    pin_stream( pin_holder ),
    pout_stream( pout_holder ),
    perr_stream( perr_holder ) {}

inline
_Process::~_Process() throw () {
  try {
    this->close();
  }
  catch ( ... ) {}
}

inline
bool _Process::open( const std::string &command ) throw () {
  
  if ( isLiving() ) {
    return false;
  }
  
  int
    pipeIn[ 2 ],
    pipeOut[ 2 ],
    pipeErr[ 2 ];

  if ( ::pipe( pipeIn ) < 0 ) {
    return false;
  }
  if ( ::pipe( pipeOut ) < 0 ) {
    closePipe( pipeIn );
    return false;
  }
  if ( ::pipe( pipeErr ) < 0 ) {
    closePipe( pipeIn );
    closePipe( pipeOut );
    return false;
  }
  
  if ( ( pid = ::fork() ) < 0 ) {
    closePipe( pipeIn );
    closePipe( pipeOut );
    closePipe( pipeErr );
    return false;
  }
  else if ( pid == 0 ) { //! child process.
    ::close( pipeIn[ WRITE ] );
    ::close( pipeOut[ READ ] );
    ::close( pipeErr[ READ ] );

    ::dup2( pipeIn[ READ ], 0 );
    ::dup2( pipeOut[ WRITE ], 1 );
    ::dup2( pipeErr[ WRITE ], 2 );
    
    ::close( pipeIn[ READ ] );
    ::close( pipeOut[ WRITE ] );
    ::close( pipeErr[ WRITE ] );
    
    if ( ::execlp( "sh", "sh", "-c", command.c_str(), NULL ) < 0 ) {
      ::exit( errno );
    }
    else {
      ::exit( 0 );
    }
  }
  else {
    ::close( pipeIn[ READ ] );
    ::close( pipeOut[ WRITE ] );
    ::close( pipeErr[ WRITE ] );
    
    pin = pipeIn[ WRITE ];
    pout = pipeOut[ READ ];
    perr = pipeErr[ READ ];
    
    return true;
  }
}

inline
void _Process::pin_close() throw () {
  if ( pin != -1 ) {
    ::close( pin );
    pin = -1;
  }
}

inline
void _Process::pout_close() throw () {
  if ( pout != -1 ) {
    ::close( pout );
    pout = -1;
  }
}

inline
void _Process::perr_close() throw () {
  if ( perr != -1 ) {
    ::close( perr );
    perr = -1;
  }
}

inline
void _Process::close() {
  if ( !isLiving() ) {
    pid = -1;
  }

  if ( isRunning() ) {
    this->kill( SIGKILL, 0 );
    
    //! blocking
    while ( isRunning() ) {
      ::usleep( 10000 );
    }
  }
  
  //! process has already killed here.
  perr_close();
  pout_close();
  pin_close();
  
  pid = -1;
}

inline
void _Process::kill( int signum, int timeout_usec ) {
  if ( !isLiving() ) {
    return;
  }
  
  ::kill( pid, signum );
  
  int time = 0;
  
  while ( isRunning() ) {
    time += 10000;
    if ( timeout_usec < time ) {
      break;
    }
    ::usleep( 10000 );
  }
}


inline
bool _Process::isLiving() throw () {
  if ( pid == -1 ) {
    return false;
  }
    
  if ( ::kill( pid, 0 ) < 0 &&
       errno == ESRCH ) {
    return false;
  }
  
  return true;
}

inline
bool _Process::isRunning() {
  if ( !isLiving() ) {
    return false;
  }
  
  int val = ::waitpid( pid, &status, WNOHANG );

  if ( val == 0 ) {
    return true;
  }
  
  //! zombie
  return false;
}


inline
int _Process::returnCode() throw () {
  return WEXITSTATUS( status );
}

inline
int _Process::writeIn( const void *data, int size ) {
  if ( !isLiving() ) {
    return -1;
  }
  int ret = ::write( pin, data, size );
  if ( ret < 0 ) {
    this->pin_close();
  }
  return ret;
}

inline
int _Process::readOut( void *data, int size ) {
  if ( !isLiving() ) {
    return -1;
  }

  int ret = ::read( pout, data, size );
  if ( ret <= 0 ) {
    this->pout_close();
  }
  return ret;
}

inline
int _Process::readErr( void *data, int size ) {
  if ( !isLiving() ) {
    return -1;
  }
  int ret = ::read( perr, data, size );
  if ( ret <= 0 ) {
    this->perr_close();
  }
  return ret;
}


inline
std::ostream& _Process::stdin() throw () {
  return pin_stream;
}

inline
std::istream& _Process::stdout() throw () {
  return pout_stream;
}

inline
std::istream& _Process::stderr() throw () {
  return perr_stream;
}


}
