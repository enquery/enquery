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

#ifndef INCLUDE_ENQUERY_EXECUTOR_SERVICE_H_
#define INCLUDE_ENQUERY_EXECUTOR_SERVICE_H_

#include "enquery/futures.h"

namespace enquery {

class Task {
 public:
  virtual ~Task() {}
  virtual void Invoke() = 0;
};

template <typename ReturnType, typename Func, typename A1>
class Task_1 : public Task {
 public:
  Task_1(Promise<ReturnType> promise, Func func, A1 arg1)
      : promise_(promise), func_(func), arg1_(arg1) {}
  virtual void Invoke() { promise_.SetValue(func_(arg1_)); }

 private:
  Promise<ReturnType> promise_;
  Func func_;
  A1 arg1_;
};

class ExecutionStrategy {
 public:
  virtual ~ExecutionStrategy() {}
  virtual void Execute(Task* task) = 0;
};

class CurrentThreadExecutionStrategy : public ExecutionStrategy {
 public:
  virtual void Execute(Task* task) {
    task->Invoke();
    delete task;
  }
};

template <typename ExecStrategy>
class ExecutorService {
 public:
  explicit ExecutorService(ExecStrategy es = ExecStrategy())
      : exec_strategy_(es) {}

  template <typename ReturnType, typename Func, typename A1>
  Future<ReturnType> submit(Func func, const A1& arg1) {
    Promise<ReturnType> promise;
    Task* task = new Task_1<ReturnType, Func, A1>(promise, func, arg1);
    exec_strategy_.Execute(task);
    return promise.GetFuture();
  }

 private:
  ExecStrategy exec_strategy_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_EXECUTOR_SERVICE_H_
