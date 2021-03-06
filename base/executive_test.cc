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
#include "enquery/execution.h"
#include "enquery/executive.h"
#include "enquery/status.h"
#include "enquery/testing.h"

using ::enquery::Execution;
using ::enquery::Executive;
using ::enquery::Future;
using ::enquery::Status;

namespace enquery {

// Execution implementation that always fails. The constructor accepts a
// a pointer-to-bool, which is set to 'false' on construction, and set to
// 'true' on destruction; this can be used by the test to determine the
// correct behavior of Executive's optional mechanism for taking ownership
// of the Execution instance that's passed during creation.
class Task;
class FailingExecution : public Execution {
 public:
  explicit FailingExecution(bool* set_when_destroyed)
      : set_when_destroyed_(set_when_destroyed) {
    *set_when_destroyed_ = false;
  }

  virtual ~FailingExecution() { *set_when_destroyed_ = true; }

  virtual Status Execute(Task*) {  // NOLINT
    return Status::MakeError("test", "failed to execute");
  }

 private:
  bool* set_when_destroyed_;
};

}  // namespace enquery

// Sample function for unit test (simply negates integer that's passed.)
int negate(int x) { return -x; }

void test_default_use() {
  const int input_value = 42;

  // Create an Executive with default options: execute tasks on the
  // current thread.
  Executive* executive = Executive::Create(Executive::DefaultSettings());

  // Create an empty future, to be populated later via Submit()
  Future<int> future_result;
  ASSERT_FALSE(future_result.Valid());

  // Submit a task; in this case, we're making a call to the 'negate'
  // type, passing input_value as the argument; if successful,
  // 'future_result' is populated with a future that will eventually
  // hold the result of the calculation.
  Status status = executive->Submit(negate, input_value, &future_result);

  // We should succeed
  ASSERT_TRUE(status.IsSuccess());

  // We should get the correct answer
  ASSERT_EQUALS(future_result.GetValue(), negate(input_value));

  delete executive;
}

// Test failing execution while simultaneously testing that the executive
// properly cleans up its "execution method" when so configured.
void test_failing_use_with_ownership() {
  const int input_value = 42;

  // First, create an execution method that always fails.
  bool destroyed = false;
  Execution* exm = new enquery::FailingExecution(&destroyed);

  // Create an executive that's configured to take ownership of
  // the execution method that's passed in.
  Executive::Settings settings;
  settings.set_execution(exm);
  settings.set_take_ownership(true);
  Executive* executive = Executive::Create(settings);

  // Create an empty future to hold the result.
  Future<int> future_result;
  ASSERT_FALSE(future_result.Valid());

  // Submit the task for execution, which should fail fail.
  Status status = executive->Submit(negate, input_value, &future_result);
  ASSERT_FALSE(status.IsSuccess());
  ASSERT_FALSE(future_result.Valid());

  // Delete the executive.
  delete executive;

  // Assert that Execution owned by Executive was actually destroyed.
  ASSERT_TRUE(destroyed);
}

// Test that the executive does not delete the "execution method" when
// so configured.
void test_not_taking_ownership() {
  bool destroyed = false;

  // Create a "failing execution" instance
  Execution* exm = new enquery::FailingExecution(&destroyed);

  // Create an executive that uses the failing execution method but does
  // not take ownership of the instance.
  Executive::Settings settings;
  settings.set_execution(exm);
  settings.set_take_ownership(false);
  Executive* executive = Executive::Create(settings);

  // Delete the executive
  delete executive;

  // Assert that the execution instance was not destroyed.
  ASSERT_FALSE(destroyed);

  delete exm;
}

int main(int argc, char* argv[]) {
  test_default_use();
  test_failing_use_with_ownership();
  test_not_taking_ownership();
  return EXIT_SUCCESS;
}
