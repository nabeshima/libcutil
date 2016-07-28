/**
 * $Id: Signal.h 7 2013-09-30 05:32:42Z cota@upard.org $
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef __CUTIL_SIGNAL_H__
#define __CUTIL_SIGNAL_H__

#ifdef __clang__
#include <functional>
#include <type_traits>
#else
#include <tr1/functional>
#include <tr1/type_traits>
#endif

#include <list>

namespace cutil {

#ifdef __clang__
namespace tr1 = std;
#else
namespace tr1 = std::tr1;
#endif

template <typename R, typename Signature>
class SignalBase {
 protected:
  typedef tr1::function<Signature> function_type;
  typedef std::list<tr1::function<Signature> > slot_type;

  std::list<tr1::function<Signature> > slots;

 public:
  std::list<R> results;
};

template <typename Signature>
class SignalBase<void, Signature> {
 protected:
  typedef tr1::function<Signature> function_type;
  typedef std::list<tr1::function<Signature> > slot_type;

  std::list<tr1::function<Signature> > slots;
};

template <class T>
class Signal;

#include "./_signal_def.h"

#define CUTIL_SIGNAL_CLASS(x)                                       \
  template <class R CUTIL_SIGNAL_clP##x>                            \
  class Signal<R(CUTIL_SIGNAL_P##x)>                                \
      : public SignalBase<R, R(CUTIL_SIGNAL_P##x)> {                \
    typedef SignalBase<R, R(CUTIL_SIGNAL_P##x)> BASE;               \
                                                                    \
   public:                                                          \
    void connect(typename BASE::function_type slot) {               \
      BASE::slots.push_back(slot);                                  \
    }                                                               \
                                                                    \
    void operator()(CUTIL_SIGNAL_Pp##x) {                           \
      BASE::results.clear();                                        \
      typename BASE::slot_type::iterator it = BASE::slots.begin(),  \
                                         endIt = BASE::slots.end(); \
      while (it != endIt) {                                         \
        BASE::results.push_back((*it)(CUTIL_SIGNAL_p##x));          \
        ++it;                                                       \
      }                                                             \
    }                                                               \
  };

#define CUTIL_SIGNAL_CLASSN(x)                                            \
  template <CUTIL_SIGNAL_clP##x##n>                                       \
  class Signal<void(CUTIL_SIGNAL_P##x)>                                   \
      : public SignalBase<void, void(CUTIL_SIGNAL_P##x)> {                \
    typedef SignalBase<void, void(CUTIL_SIGNAL_P##x)> BASE;               \
                                                                          \
   public:                                                                \
    void connect(CUTIL_TYPENAME BASE::function_type slot) {               \
      BASE::slots.push_back(slot);                                        \
    }                                                                     \
                                                                          \
    void operator()(CUTIL_SIGNAL_Pp##x) {                                 \
      CUTIL_TYPENAME BASE::slot_type::iterator it = BASE::slots.begin(),  \
                                               endIt = BASE::slots.end(); \
      while (it != endIt) {                                               \
        (*it)(CUTIL_SIGNAL_p##x);                                         \
        ++it;                                                             \
      }                                                                   \
    }                                                                     \
  };

CUTIL_SIGNAL_CLASS(0)
CUTIL_SIGNAL_CLASS(1)
CUTIL_SIGNAL_CLASS(2)
CUTIL_SIGNAL_CLASS(3)
CUTIL_SIGNAL_CLASS(4)
CUTIL_SIGNAL_CLASS(5)
CUTIL_SIGNAL_CLASS(6)
CUTIL_SIGNAL_CLASS(7)
CUTIL_SIGNAL_CLASS(8)
CUTIL_SIGNAL_CLASS(9)

#define CUTIL_TYPENAME
CUTIL_SIGNAL_CLASSN(0)
#undef CUTIL_TYPENAME
#define CUTIL_TYPENAME typename

CUTIL_SIGNAL_CLASSN(1)
CUTIL_SIGNAL_CLASSN(2)
CUTIL_SIGNAL_CLASSN(3)
CUTIL_SIGNAL_CLASSN(4)
CUTIL_SIGNAL_CLASSN(5)
CUTIL_SIGNAL_CLASSN(6)
CUTIL_SIGNAL_CLASSN(7)
CUTIL_SIGNAL_CLASSN(8)
CUTIL_SIGNAL_CLASSN(9)

#undef CUTIL_SIGNAL_CLASS

#include "./_signal_undef.h"
}  // namespace cutil

#endif
