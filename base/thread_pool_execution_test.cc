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
#include "enquery/thread_pool_execution.h"
#include "enquery/testing.h"

using ::enquery::AtomicIncrement;
using ::enquery::Execution;
using ::enquery::Status;
using ::enquery::Task;
using ::enquery::ThreadPoolExecution;

namespace enquery {}  // namespace enquery

class IncrementingTask : public Task {
 public:
  explicit IncrementingTask(int* counter) : counter_(counter) {}
  virtual ~IncrementingTask() {}
  virtual void Run() { AtomicIncrement(counter_); }

 private:
  int* counter_;
};

Status create_destroy_test(int num_thread) {
  Status status;
  ThreadPoolExecution::Settings settings;
  settings.set_thread_count(num_thread);
  Execution* tpe = ThreadPoolExecution::Create(settings, &status);
  delete tpe;
  return status;
}

Status create_pool_test(int num_thread, int num_tasks) {
  Status status;
  ThreadPoolExecution::Settings settings;
  settings.set_thread_count(num_thread);
  Execution* tpe = ThreadPoolExecution::Create(settings, &status);
  int counter = 0;
  for (int i = 0; i < num_tasks; ++i) {
    Status s2(tpe->Execute(new IncrementingTask(&counter)));
    ASSERT_TRUE(s2.IsSuccess());
  }
  delete tpe;
  ASSERT_EQUALS(counter, num_tasks);
  return status;
}

int main(int argc, char* argv[]) {
  Status status;

  Status t(Status::MakeError("foo", "bar"));
  ASSERT_TRUE(t.IsFailure());

  // Ensure that you can't create a pool with zero threads.
  status = create_destroy_test(0);
  ASSERT_FALSE(status.IsSuccess());

  // Ensure that you can't create a pool with negative number of threads.
  status = create_destroy_test(-1);
  ASSERT_TRUE(status.IsFailure());

  // Mini stress test of creation in which a pool is created with N threads
  // and immediately destroyed without sending any tasks for execution.
  const int kMaxIterations = 10;
  const int kMaxStressThreads = 40;
  for (int i = 0; i < kMaxIterations; ++i) {
    for (int t = 1; t < kMaxStressThreads; ++t) {
      status = create_destroy_test(t);
      ASSERT_TRUE(status.IsSuccess());
    }
  }

  // Create a pool with one thread, one task
  status = create_pool_test(1, 1);
  ASSERT_TRUE(status.IsSuccess());

  // Run a series of tests with a matrix of thread/thread combinations
  const int kMaxThreads = 16;
  const int kMaxTasks = 20000;
  const int kTaskStep = 10000;
  for (int threads = 1; threads <= kMaxThreads; ++threads) {
    for (int tasks = 0; tasks <= kMaxTasks; tasks += kTaskStep) {
      Status status = create_pool_test(threads, tasks);
      ASSERT_TRUE(status.IsSuccess());
    }
  }

  return EXIT_SUCCESS;
}
