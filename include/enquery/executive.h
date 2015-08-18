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

#include "enquery/futures.h"
#include "enquery/shared.h"
#include "enquery/status.h"
#include "enquery/utility.h"

namespace enquery {

class Task {
 public:
  virtual ~Task() {}
  virtual void Run() = 0;
};

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

class Execution {
 public:
  virtual ~Execution() {}
  virtual void Execute(Task* task) = 0;
};

class CurrentThreadExecution : public Execution {
 public:
  virtual void Execute(Task* task) {
    task->Run();
    delete task;
  }
};

class Executive {
 public:
  // Construct Executive. If caller passes NULL, the default behavior
  // of "current thread" execution is used, and take_ownership is
  // ignored. Otherwise, the caller-provided execution method is used
  // and the instance takes ownership of the pointer at the behest of
  // the caller.
  static Executive* Create(Execution* exec, bool take_ownership) {
    return new Executive(exec, take_ownership);
  }

  ~Executive() {
    if (take_ownership_) {
      delete execution_;
    }
  }

  template <typename ReturnType, typename Func, typename A1>
  Future<ReturnType> Submit(Func func, const A1& arg1) {
    Promise<ReturnType> promise;
    Task* task = new Task_1<ReturnType, Func, A1>(promise, func, arg1);
    execution_->Execute(task);
    return promise.GetFuture();
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
