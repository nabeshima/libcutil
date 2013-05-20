/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */


namespace cutil {


template< class T >
inline
StreamBuf< T >::StreamBuf( T &t, std::size_t buf_size ) throw ()
  : t( t ), buf_size( buf_size ) {
  buffer = new char[ buf_size ];
}

template< class T >
inline
StreamBuf< T >::~StreamBuf() throw () {
  delete[] buffer;
}

template< class T >
inline
std::streamsize StreamBuf< T >::xsputn( const char_type* s, std::streamsize n ) {
  if ( !t.isOpen() ) {
    return 0;
  }

  int res = t.write( s, (int)n );
  return ( res == -1 )? 0 : res;
}

template< class T >
inline
std::streamsize StreamBuf< T >::xsgetn( char_type* s, std::streamsize n ) {
  if ( !t.isOpen() ) {
    return 0;
  }
  
  int res = t.read( s, (int)n );
  return ( res == -1 )? 0 : res;
}

template< class T >
inline
std::streambuf::int_type StreamBuf< T >::overflow( std::streambuf::int_type c ) {
  if ( !std::streambuf::traits_type::eq_int_type( c, std::streambuf::traits_type::eof() ) ) {
    char cc= (char)c;
    if ( std::streambuf::traits_type::eq_int_type( xsputn( &cc, 1 ), 0 ) ) {
      return std::streambuf::traits_type::eof();
    }
  }
  return 'a';
}

template< class T >
inline
std::streambuf::int_type StreamBuf< T >::underflow() {
  if ( !t.isOpen() ) {
    return std::streambuf::traits_type::eof();
  }
  
  if ( !gptr() || gptr() >= egptr() ) {
    if ( gptr() && gptr() > eback() ) {
      buffer[ 0 ]= *( gptr() - 1 );
    }
    else {
      buffer[ 0 ]= '\0';
    }

    std::streamsize size = xsgetn( &buffer[ 1 ], (int)( buf_size - 1 ) );

    if ( size <= 0 ) {
      return std::streambuf::traits_type::eof();
    }
    else {
      setg( &buffer[ 0 ], &buffer[ 1 ], &buffer[ size + 1 ] );
    }
  }
  return std::streambuf::traits_type::to_int_type( *gptr() );
}


template< class T >
inline
IOStream< T >::IOStream( T &t ) throw () 
  : std::iostream( 0 ), t( t ), sbuf( t ) {
  rdbuf( &sbuf );

//   if( !t.isOpen() ) {
//     setstate( std::ios::badbit );
//   }
}

template< class T >
inline
IOStream< T >::~IOStream() throw () {
  rdbuf( 0 );
}

template< class T >
inline
OStream< T >::OStream( T &t ) throw () 
  : std::ostream( 0 ), t( t ), sbuf( t ) {
  rdbuf( &sbuf );

//   if( !t.isOpen() ) {
//     setstate( std::ios::badbit );
//   }
}

template< class T >
inline
OStream< T >::~OStream() throw () {
  rdbuf( 0 );
}

template< class T >
inline
IStream< T >::IStream( T &t ) throw () 
  : std::istream( 0 ), t( t ), sbuf( t ) {
  rdbuf( &sbuf );

//   if( !t.isOpen() ) {
//     setstate( std::ios::badbit );
//   }
}

template< class T >
inline
IStream< T >::~IStream() throw () {
  rdbuf( 0 );
}

}
