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

#include "enquery/http_request.h"

namespace enquery {

HttpRequest::HttpRequest() : method_(HttpRequest::GET) {}

HttpRequest::~HttpRequest() {}

HttpRequest& HttpRequest::set_method(int method) {
  switch (method) {
    case HttpRequest::GET:
    case HttpRequest::PUT:
      method_ = method;
      break;
  }
  return *this;
}

int HttpRequest::method() const { return method_; }

HttpRequest& HttpRequest::set_url(const char* url) {
  url_ = url ? url : "";
  return *this;
}

const char* HttpRequest::url() const { return url_.c_str(); }

// Set body data (typically POST data)
HttpRequest& HttpRequest::set_body(const char* data, size_t size) {
  body_ = Buffer(data, size);
  return *this;
}

bool HttpRequest::HasBody() const { return (body_.Size() > 0); }

// Get body data
const Buffer& HttpRequest::body() const { return body_; }

}  // namespace enquery
