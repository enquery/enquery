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

#ifndef INCLUDE_ENQUERY_SCOPE_LOCK_H_
#define INCLUDE_ENQUERY_SCOPE_LOCK_H_

#include <pthread.h>

// TODO(tdial): Transition to use a portable concept (e.g. std::lock_guard)

namespace enquery {

// Acquire and hold a pthread_mutex for the lifetime of the object.
class ScopeLock {
 public:
  explicit ScopeLock(pthread_mutex_t* mutex);
  ~ScopeLock();

 private:
  ScopeLock(const ScopeLock& no_copy);
  ScopeLock& operator=(const ScopeLock& no_assign);

  pthread_mutex_t* mutex_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_SCOPE_LOCK_H_
