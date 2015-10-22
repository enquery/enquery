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
#include "enquery/http.h"
#include "enquery/shared.h"
#include "enquery/status.h"
#include "enquery/testing.h"
#include "http/curl_http.h"

using ::enquery::CurlHttp;
using ::enquery::Http;
using ::enquery::Shared;
using ::enquery::Status;

int sub_main() {
  Status status;
  Shared<Http>::Ptr http(Http::Create(&status));
  ASSERT_TRUE(status.IsSuccess());

  // Check the Curl's global reference count. It should be one.
  int ref_count = CurlHttp::GetReferenceCount();
  ASSERT_EQUALS(ref_count, 1);

  return EXIT_SUCCESS;
}

int main(int argc, char* argv[]) {
  sub_main();

  // Check the curl's global reference count. It should be zero.
  int ref_count = CurlHttp::GetReferenceCount();
  ASSERT_EQUALS(ref_count, 0);

  return EXIT_SUCCESS;
}
