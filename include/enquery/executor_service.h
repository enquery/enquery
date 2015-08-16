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
  virtual void Run() = 0;
};

class ExecutionStrategy {
 public:
  virtual void Execute(Task* task) = 0;
};

template <typename ExecStrategy>
class ExecutorService {
 public:
  explicit ExecutorService(ExecStrategy es = ExecStrategy())
      : exec_strategy_(es) {}

  template <typename ReturnType, typename A1>
  Future<ReturnType> submit(const A1& arg) {
    Promise<ReturnType> promise;
    return promise.GetFuture();
  }

 private:
  ExecStrategy exec_strategy_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_EXECUTOR_SERVICE_H_
