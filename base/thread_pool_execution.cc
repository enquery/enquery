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
#include <pthread.h>
#include <deque>
#include "enquery/scope_lock.h"
#include "enquery/scope_pointer.h"
#include "enquery/status.h"
#include "enquery/utility.h"

// The ThreadPoolExecution class provides an implementation of Execution
// that schedules Task objects to execute on a pool of threads. Each
// instance owns a single TaskQueue and multiple Worker instances, all
// of which share the TaskQueue. When a task is scheduled for execution,
// it is placed onto the queue and executed by the first available Worker
// thread.
//
// ThreadPoolExecution guarantees that all tasks that have been queued
// will be executed prior to shutdown. This is

namespace {

using ::enquery::Task;

// TaskQueue is a FIFO that's used internally by the thread pool to manage a
// queue of pointers-to-Task to be executed by a pool of workers. It provides
// little functionality over the deque that it wraps, save for the ability
// to optionally delete left-over pointers that remain in the queue at the
// time of destruction. TaskQueue has no provisions for thread safety, by
// design; it is expected that the caller will protect calls to TaskQueue
// via the approptiate synchronization method.

class TaskQueue {
 public:
  // Create a new task queue. If the caller wishes the TaskQueue to take
  // ownership for deleting left over pointers upon destruction, she may opt
  // in by setting take_ownership to 'true'.
  // Otherwise, it is the caller's responsibility to clean up pointers to
  // objects that were placed in, but not removed from the queue.
  explicit TaskQueue(bool take_ownership) : take_ownership_(take_ownership) {}

  ~TaskQueue() { MaybeCleanupItems(); }

  // Add a task pointer to the queue.
  void Add(Task* task) { tasks_.push_front(task); }

  // Remove a task pointer from the queue. Returns NULL if the queue is empty.
  Task* Remove() {
    if (tasks_.size() == 0) {
      return NULL;
    }
    Task* task = tasks_.back();
    tasks_.pop_back();
    return task;
  }

  // Return the number of items currently in queue.
  size_t Size() const { return tasks_.size(); }

 private:
  TaskQueue(const TaskQueue& no_copy);

  TaskQueue& operator=(const TaskQueue& no_assign);

  // If the caller specified to take ownership of the pointers, we delete
  // any objects that are left in the queue upon destruction. Called only
  // from the destructor.
  void MaybeCleanupItems() {
    if (!take_ownership_) {
      return;
    }
    while (tasks_.size() > 0) {
      Task* task = tasks_.back();
      tasks_.pop_back();
      delete task;
    }
  }

  std::deque<Task*> tasks_;
  bool take_ownership_;
};

}  // namespace

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
