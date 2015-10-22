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

#ifndef HTTP_CURL_HTTP_RESPONSE_H_
#define HTTP_CURL_HTTP_RESPONSE_H_

#include "enquery/http_response.h"
#include "enquery/shared.h"

namespace enquery {

class Buffer;

class CurlHttpResponse : public HttpResponse {
 public:
  explicit CurlHttpResponse(Shared<Buffer>::Ptr body);
  virtual ~CurlHttpResponse();

  virtual const char* Body() const;
  virtual size_t BodySize() const;

 private:
  CurlHttpResponse(const CurlHttpResponse& no_copy);
  CurlHttpResponse& operator=(const CurlHttpResponse& no_assign);
  Shared<Buffer>::Ptr body_;
};

}  // namespace enquery

#endif  // HTTP_CURL_HTTP_RESPONSE_H_
