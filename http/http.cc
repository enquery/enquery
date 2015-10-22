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

#include "enquery/http.h"
#include <stdlib.h>
#include "http/curl_http_client.h"
#include "http/curl_http.h"
#include "enquery/shared.h"
#include "enquery/status.h"
#include "enquery/utility.h"

namespace enquery {

Http::Http() {}

Http::~Http() {}

Http* Http::Create(Status* status_out) {
  Status status(Status::OK());
  Http* http = CurlHttp::Create(&status);
  MaybeAssign(status_out, status);
  if (status.IsFailure()) {
    return NULL;
  }
  return http;
}

}  // namespace enquery
