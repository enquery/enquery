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

HttpRequest& HttpRequest::set_method(Method method) {
  switch (method) {
    case HttpRequest::GET:
    case HttpRequest::HEAD:
    case HttpRequest::POST:
    case HttpRequest::PUT:
    case HttpRequest::DELETE:
    case HttpRequest::TRACE:
      method_ = method;
      break;
  }
  return *this;
}

HttpRequest::Method HttpRequest::method() const { return method_; }

HttpRequest& HttpRequest::set_uri(const char* uri) {
  uri_ = uri ? uri : "";
  return *this;
}

const char* HttpRequest::uri() const { return uri_.c_str(); }

HttpRequest& HttpRequest::set_body(const char* data, size_t size) {
  body_ = Buffer(data, size);
  return *this;
}

HttpRequest& HttpRequest::set_content_type(const char* content_type) {
  content_type_ = content_type;
  return *this;
}

const char* HttpRequest::content_type() const {
  return content_type_.c_str();
}

bool HttpRequest::HasBody() const { return (body_.Size() > 0); }

const Buffer& HttpRequest::body() const { return body_; }

}  // namespace enquery
