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

#ifndef INCLUDE_ENQUERY_FUTURES_H_
#define INCLUDE_ENQUERY_FUTURES_H_

#include <inttypes.h>
#include <pthread.h>
#include <algorithm>
#include <deque>

namespace enquery {

// An abstract interface for callbacks.
class Callback {
 public:
  virtual ~Callback() {}
  virtual void Execute() = 0;
};

// Container used by Future to store callbacks to be invoked when populated.
class CallbackQueue {
 public:
  CallbackQueue() { }

  void Add(Callback* callback) { callbacks_.push_front(callback); }

  ~CallbackQueue() {
    Dispatch(false);
  }

  void Execute() { Dispatch(true); }

  void swap(CallbackQueue& other) {
    std::swap(callbacks_, other.callbacks_);
  }

 private:
  CallbackQueue(const CallbackQueue& no_copy);
  CallbackQueue& operator=(const CallbackQueue& no_assign);

  void Dispatch(bool execute) {
    while (callbacks_.size()) {
      Callback* cb = callbacks_.back();
      callbacks_.pop_back();
      if (execute) {
        cb->Execute();
      }
      delete cb;
    }
  }

  std::deque<Callback*> callbacks_;
};

template <typename F>
class Callback_0 : public Callback {
 public:
  explicit Callback_0(F f) : f_(f) {}
  virtual ~Callback_0() {}
  virtual void Execute() { f_(); }

 private:
  F f_;
};

template <typename F, typename A1>
class Callback_1 : public Callback {
 public:
  Callback_1(F f, A1 a1) : f_(f), a1_(a1) {}
  virtual ~Callback_1() {}
  virtual void Execute() { f_(a1_); }

 private:
  F f_;
  A1 a1_;
};

template <typename F, typename A1, typename A2>
class Callback_2 : public Callback {
 public:
  Callback_2(F f, A1 a1, A2 a2) : f_(f), a1_(a1), a2_(a2) {}
  virtual ~Callback_2() {}
  virtual void Execute() { f_(a1_, a2_); }

 private:
  F f_;
  A1 a1_;
  A2 a2_;
};

// Shared representation used by Promise / Future to synchronize on value.
template <typename T>
class SharedValue {
  template <typename U>
  friend class Promise;

  template <typename U>
  friend class Future;

  SharedValue() : ready_(false), refcount_(1), value_(T()) {
    pthread_mutex_init(&mutex_, NULL);
    pthread_cond_init(&cond_, NULL);
  }

  void Up() {
    pthread_mutex_lock(&mutex_);
    ++refcount_;
    pthread_mutex_unlock(&mutex_);
  }

  void Down() {
    pthread_mutex_lock(&mutex_);
    int64_t newrefcount = --refcount_;
    pthread_mutex_unlock(&mutex_);
    if (newrefcount == 0) {
      delete this;
    }
  }

  void Set(const T& value) {
    pthread_mutex_lock(&mutex_);
    value_ = value;
    ready_ = true;

    // Swap notification callbacks out to a local copy, to be executed
    // outside the mutex. This makes it possible for a callback to call
    // Get() on a future without causing deadlock.
    CallbackQueue calls;
    callbacks_.swap(calls);

    pthread_cond_broadcast(&cond_);
    pthread_mutex_unlock(&mutex_);
    calls.Execute();
  }

  T Get() {
    pthread_mutex_lock(&mutex_);
    while (!ready_) {
      pthread_cond_wait(&cond_, &mutex_);
    }
    pthread_mutex_unlock(&mutex_);
    return value_;
  }

  void Notify(Callback* callback) {
    // If the the SharedValue has already been set, execute the callback
    // immediately on the current thread. Otherwise, queue the callback
    // for later execution, which occurs on the thread that calls Set().
    pthread_mutex_lock(&mutex_);
    Callback* tmp = NULL;
    if (ready_) {
      tmp = callback;
    } else {
      callbacks_.Add(callback);
    }
    pthread_mutex_unlock(&mutex_);
    if (tmp) {
      tmp->Execute();
      delete tmp;
    }
  }

  ~SharedValue() {
    pthread_cond_destroy(&cond_);
    pthread_mutex_destroy(&mutex_);
  }

  pthread_mutex_t mutex_;
  pthread_cond_t cond_;
  bool ready_;
  int64_t refcount_;
  T value_;
  CallbackQueue callbacks_;
};

// Future represents an "eventual value" that will be set by a Promise holder.
template <typename T>
class Future {
 public:
  Future(const Future<T>& copy) : value_(copy.value_) {  // NOLINT
    value_->Up();
  }

  Future<T>& operator=(const Future<T>& assign) {
    Future<T> tmp(assign);
    this->swap(tmp);
    return *this;
  }

  ~Future() { value_->Down(); }

  T GetValue() { return value_->Get(); }

  template <typename F>
  void Notify(F functor) {
    value_->Notify(new Callback_0<F>(functor));
  }

  template <typename F, typename A1>
  void Notify(F functor, A1 arg1) {
    value_->Notify(new Callback_1<F, A1>(functor, arg1));
  }

  template <typename F, typename A1, typename A2>
  void Notify(F functor, A1 arg1, A2 arg2) {
    value_->Notify(new Callback_2<F, A1, A2>(functor, arg1, arg2));
  }

 private:
  template <typename U>
  friend class Promise;
  Future(SharedValue<T>* val) : value_(val) {  // NOLINT
    value_->Up();
  }

  void swap(Future<T>& other) {
    SharedValue<T>* tmp_value = other.value_;
    other.value_ = this->value_;
    this->value_ = tmp_value;
  }

  SharedValue<T>* value_;
};

// Promise represents an obligation to set a value.
template <typename T>
class Promise {
 public:
  Promise() : value_(new SharedValue<T>()) {}

  Promise(const Promise<T>& copy) : value_(copy.value_) {  // NOLINT
    value_->Up();
  }

  Promise& operator=(const Promise<T>& assign) {
    Promise tmp(assign);
    this->swap(tmp);
    return *this;
  }

  ~Promise() { value_->Down(); }

  void SetValue(const T& val) { value_->Set(val); }

  Future<T> GetFuture() const { return Future<T>(value_); }

 private:
  void swap(Promise<T>& other) {
    SharedValue<T>* tmp_value = other.value_;
    other.value_ = this->value_;
    this->value_ = tmp_value;
  }

  SharedValue<T>* value_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_FUTURES_H_
