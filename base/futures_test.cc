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

#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include "enquery/futures.h"
#include "enquery/testing.h"

using ::enquery::Future;
using ::enquery::Promise;

namespace {

int n0 = 0;
int n1_a = 0;
int n2_a = 0;
int n2_b = 0;

const int k0 = 1;
const int k1_a = 2;
const int k2_a = 3;
const int k2_b = 4;

void Notify_0() { n0 = k0; }

void Notify_1(int a) { n1_a = a; }

void Notify_2(int a, int b) {
  n2_a = a;
  n2_b = b;
}

}  // anonymous namespace

int main(int argc, char* argv[]) {
  Promise<int> promise;

  Future<int> future = promise.GetFuture();
  future.Notify(Notify_0);
  future.Notify(Notify_1, k1_a);
  future.Notify(Notify_2, k2_a, k2_b);

  const int kTestValue = 1;
  promise.SetValue(kTestValue);

  int value = future.GetValue();
  ASSERT_EQUALS(kTestValue, value);

  ASSERT_EQUALS(n0, k0);
  ASSERT_EQUALS(n1_a, k1_a);
  ASSERT_EQUALS(n2_a, k2_a);
  ASSERT_EQUALS(n2_b, k2_b);

  return EXIT_SUCCESS;
}
