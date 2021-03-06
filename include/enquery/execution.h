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

#ifndef INCLUDE_ENQUERY_EXECUTION_H_
#define INCLUDE_ENQUERY_EXECUTION_H_

#include "enquery/status.h"

namespace enquery {

class Task;

// This abstract class provides an interface for scheduling task execution.
// Subclasses implement particular execution strategies, such as "execution
// on the current thread" or "execution on a pool of threads." A successful
// call to the Execute() method guarantees that the Task will be Run()
// once and subsequently deleted. If the call to Execute() fails, it is up
// to the caller to delete the Task object.
class Execution {
 public:
  virtual ~Execution() {}
  virtual Status Execute(Task* task) = 0;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_EXECUTION_H_
