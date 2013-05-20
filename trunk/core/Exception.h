/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_EXCEPTION_H_
#define _CUTIL_EXCEPTION_H_

#include <csetjmp>
#include <csignal>

#include <stdexcept>
#include <vector>
#include <string>
#include <iostream>


namespace cutil {

#define __EXCEPTION_INFO__                      \
  __FILE__, __LINE__, __PRETTY_FUNCTION__


/*!
  Exception Base class.
  It holds source information.
*/
class Exception: public std::runtime_error {
public:
  static const int
    FUNCLIST_NUM = 256,
    FUNCNAME_NUM = 256;
  
private:
  std::string
    orig_file,
    orig_line,
    orig_func;
  
  std::vector< std::string > stacktrace;
  
  void getStackTrace() throw ();
  
public:
  Exception() throw ();
  Exception( const std::string &msg, const std::string &file, int line, const std::string &func ) throw ();
  Exception( const std::runtime_error &e, const std::string &file, int line, const std::string &func ) throw ();
  Exception( const Exception &e ) throw ();
  virtual ~Exception() throw () {};
  
  std::string where() const throw ();
  void printStackTrace( std::ostream &out = std::cout ) const throw ();
};



/*!
  -D_SIGEXCEPTION_ enables the program to handle signals as exception.
  It is really dangerous.
  Programmer never allocate memory between sigtry and sigcatch;
  it can not call destructors.
  This will cause easily memory fault.
*/
#ifndef _SIGEXCEPTION_

#define sigtry try
#define sigcatch catch

#else  // #ifdef _SIGEXCEPTION_

#define sigtry                                     \
try {                                              \
  SignalToException __sig2exc;                     \
  int __retmsg = sigsetjmp( __sig2exc.mark, 1 );   \
  if ( __retmsg != 0 ) {                           \
    __sig2exc.throwException( __retmsg );          \
  }                                                \
  else                                             \
    
#define sigcatch  \
  }               \
  catch           \
    

/*!
  Convert signal to exception.
*/
class SignalToException {

typedef struct sigaction sigaction_t;
  
private:
  sigaction_t oldact[ 4 ];
  
  static void signalHandler( int signum, siginfo_t *info, void *ctx ) throw ();
  
public:
  static sigjmp_buf mark;
  void throwException( int signum ) throw ( Exception );
  
public:
  SignalToException() throw ();
  ~SignalToException() throw ();
};


#endif // #ifndef _SIGEXCEPTION_

}

#include "Exception.ipp"

#endif
