/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_READLINE_H__
#define __CUTIL_READLINE_H__

extern "C" {
#include <termios.h>
}

#include <fstream>
#include <vector>
#include <string>

#include "ReadlineException.h"

namespace cutil {


class Readline {
private:
  bool is_open;
  termios pre_term_attr;

  int winHeight;
  std::string prompt;
  int prompt_offset;
  
  std::ofstream history_outf;

  std::vector< std::string > command;
  std::vector< std::string > history;
 
  bool _open()
    throw ( ReadlineException );
  
  void _close() 
    throw ( ReadlineException );
  
  bool _isOpen() const throw ();
  
  char _read( int timeout_usec )
    throw ( ReadlineException, ReadlineTimeoutException );  

  void completion( std::string &buf, size_t &cursor_pos ) throw ();
  
  void historyNext( std::string &buf, size_t &cursor_pos, size_t &history_i ) throw ();
  void historyPrevious( std::string &buf, size_t &cursor_pos, size_t &history_i ) throw ();

  void moveBack( size_t &cursor_pos ) throw ();
  void moveFront( const std::string &buf, size_t &cursor_pos ) throw ();
  void moveBackward( size_t &cursor_pos ) throw ();
  void moveForward( const std::string &buf, size_t &cursor_pos ) throw ();


  void removeChar( std::string &buf, size_t &cursor_pos ) throw ();
  void removeLine( std::string &buf, const size_t &cursor_pos ) throw ();

  std::string _get( int timeout_usec )
    throw ( ReadlineException, ReadlineTimeoutException );  
 
  
  Readline() throw ();
  ~Readline() throw ();

  static Readline& instance() throw ();
  
  
public:
  static void setPrompt( const std::string &prompt ) throw ();
  static bool setHistoryFile( const std::string &history_file );
  static void clearHistory() throw ();  
  
  static void addCommand( const std::string &command ) throw ();
  static void clearCommand() throw ();
  
  static std::string get( int timeout_usec = -1 )
    throw ( ReadlineException, ReadlineTimeoutException );
  
};

}

#include "Readline.ipp"

#endif

