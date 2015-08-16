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
#include "enquery/executor_service.h"
#include "enquery/testing.h"

using ::enquery::CurrentThreadExecutionStrategy;
using ::enquery::ExecutorService;
using ::enquery::Future;

int negate(int x) { return -x; }

int main(int argc, char* argv[]) {
  const int input_value = 42;

  ExecutorService<CurrentThreadExecutionStrategy> executor;

  Future<int> future_result = executor.submit<int>(negate, input_value);

  ASSERT_EQUALS(future_result.GetValue(), negate(input_value));

  return EXIT_SUCCESS;
}