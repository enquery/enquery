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

#ifndef INCLUDE_ENQUERY_ATOMIC_H_
#define INCLUDE_ENQUERY_ATOMIC_H_

namespace enquery {

// Increment atomically, returning the incremented value.
inline int AtomicIncrement(int* volatile addend) {
  return __sync_add_and_fetch(addend, 1);
}

// Decrement atomically, returning the decremented value.
inline int AtomicDecrement(int* volatile addend) {
  return __sync_sub_and_fetch(addend, 1);
}

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_ATOMIC_H_
