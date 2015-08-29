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

#ifndef INCLUDE_ENQUERY_THREAD_POOL_EXECUTION_H_
#define INCLUDE_ENQUERY_THREAD_POOL_EXECUTION_H_

#include "enquery/execution.h"
#include "enquery/shared.h"
#include "enquery/status.h"
#include "enquery/task.h"

namespace enquery {

class ThreadPoolExecution : public Execution {
 public:
  // Settings used to control creation of ThreadPoolExcution
  class Settings {
   public:
    // Create with reasonable defaults (thread count = 1)
    Settings() : thread_count_(1) {}

    // Set the number of threads to configure in the pool.
    Settings& set_thread_count(int thread_count) {
      thread_count_ = thread_count;
      return *this;
    }

    // Get the number of threads to configure in the pool.
    int thread_count() const { return thread_count_; }

   private:
    int thread_count_;
  };

  virtual ~ThreadPoolExecution();

  // Create a thread pool with the specified settings.
  // Returns NULL in the event of an error and populates the caller's
  // (optional) Status variable with error information.
  static Execution* Create(const Settings& settings, Status* status);

  // Return an instance of settings that uses reasonable defaults.
  // Presently, this means a pool with a single thread.
  static Settings DefaultSettings();

  // Schedule a task for execution on the pool of threads. If scheduling
  // the task is successful, the function returns a successful status
  // code and guarantees to take responsibility for deleting the Task
  // object. If scheduling fails, the responsibility for deleting the
  // Task falls to the caller.
  Status Execute(Task* task);

  // Shut down the thread pool, waiting for all enqueued tasks to complete
  // prior to return. Note that it is not necessary to call this function,
  // as it will eventually be called during the destruction sequence. The
  // purpose of the function is to grant the caller explicit control over
  // when the pool is actually stopped. It is harmless to call it more
  // than one time, although subsequent invocations have no effect.
  void Shutdown();

 private:
  ThreadPoolExecution(const ThreadPoolExecution& no_copy);
  ThreadPoolExecution& operator=(const ThreadPoolExecution& no_assign);

  class Rep;
  explicit ThreadPoolExecution(Rep* rep);

  Rep* rep_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_THREAD_POOL_EXECUTION_H_
