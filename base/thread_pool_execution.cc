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
#include <string.h>
#include <deque>
#include <vector>
#include "enquery/scope_lock.h"
#include "enquery/scope_pointer.h"
#include "enquery/status.h"
#include "enquery/thread.h"
#include "enquery/utility.h"

namespace enquery {

class ThreadPoolExecution::Rep : public Execution {
 public:
  Rep() : created_sync_(false), shutting_down_(false) {
    memset(&mutex_, 0, sizeof(mutex_));
    memset(&cond_, 0, sizeof(cond_));
  }

  virtual ~Rep() {
    if (created_sync_) {
      Shutdown();
      pthread_cond_destroy(&cond_);
      pthread_mutex_destroy(&mutex_);
    }
  }

  // Initialize an instance. Only called from the Create() function of
  // ThreadPoolExecution. If this fails, Create() always follows up with
  // a delete.
  Status Init(const ThreadPoolExecution::Settings& settings) {
    const int thread_count = settings.thread_count();
    if (thread_count < 1) {
      return Status::MakeError("ThreadPoolExecution::Rep",
                               "thread count must be positive");
    }

    int status = pthread_mutex_init(&mutex_, NULL);
    if (status != 0) {
      return Status::MakeFromSystemError(status);
    }

    status = pthread_cond_init(&cond_, NULL);
    if (status != 0) {
      pthread_mutex_destroy(&mutex_);
      return Status::MakeFromSystemError(status);
    }

    // Record that we've created the core synchronization primitives used
    // in the instance. This tells the destructor that it's OK to delete
    // them. This solves a conundrum created by the fact that we're doing
    // "two phase" construction.
    created_sync_ = true;

    for (int i = 0; i < thread_count; ++i) {
      Status status;
      Thread* thread = Thread::Create(ThreadFunction, this, &status);
      if (!thread) {
        return status;
      }
      threads_.push_back(thread);
    }

    return Status::OK();
  }

  Status Execute(Task* task) {
    // Ensure the task is valid and report error otherwise.
    assert(task != NULL);
    if (task == NULL) {
      return Status::MakeError("ThreadPoolExecution::Rep", "task was null");
    }

    // We must lock the mutex to safely determine whether we are still
    // accepting task submissions. If we're being shut down, we will
    // not accept any more tasks.
    pthread_mutex_lock(&mutex_);

    if (shutting_down_) {
      pthread_mutex_unlock(&mutex_);
      return Status::MakeError("ThreadPoolExecution::Rep", "shutting down");
    }

    // Enqueue the task, signal a waiting thread
    tasks_.push_front(task);
    pthread_cond_signal(&cond_);  // TODO(tdial): pthread_cond_broadcast() ?
    pthread_mutex_unlock(&mutex_);

    return Status::OK();
  }

  void Shutdown() {
    // First, check to see if we are already shutting down. This must
    // be checked within the mutex. If we *are* in that process, then
    // simply release the mutex and return.
    pthread_mutex_lock(&mutex_);
    if (shutting_down_) {
      pthread_mutex_unlock(&mutex_);
      return;
    }

    // While under protection of the mutex, change state to shutting down.
    shutting_down_ = true;

    // Unlock the mutex
    pthread_mutex_unlock(&mutex_);

    // Queue up NULL tasks for all threads.
    const size_t thread_count = threads_.size();
    for (size_t i = 0; i < thread_count; ++i) {
      pthread_mutex_lock(&mutex_);
      tasks_.push_front(NULL);
      pthread_cond_signal(&cond_);
      pthread_mutex_unlock(&mutex_);
    }

    // Loop through threads and delete them. This is safe because the
    // destructor joins on the thread prior to exit. This means that
    // we will not risk deleting resources that are being used by
    // another thread.
    for (size_t i = 0; i < thread_count; ++i) {
      Thread* thread = threads_[i];
      delete thread;
    }

    // Erase vector that records threads.
    threads_.clear();
    assert(tasks_.size() == 0);
  }

 private:
  // Retrieve a task from the shared task queue.
  Task* GetNextTask() {
    Task* task = NULL;
    pthread_mutex_lock(&mutex_);
    while (tasks_.size() == 0) {
      pthread_cond_wait(&cond_, &mutex_);
    }
    task = tasks_.back();
    tasks_.pop_back();
    pthread_mutex_unlock(&mutex_);
    return task;
  }

  // Run in every thread; retrieve tasks forever, quitting only when a
  // NULL task is encountered. NULL tasks are not allowed to be entered
  // by clients, but are used internally as a shutdown signal. Because
  // tasks are entered in FIFO fashion, this ensures that all tasks in
  // the queue are processed prior to shutdown.
  void* WorkerLoop() {
    for (;;) {
      Task* task = GetNextTask();
      if (!task) {
        break;
      }
      task->Run();
      delete task;
    }
    return NULL;
  }

  // Worker threads run this function. At the time of thread creation, the
  // 'this' pointer of the controlling Rep instance is passed as the thread
  // argument. Here, we cast it back so that we may run the WorkerLoop()
  // member function.
  static void* ThreadFunction(void* arg) {
    Rep* execution = reinterpret_cast<Rep*>(arg);
    return execution->WorkerLoop();
  }

  Rep(const Rep& no_copy);
  Rep& operator=(const Rep& no_assign);
  pthread_mutex_t mutex_;
  pthread_cond_t cond_;
  std::vector<Thread*> threads_;
  std::deque<Task*> tasks_;
  bool created_sync_;
  bool shutting_down_;
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
