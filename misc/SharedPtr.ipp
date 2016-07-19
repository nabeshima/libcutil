/**
 * $Id$
 * Copyright (c) 2016 Cota Nabeshima <cota@upard.org>
 * This file is subject to the MIT license available at,
 * http://opensource.org/licenses/mit-license.php
 */

namespace cutil {

template <class T>
inline SharedPtr<T>::SharedPtr(T* t) : ref_counter(new int), t(t) {
  *ref_counter = 1;
}

template <class T>
inline SharedPtr<T>::SharedPtr(const SharedPtr<T>& s)
    : ref_counter(s.ref_counter), t(s.t) {
  ++(*ref_counter);
}

template <class T>
inline SharedPtr<T>::~SharedPtr() {
  --(*ref_counter);
  if (*ref_counter == 0) {
    delete ref_counter;
    if (t != 0) {
      delete t;
    }
  }
}

template <class T>
inline SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& s) {
  --(*ref_counter);
  if (*ref_counter == 0) {
    delete ref_counter;
    if (t != 0) {
      delete t;
    }
  }
  ref_counter = s.ref_counter;
  t = s.t;
  ++(*ref_counter);

  return *this;
}

template <class T>
inline T* SharedPtr<T>::operator->() {
  return t;
}

template <class T>
inline const T* SharedPtr<T>::operator->() const {
  return t;
}

template <class T>
inline T& SharedPtr<T>::operator*() {
  return *t;
}

template <class T>
inline const T& SharedPtr<T>::operator*() const {
  return *t;
}

template <class T>
inline SharedPtr<T>::operator bool() const {
  return (t != 0);
}

template <class T>
inline bool SharedPtr<T>::operator==(const SharedPtr<T>& p) const {
  return (t == p.t);
}

template <class T>
inline bool SharedPtr<T>::operator!=(const SharedPtr<T>& p) const {
  return (t != p.t);
}

}  // namespace cutil
