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

#ifndef INCLUDE_ENQUERY_HTTP_CLIENT_H_
#define INCLUDE_ENQUERY_HTTP_CLIENT_H_

#include "enquery/status.h"

namespace enquery {

class HttpRequest;
class HttpResponse;

class HttpClient {
 public:
  virtual ~HttpClient() {}

  virtual HttpResponse* SendRequest(const HttpRequest& request,
                                    Status* status) = 0;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_HTTP_CLIENT_H_
