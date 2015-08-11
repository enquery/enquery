// Copyright 2015 The Enquery Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License. See the AUTHORS file for names of
// contributors.

#ifndef INCLUDE_ENQUERY_SHARED_POINTER_H_
#define INCLUDE_ENQUERY_SHARED_POINTER_H_

#include <assert.h>
#include <stdlib.h>
#include <algorithm>
#include "enquery/atomic.h"

namespace enquery {
template <typename Typ_>
class Deleter {
 public:
  void operator()(Typ_* typ) { delete typ; }
};

class ReferenceCounted {
 public:
  virtual ~ReferenceCounted() {}
  virtual void Up() = 0;
  virtual void Down() = 0;
};

template <typename Typ_, typename Del_ = Deleter<Typ_> >
class ReferenceCountedDeleter : public ReferenceCounted {
 public:
  explicit ReferenceCountedDeleter(Typ_* ptr) : ref_count_(1), ptr_(ptr) {}

  ReferenceCountedDeleter(Typ_* ptr, Del_ del)
      : ref_count_(1), ptr_(ptr), del_(del) {}

  virtual ~ReferenceCountedDeleter() { del_(ptr_); }

  virtual void Up() { AtomicIncrement(&ref_count_); }

  virtual void Down() {
    if (AtomicDecrement(&ref_count_) == 0) {
      delete this;
    }
  }

 private:
  typedef ReferenceCountedDeleter<Typ_> ThisType;
  explicit ReferenceCountedDeleter(const ThisType& no_copy);
  ThisType& operator=(const ThisType& no_assign);
  int ref_count_;
  Typ_* ptr_;
  Del_ del_;
};

template <typename Typ_>
class SharedPointer {
 public:
  SharedPointer()
      : deleter_(new ReferenceCountedDeleter<Typ_>(NULL)), ptr_(NULL) {}

  explicit SharedPointer(Typ_* ptr)
      : deleter_(new ReferenceCountedDeleter<Typ_>(ptr)), ptr_(ptr) {}

  template <typename Del_>
  SharedPointer(Typ_* ptr, Del_ del)
      : deleter_(new ReferenceCountedDeleter<Typ_, Del_>(ptr, del)),
        ptr_(ptr) {}

  SharedPointer(const SharedPointer<Typ_>& copy)  // NOLINT
      : deleter_(copy.deleter_),
        ptr_(copy.ptr_) {
    deleter_->Up();
  }

  template <typename Other_>
  SharedPointer(const SharedPointer<Other_>& copy)  // NOLINT
      : deleter_(copy.get_deleter()),
        ptr_(copy.get()) {
    deleter_->Up();
  }

  SharedPointer<Typ_>& operator=(const SharedPointer<Typ_>& assign) {
    SharedPointer<Typ_> tmp(assign);
    this->swap(tmp);
    return *this;
  }

  ~SharedPointer() { deleter_->Down(); }

  void swap(SharedPointer<Typ_>& other) {
    std::swap(this->ptr_, other.ptr_);
    std::swap(this->deleter_, other.deleter_);
  }

  Typ_* get() const { return ptr_; }

  ReferenceCounted* get_deleter() const { return deleter_; }

  const Typ_* operator->() const {
    assert(ptr_ != NULL);
    return get();
  }

  Typ_* operator->() {
    assert(ptr_ != NULL);
    return get();
  }

 private:
  ReferenceCounted* deleter_;
  Typ_* ptr_;
};

template <typename Lhs_, typename Rhs_>
inline bool operator==(const SharedPointer<Lhs_>& lhs,
                       const SharedPointer<Rhs_>& rhs) {
  return (lhs.get() == rhs.get());
}

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_SHARED_POINTER_H_
