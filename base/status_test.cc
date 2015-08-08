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
#include "enquery/status.h"
#include "enquery/testing.h"

using enquery::Status;

int main(int argc, char* argv[]) {
  // Ensure we can create a success status
  Status s1 = Status::OK();
  ASSERT_TRUE(s1.IsSuccess());
  ASSERT_FALSE(s1.IsFailure());

  // Ensure we can make an error status
  const int kTestCode = 7;
  const char* const kTestModule = "testmodule";
  const char* const kTestMessage = "testmessage";
  Status s2 = Status::MakeError(kTestModule, kTestMessage, kTestCode);
  ASSERT_TRUE(s2.IsFailure());
  ASSERT_EQUALS(s2.GetCode(), kTestCode);
  ASSERT_STRING_EQUALS(s2.GetModule(), kTestModule);
  ASSERT_STRING_EQUALS(s2.GetMessage(), kTestMessage);

  // Ensure we can create a system error
  Status s3 = Status::MakeFromSystemError(EINVAL);
  ASSERT_TRUE(s3.IsFailure());
  ASSERT_EQUALS(s3.GetCode(), EINVAL);
  const char* const kExpectedMessage = "Invalid argument";
  ASSERT_STRING_EQUALS(s3.GetMessage(), kExpectedMessage);

  // Test copy constructor
  Status s4 = Status::MakeError(kTestModule, kTestMessage, kTestCode);
  Status s5(s4);
  ASSERT_EQUALS(s4.IsSuccess(), s5.IsSuccess());
  ASSERT_EQUALS(s4.GetCode(), s5.GetCode());
  ASSERT_STRING_EQUALS(s4.GetModule(), s5.GetModule());
  ASSERT_STRING_EQUALS(s4.GetMessage(), s5.GetMessage());

  // Test assignment operator
  Status s6 = Status::OK();
  s6 = s5;
  ASSERT_EQUALS(s6.IsSuccess(), s5.IsSuccess());
  ASSERT_EQUALS(s6.GetCode(), s5.GetCode());
  ASSERT_STRING_EQUALS(s6.GetModule(), s5.GetModule());
  ASSERT_STRING_EQUALS(s6.GetMessage(), s5.GetMessage());

  // Test MakeError that takes module, message and no code
  Status s7 = Status::MakeError(kTestModule, kTestMessage);
  ASSERT_TRUE(s7.IsFailure());
  ASSERT_EQUALS(s7.GetCode(), 0);
  ASSERT_STRING_EQUALS(s7.GetModule(), kTestModule);
  ASSERT_STRING_EQUALS(s7.GetMessage(), kTestMessage);

  return EXIT_SUCCESS;
}
