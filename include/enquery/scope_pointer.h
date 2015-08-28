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

#ifndef INCLUDE_ENQUERY_SCOPE_POINTER_H_
#define INCLUDE_ENQUERY_SCOPE_POINTER_H_

#include <stdlib.h>

namespace enquery {

// ScopePointer is a simple RAII helper that takes responsibility for
// deleting an object, a pointer to which is passed at construction time.
// The container provides const and non-const access via Get(), as well
// as a method for explicitly releasing ownership of the raw pointer to
// the caller. The main purpose of this class is to avoid leaking memory
// in factory functions when the factory must create an object first and
// initialize it in a second step that has the potential to fail. The
// pattern of use in this case is:
//
//    ScopePointer<Foo> foo(new Foo());
//    Status status = foo->InitThatMayFail();
//    if (status.IsFailure()) {
//      return NULL;
//    }

template <typename Typ_>
class ScopePointer {
 public:
  // Construct empty
  ScopePointer() : typ_(NULL) {}

  // Construct with pointer-to-object-type
  explicit ScopePointer(Typ_* typ) : typ_(typ) {}
  ~ScopePointer() { delete typ_; }

  // Return pointer (const.)
  const Typ_* Get() const { return typ_; }

  // Return pointer (non-const.)
  Typ_* Get() { return typ_; }

  // Operator overload for -> (const.)
  const Typ_* operator->() const { return typ_; }

  // Operator overload for -> (non-const.)
  Typ_* operator->() { return typ_; }

  // Return pointer, releasing ownership to the caller.
  Typ_* ReleaseOwnership() {
    Typ_* tmp = typ_;
    typ_ = NULL;
    return tmp;
  }

 private:
  explicit ScopePointer(const ScopePointer<Typ_>& no_copy);
  ScopePointer<Typ_>& operator=(const ScopePointer<Typ_>& no_assign);
  Typ_* typ_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_SCOPE_POINTER_H_
