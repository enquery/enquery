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

#ifndef INCLUDE_ENQUERY_EXECUTIVE_H_
#define INCLUDE_ENQUERY_EXECUTIVE_H_

#include <assert.h>
#include "enquery/execution.h"
#include "enquery/futures.h"
#include "enquery/shared.h"
#include "enquery/status.h"
#include "enquery/task.h"
#include "enquery/utility.h"

namespace enquery {

template <typename ReturnType, typename Func, typename A1>
class Task_1 : public Task {
 public:
  Task_1(Promise<ReturnType> promise, Func func, A1 arg1)
      : promise_(promise), func_(func), arg1_(arg1) {}
  virtual ~Task_1() {}
  virtual void Run() { promise_.SetValue(func_(arg1_)); }

 private:
  Promise<ReturnType> promise_;
  Func func_;
  A1 arg1_;
};

class CurrentThreadExecution : public Execution {
 public:
  virtual Status Execute(Task* task) {
    task->Run();
    delete task;
    return Status::OK();
  }
};

class Executive {
 public:
  // The Settings class is used to configure the Executive class; the user
  // may specify options such as the execution method to use and whether
  // the Executive should take ownership of the execution instance.
  class Settings {
   public:
    Settings() : execution_(NULL), take_ownership_(false) {}

    // Set the Execution implementation to use for running tasks.
    Settings& set_execution(Execution* ex) {
      execution_ = ex;
      return *this;
    }

    // Get the Execution implementation to use for running tasks.
    Execution* execution() const { return execution_; }

    // Set whether the Executive instance should own the execution instance.
    // There is a condition when this is ignored; see Executive for info.
    Settings& set_take_ownership(bool take) {
      take_ownership_ = take;
      return *this;
    }

    // Get whether we intend Executive to own the execution instance.
    bool take_ownership() const { return take_ownership_; }

   private:
    Execution* execution_;
    bool take_ownership_;
  };

  // Construct Executive. If caller passes NULL, the default behavior
  // of "current thread" execution is used, and take_ownership is
  // ignored. Otherwise, the caller-provided execution method is used
  // and the instance takes ownership of the pointer at the behest of
  // the caller.
  static Executive* Create(const Settings& settings) {
    return new Executive(settings.execution(), settings.take_ownership());
  }

  // Syntax helper: makes it easy to Create() with default settings
  // without requiring that Create() use a default argument.
  static Executive::Settings DefaultSettings() { return Settings(); }

  ~Executive() {
    if (take_ownership_) {
      delete execution_;
    }
  }

  // Submit a single-argument function call for execution. Returns a
  // Future that may be used by the caller to obtain the return value.
  // Actual executon is delegated to an implementation of the Execution
  // abstract class. If default settings are used to construct the
  // Executive, execution takes place on the current thread, and the
  // call to Submit() will block until execution completes. If an
  // alternative Execution instance is supplied, execution is scheduled
  // to take place and may run on an alternate thread at some point in
  // the future. If Submit() returns a failed Status, the function is
  // guaranteed not to execute.
  template <typename ReturnType, typename Func, typename A1>
  Status Submit(Func func, const A1& arg1, Future<ReturnType>* future) {
    assert(future != NULL);
    Promise<ReturnType> promise;
    Task* task = new Task_1<ReturnType, Func, A1>(promise, func, arg1);
    Status status = execution_->Execute(task);
    if (status.IsFailure()) {
      delete task;
      return status;
    }
    *future = promise.GetFuture();
    return Status::OK();
  }

 private:
  Executive(Execution* exec, bool take_ownership)
      : execution_(exec ? exec : new CurrentThreadExecution()),
        take_ownership_(exec ? take_ownership : true) {}

  Executive(const Executive& no_copy);
  Executive& operator=(const Executive& no_assign);

  Execution* execution_;
  bool take_ownership_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_EXECUTIVE_H_
