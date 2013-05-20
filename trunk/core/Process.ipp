/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#include <sstream>

namespace cutil {

inline
bool Process::open( const std::string &command ) 
  throw ( ProcessException ) {
  
  if ( isLiving() ) {
    return false;
  }
  
  int
    pipeIn[ 2 ],
    pipeOut[ 2 ],
    pipeErr[ 2 ];

  if ( ::pipe( pipeIn ) < 0 ) {
    throw ProcessException( "Fail to create pipeIn", __EXCEPTION_INFO__ );
  }
  if ( ::pipe( pipeOut ) < 0 ) {
    closePipe( pipeIn );
    throw ProcessException( "Fail to create pipeOut", __EXCEPTION_INFO__ );
  }
  if ( ::pipe( pipeErr ) < 0 ) {
    closePipe( pipeIn );
    closePipe( pipeOut );
    throw ProcessException( "Fail to create pipeErr", __EXCEPTION_INFO__ );
  }
  if ( ( pid = ::fork() ) < 0 ) {
    closePipe( pipeIn );
    closePipe( pipeOut );
    closePipe( pipeErr );
    throw ProcessException( "Fail to fork()", __EXCEPTION_INFO__ );
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
    ::exit( 0 );
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
void Process::close() 
  throw ( ProcessException ) {
  _Process::close();
}

inline
void Process::kill( int signum, int timeout_usec )
  throw ( ProcessException, ProcessTimeoutException ) {

  if ( !isLiving() ) {
    return;
  }
  
  ::kill( pid, signum );
  
  int time = 0;
  
  while ( isRunning() ) {
    time += 10000;
    
    if ( timeout_usec < time ) {
      //! timeout exception.
      std::stringstream sstr;
      sstr << "waitpid( " << pid << " ) timeout";
      throw ProcessTimeoutException( sstr.str(), __EXCEPTION_INFO__ );
    }
    ::usleep( 10000 );
  }
}

inline
bool Process::isRunning() 
  throw ( ProcessException ) {

  if ( !isLiving() ) {
    return false;
  }
  
  int val = ::waitpid( pid, &status, WNOHANG );

  if ( val == 0 ) {
    return true;
  }
  else if ( val < 0 ) {
    //! exception
    std::stringstream sstr;
    sstr << "Fail to waitpid( " << pid << " ) ";
    throw ProcessException( sstr.str(), __EXCEPTION_INFO__ );
  }
  
  //! already finished zombie
  return false;
}

}
