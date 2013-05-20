/**
 * $Id$
 * Copyright (c) 2013 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_SIGNAL_H__
#define __CUTIL_SIGNAL_H__

#include <list>

#include <tr1/functional>
#include <tr1/type_traits>


namespace cutil {

template< typename R, typename Signature >
class SignalBase {
protected:
  typedef std::tr1::function< Signature > function_type;
  typedef std::list< std::tr1::function< Signature > > slot_type;

  std::list< std::tr1::function< Signature > > slots;
  
public:  
  std::list< R > results;
};


template< class T >
class Signal;


#include "_signal_def.h"

#define CUTIL_SIGNAL_CLASS( x )                                         \
  template< class R  CUTIL_SIGNAL_clP##x >                              \
  class Signal< R ( CUTIL_SIGNAL_P##x ) >: public SignalBase< R, R ( CUTIL_SIGNAL_P##x ) > { \
    typedef SignalBase< R, R ( CUTIL_SIGNAL_P##x ) > BASE;              \
                                                                        \
  public:                                                               \
  void connect( typename BASE::function_type slot ) throw () {          \
    BASE::slots.push_back( slot );                                      \
  }                                                                     \
                                                                        \
  void operator()( CUTIL_SIGNAL_Pp##x ) {                               \
    BASE::results.clear();                                              \
    typename BASE::slot_type::iterator                                  \
      it = BASE::slots.begin(),                                         \
      endIt = BASE::slots.end();                                        \
    while ( it != endIt ) {                                             \
      BASE::results.push_back( (*it)( CUTIL_SIGNAL_p##x ) );            \
      ++it;                                                             \
    }                                                                   \
  }                                                                     \
  };                                                                    \


CUTIL_SIGNAL_CLASS( 0 )
CUTIL_SIGNAL_CLASS( 1 )
CUTIL_SIGNAL_CLASS( 2 )
CUTIL_SIGNAL_CLASS( 3 )
CUTIL_SIGNAL_CLASS( 4 )
CUTIL_SIGNAL_CLASS( 5 )
CUTIL_SIGNAL_CLASS( 6 )
CUTIL_SIGNAL_CLASS( 7 )
CUTIL_SIGNAL_CLASS( 8 )
CUTIL_SIGNAL_CLASS( 9 )

#undef CUTIL_SIGNAL_CLASS

#include "_signal_undef.h"

}

#endif
