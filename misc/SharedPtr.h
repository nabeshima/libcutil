/**
 * $Id$
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

#ifndef CUTIL_SHAREDPTR_H_
#define CUTIL_SHAREDPTR_H_

namespace cutil {

template <class T>
class SharedPtr {
 private:
  int* ref_counter;
  T* t;

  SharedPtr();

 public:
  explicit SharedPtr(T* t);
  SharedPtr(const SharedPtr& s);
  ~SharedPtr();

  SharedPtr& operator=(const SharedPtr& s);

  T* operator->();
  const T* operator->() const;

  T& operator*();
  const T& operator*() const;

  operator bool() const;

  bool operator==(const SharedPtr& p) const;
  bool operator!=(const SharedPtr& p) const;
};

}  // namespace cutil

#include "SharedPtr.ipp"

#endif  // CUTIL_SHAREDPTR_H_
