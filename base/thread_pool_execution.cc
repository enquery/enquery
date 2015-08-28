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

#include "enquery/thread_pool_execution.h"
#include <assert.h>
#include "enquery/scope_pointer.h"
#include "enquery/utility.h"

namespace enquery {

class ThreadPoolExecution::Rep : public Execution {
 public:
  Rep() {}

  virtual ~Rep() {}

  Status Init(const ThreadPoolExecution::Settings& settings) {
    // TODO(tdial): Implement
    return Status::MakeError("ThreadPoolExecution::Rep::Init",
                             "not implemented");
  }

  Status Execute(Task* task) {
    // TODO(tdial): Implement
    return Status::MakeError("ThreadPoolExecution::Rep::Execute",
                             "not implemented");
  }

  void Shutdown() {
    // TODO(tdial): Implement
  }

 private:
  Rep(const Rep& no_copy);
  Rep& operator=(const Rep& no_assign);
};

ThreadPoolExecution::ThreadPoolExecution(Rep* rep) : rep_(rep) {
  assert(rep != NULL);
}

ThreadPoolExecution::~ThreadPoolExecution() { delete rep_; }

Execution* ThreadPoolExecution::Create(const Settings& settings,
                                       Status* status_out) {
  ScopePointer<Rep> rep(new Rep());
  Status status = rep->Init(settings);
  if (status.IsFailure()) {
    MaybeAssign(status_out, status);
    return NULL;
  }

  ThreadPoolExecution* execution = new ThreadPoolExecution(rep.Get());
  rep.ReleaseOwnership();

  return execution;
}

ThreadPoolExecution::Settings ThreadPoolExecution::DefaultSettings() {
  return Settings();
}

Status ThreadPoolExecution::Execute(Task* task) { return rep_->Execute(task); }

void ThreadPoolExecution::Shutdown() { rep_->Shutdown(); }

}  // namespace enquery
