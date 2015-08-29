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

#ifndef INCLUDE_ENQUERY_THREAD_H_
#define INCLUDE_ENQUERY_THREAD_H_

#include <assert.h>
#include <pthread.h>
#include <string.h>
#include "enquery/status.h"

namespace enquery {

class Thread {
 public:
  // Thread function type.
  typedef void* (*thread_function_t)(void*);

  // Destructor joins on the thread.
  ~Thread();

  // Create a new thread to run the specified function.
  static Thread* Create(thread_function_t func, void* arg, Status* status);

 private:
  // Construct and set values
  Thread(thread_function_t func, void* arg);

  // Perform (possibly failing) initialization.
  Status Init();

  // Disallow copy / assignment
  Thread(const Thread& no_copy);
  Thread& operator=(const Thread& no_assign);

  bool created_;
  pthread_t thread_;
  thread_function_t func_;
  void* arg_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_THREAD_H_
