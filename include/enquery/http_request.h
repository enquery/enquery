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

#ifndef INCLUDE_ENQUERY_HTTP_REQUEST_H_
#define INCLUDE_ENQUERY_HTTP_REQUEST_H_

#include <string>
#include "enquery/buffer.h"

namespace enquery {

class HttpRequest {
 public:
  typedef enum { GET = 0, PUT = 1 } Method;

  HttpRequest();
  ~HttpRequest();

  // Copy Constructor, Assignment, Destructor O.K.

  // Set request method (GET, PUT, etc,.)
  HttpRequest& set_method(int method);

  // Return request method.
  int method() const;

  // Set Request URL.
  HttpRequest& set_url(const char* url);

  // Get Request URL.
  const char* url() const;

  // Set body data (typically POST data)
  HttpRequest& set_body(const char* data, size_t size);

  bool HasBody() const;

  // Get body data
  const Buffer& body() const;

 private:
  int method_;
  std::string url_;
  Buffer body_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_HTTP_REQUEST_H_
