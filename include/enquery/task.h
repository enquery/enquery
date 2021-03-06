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

#ifndef INCLUDE_ENQUERY_TASK_H_
#define INCLUDE_ENQUERY_TASK_H_

namespace enquery {

// 'Task' is as an abstract interface that represents a single, runnable
// task or function. Calls to the Run() method cause the subclass'
// implementation to run immediately on the current thread of execution,
// blocking until complete. This interface is exposed publicly to
// facilitate unit testing. At the present time, users of enquery should
// not be sublcassing Task.
class Task {
 public:
  virtual ~Task() {}

  // Execute on the current thread, blocking until complete.
  virtual void Run() = 0;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_TASK_H_
