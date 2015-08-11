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
#include "enquery/shared_pointer.h"
#include "enquery/testing.h"

using ::enquery::SharedPointer;

class Test;

Test* MakeTest(int* p);

int main(int argc, char* argv[]) {
  int value = 1;
  {
    SharedPointer<Test> test(MakeTest(&value));
    SharedPointer<Test> t1(test);
    SharedPointer<Test> t2;
    t2 = t1;
  }
  ASSERT_EQUALS(value, 0);
  return EXIT_SUCCESS;
}

class Test {
 public:
  explicit Test(int* val) : value_(val) {}

  ~Test() { *value_ = 0; }

 private:
  explicit Test(const Test& no_copy);
  Test& operator=(const Test& no_assign);
  int* value_;
};

Test* MakeTest(int* p) { return new Test(p); }
