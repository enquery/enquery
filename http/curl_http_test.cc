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

#include <stdlib.h>
#include "enquery/status.h"
#include "enquery/testing.h"
#include "http/curl_http.h"

using ::enquery::CurlHttp;
using ::enquery::Http;
using ::enquery::Shared;
using ::enquery::Status;

void use_ref(Shared<void>::Ptr ref) {
  int reference_count = CurlHttp::GetReferenceCount();
  ASSERT_EQUALS(reference_count, 2);
}

void make_ref() {
  // Begin by retrieving the global reference count
  int reference_count = CurlHttp::GetReferenceCount();
  ASSERT_EQUALS(reference_count, 0);

  // Ensure that initialization succeeds.
  Status status;
  Shared<Http>::Ptr ref1(CurlHttp::Create(&status));
  ASSERT_TRUE(status.IsSuccess());

  // Ensure that the library reference count is now one.
  reference_count = CurlHttp::GetReferenceCount();
  ASSERT_EQUALS(reference_count, 1);

  Shared<Http>::Ptr ref2(CurlHttp::Create(&status));

  // The global reference count should now be two.
  reference_count = CurlHttp::GetReferenceCount();
  ASSERT_EQUALS(reference_count, 2);

  // Pass the reference we received to a function. This should not increase
  // the library reference count; it increases the reference count of the
  // shared object, which should not require the global lock to be taken.
  use_ref(ref2);

  // Ensure that the library reference count is still 2
  reference_count = CurlHttp::GetReferenceCount();
  ASSERT_EQUALS(reference_count, 2);
}

int main(int argc, char* argv[]) {
  // Begin by ensuring that the library reference count is zero.
  int ref_count = CurlHttp::GetReferenceCount();
  ASSERT_EQUALS(ref_count, 0);

  // Call a helper that creates a reference to the library. The function
  // creates it on the stack. The reference should be destroyed on scope
  // exit.
  make_ref();

  // Check the library's global reference count again. It should be zero.
  ref_count = CurlHttp::GetReferenceCount();
  ASSERT_EQUALS(ref_count, 0);

  return EXIT_SUCCESS;
}
