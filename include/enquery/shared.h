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

#ifndef INCLUDE_ENQUERY_SHARED_H_
#define INCLUDE_ENQUERY_SHARED_H_

// While shared pointers are not used in the public interfaces in this API,
// they are used in a limited way in its implementation. If portability is
// a goal, using shared pointers raises issues due to the fact that there
// are several versions available, depending on your environment:
//
//   * boost::shared_ptr
//   * tr1::shared_ptr
//   * std::shared_ptr
//
// Large corporations with legacy codebases may even have their own
// implementations, and at the extreme, shared_ptr may not be available at
// all.
//
// The Shared<T>::Ptr construct (defined below) was created to alleviate
// this problem. By default, Shared<T>::Ptr uses a minimal, library-defined
// version (see shared_pointer.h) that is part of the enquery library.
//
// In the future, the build_config script may attempt to detect the shared
// pointer types available on the system. In the mean-time, if there is a
// strong desire to use a proper version, it should be as simple as changing
// this file to include the appropriate implementation and modify the typedef.

#include "enquery/shared_pointer.h"

namespace enquery {
template <typename T>
class Shared {
 public:
  typedef SharedPointer<T> Ptr;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_SHARED_H_
