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

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "enquery/atomic.h"
#include "enquery/testing.h"

using ::enquery::AtomicIncrement;
using ::enquery::AtomicDecrement;

namespace {
const int kNumIterations = 1000000;
const int kNumThreads = 32;

}  // namespace

void* thread_func(void* arg) {
  int* ret = reinterpret_cast<int*>(arg);
  for (int i = 0; i < kNumIterations; ++i) {
    AtomicIncrement(ret);
    AtomicDecrement(ret);
  }
  return NULL;
}

int main(int argc, char* argv[]) {
  int value = 0;

  pthread_t ids[kNumThreads] = {0};

  for (int i = 0; i < kNumThreads; ++i) {
    pthread_create(ids + i, NULL, thread_func, reinterpret_cast<void*>(&value));
  }

  for (int i = 0; i < kNumThreads; ++i) {
    void* tmp = 0;
    pthread_join(ids[i], &tmp);
    (void)tmp;
  }

  ASSERT_EQUALS(value, 0);

  return EXIT_SUCCESS;
}
