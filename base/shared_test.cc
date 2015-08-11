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
#include <stdio.h>
#include <stdlib.h>
#include "enquery/shared.h"
#include "enquery/testing.h"

using ::enquery::Shared;

// Forward declare a type; the main function won't "see" the definition,
// which is done intentionally to ensure that the implementation of
// Shared::Ptr can handle incomplete types.
class Type;

// A function that factories a new instance of Type.
Type* factory_type(bool* val);

int main(int argc, char* argv[]) {
  bool destructed = false;

  for (;;) {
    // Create an instance of the type in a scope so that we can ensure that
    // it is destructed upon scope exit.
    Shared<Type>::Ptr type(factory_type(&destructed));
    break;
  }

  ASSERT_TRUE(destructed);

  return EXIT_SUCCESS;
}

// Define a type that accepts a pointer-to-bool as its single argument and
// sets that bool to 'true' on destruction.
class Type {
 public:
  explicit Type(bool* destructed) : destructed_(destructed) {}

  ~Type() { *destructed_ = true; }

 private:
  Type(const Type&);
  Type& operator=(const Type&);

  bool* destructed_;
};

// Implement the factory function defined above.
Type* factory_type(bool* val) { return new Type(val); }
