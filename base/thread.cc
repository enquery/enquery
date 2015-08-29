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

#include "enquery/thread.h"
#include <assert.h>
#include <pthread.h>
#include <string.h>
#include "enquery/scope_pointer.h"
#include "enquery/status.h"
#include "enquery/utility.h"

namespace enquery {

Thread::~Thread() {
  if (!created_) {
    return;
  }
  pthread_join(thread_, NULL);
}

Thread* Thread::Create(thread_function_t func, void* arg, Status* status) {
  assert(func);
  if (!func) {
    return NULL;
  }

  ScopePointer<Thread> new_thread(new Thread(func, arg));
  Status s = new_thread->Init();
  if (s.IsFailure()) {
    MaybeAssign(status, s);
    return NULL;
  }

  return new_thread.ReleaseOwnership();
}

Thread::Thread(thread_function_t func, void* arg)
    : created_(false), func_(func), arg_(arg) {
  memset(&thread_, 0, sizeof(thread_));
}

Status Thread::Init() {
  int status = pthread_create(&thread_, NULL, func_, arg_);
  if (status != 0) {
    return Status::MakeFromSystemError(status);
  }
  created_ = true;
  return Status::OK();
}

}  // namespace enquery
