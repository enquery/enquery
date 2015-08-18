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
  FailingExecution(bool* set_when_destroyed)
      : set_when_destroyed_(set_when_destroyed) {
    *set_when_destroyed_ = false;
  }

  virtual ~FailingExecution() { *set_when_destroyed_ = true; }

  virtual Status Execute(Task*) {
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

  // Create an Executive with default options; pass NULL to use execute
  // everything on the curren thread. The boolean parameter indicates
  // that Executive should own the 'Execution' object, however when
  // NULL is passed, the current thread executive is used, and the
  // Executive *always* owns that.
  Executive* executive = Executive::Create(NULL, true);

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

void test_failing_use_with_ownership() {
  const int input_value = 42;

  // First, create an execution method that always fails.
  bool destroyed = false;
  Execution* exm = new enquery::FailingExecution(&destroyed);

  // Create an executive that's configured to take ownership of
  // the execution method that's passed in.
  Executive* executive = Executive::Create(exm, true);

  // Create an empty future to hold the result.
  Future<int> future_result;
  ASSERT_FALSE(future_result.Valid());

  // Submit the task for execution, which should fail fail.
  Status status = executive->Submit(negate, input_value, &future_result);
  ASSERT_FALSE(status.IsSuccess());

  // Delete the executive.
  delete executive;

  // Assert that Execution owned by Executive was actually destroyed.
  ASSERT_TRUE(destroyed);
}

int main(int argc, char* argv[]) {
  test_default_use();
  test_failing_use_with_ownership();
  return EXIT_SUCCESS;
}
