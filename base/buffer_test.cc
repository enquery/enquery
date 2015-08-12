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

#include <string>
#include "enquery/buffer.h"
#include "enquery/testing.h"

using ::enquery::Buffer;

int main(int argc, char* argv[]) {
  // Test empty buffer has size of zero.
  Buffer b1;
  ASSERT_EQUALS(b1.Size(), 0);

  // Test construction from string. Size should include NULL.
  const std::string kTestStr1 = "test1";
  Buffer b2(kTestStr1);
  ASSERT_EQUALS(b2.Size(), kTestStr1.size() + 1);

  // Test copy construction
  Buffer b2_copy(b2);

  // Test equality operator
  ASSERT_TRUE(b2 == b2_copy);

  // Ensure equality operator fails appropriately.
  const std::string kTestStr2 = "test2";
  Buffer b3(kTestStr2);
  ASSERT_FALSE(b2 == b3);

  // Test Data accessor
  ASSERT_EQUALS(b3.Data()[0], 't');

  return EXIT_SUCCESS;
}
