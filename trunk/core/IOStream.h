/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef _CUTIL_IOSTREAM_H_
#define _CUTIL_IOSTREAM_H_


#include <ios>            //! streamsize
#include <iostream>       //! iostream
#include <streambuf>      //! basic_streambuf


namespace cutil {


template< class T >
class StreamBuf: public std::streambuf {
private:
  T t;
  int buf_size;
  char *buffer;
  
protected:
  //! write n character
  virtual std::streamsize xsputn( const char_type* s, std::streamsize n );
  
  //! read n character
  virtual std::streamsize xsgetn( char_type* s, std::streamsize n );
  
  //! write one character
  virtual std::streambuf::int_type overflow( std::streambuf::int_type c = std::char_traits< char >::eof() );
  
  //! read buffer got empty 
  virtual std::streambuf::int_type underflow();
  
  
public:
  static const std::size_t DEFAULT_BUF_SIZE = 1024;
  
  explicit StreamBuf( T &t, std::size_t buf_size = DEFAULT_BUF_SIZE ) throw ();
  virtual ~StreamBuf() throw (); 
};


template< class T >
class IOStream: public std::iostream {
private:
  T t;
  StreamBuf< T > sbuf;
  
public:
  IOStream( T &t ) throw ();
  virtual ~IOStream() throw ();
};

template< class T >
class OStream: public std::ostream {
private:
  T t;
  StreamBuf< T > sbuf;

public:
  OStream( T &t ) throw ();
  virtual ~OStream() throw ();
};


template< class T >
class IStream: public std::istream {
private:
  T t;
  StreamBuf< T > sbuf;

public:
  IStream( T &t ) throw ();
  virtual ~IStream() throw ();
};

}

#include "IOStream.ipp"

#endif

