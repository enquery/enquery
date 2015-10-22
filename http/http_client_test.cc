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
#include "enquery/http_client.h"
#include "enquery/http.h"
#include "enquery/http_request.h"
#include "enquery/http_response.h"
#include "enquery/shared.h"
#include "enquery/status.h"
#include "enquery/testing.h"
#include "http/curl_http.h"

using ::enquery::CurlHttp;
using ::enquery::Http;
using ::enquery::HttpClient;
using ::enquery::HttpRequest;
using ::enquery::HttpResponse;
using ::enquery::Shared;
using ::enquery::Status;

int main(int argc, char* argv[]) {
  Status status;
  Shared<Http>::Ptr http(Http::Create(&status));
  ASSERT_TRUE(status.IsSuccess());

  Shared<HttpClient>::Ptr client(http->CreateClient(&status));
  ASSERT_TRUE(status.IsSuccess());

  HttpRequest request;

  // TODO(tdial): We can't and should not assume that we're connected to
  // the internet, nor should we assume that a local web server is running.
  // Should probably build a simple HTTP server to satisfy this unit test.

  request.set_url("http://www.example.com");
  // request.set_url("http://127.0.0.1");

  Shared<HttpResponse>::Ptr response(client->SendRequest(request, &status));
  ASSERT_TRUE(status.IsSuccess());

  // Check Curl's global reference count. It should be 1, because the
  // module holds the global reference and the client holds a shared
  // pointer to that.
  int ref_count = CurlHttp::GetReferenceCount();
  ASSERT_EQUALS(ref_count, 1);

  return EXIT_SUCCESS;
}
