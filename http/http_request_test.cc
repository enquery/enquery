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
#include <string.h>
#include "enquery/http_request.h"
#include "enquery/status.h"
#include "enquery/testing.h"

using ::enquery::HttpRequest;
using ::enquery::Status;

int main(int argc, char* argv[]) {
  HttpRequest request;

  // Default method should be GET
  ASSERT_EQUALS(request.method(), HttpRequest::GET);

  // Test that method PUT sticks
  request.set_method(HttpRequest::PUT);
  ASSERT_EQUALS(request.method(), HttpRequest::PUT);

  // Test that method GET sticks
  request.set_method(HttpRequest::GET);
  ASSERT_EQUALS(request.method(), HttpRequest::GET);

  // method (bogus: should still be GET from last set)
  const int kBogusMethod = 17;
  request.set_method(kBogusMethod);
  ASSERT_EQUALS(request.method(), HttpRequest::GET);

  // URL
  const char* kTestUrl = "http://www.example.com";
  request.set_url(kTestUrl);
  ASSERT_EQUALS(strcmp(kTestUrl, request.url()), 0);

  return EXIT_SUCCESS;
}
