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
  typedef enum Method {
    GET = 0,
    HEAD = 1,
    POST = 2,
    PUT = 3,
    DELETE = 4,
    TRACE = 5
  } Method;

  HttpRequest();
  ~HttpRequest();

  // Copy Constructor, Assignment, Destructor O.K.

  // Set request method (GET, PUT, etc,.)
  HttpRequest& set_method(Method method);

  // Return request method.
  Method method() const;

  // Set Request URI.
  HttpRequest& set_uri(const char* uri);

  // Get Request URI.
  const char* uri() const;

  // Set body data (typically POST data)
  HttpRequest& set_body(const char* data, size_t size);

  // Set the content type.
  HttpRequest& set_content_type(const char* content_type);

  // Get the content type.
  const char* content_type() const;

  bool HasBody() const;

  // Get body data
  const Buffer& body() const;

 private:
  Method method_;
  std::string uri_;
  Buffer body_;
  std::string content_type_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_HTTP_REQUEST_H_
