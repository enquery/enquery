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

#ifndef HTTP_CURL_HTTP_H_
#define HTTP_CURL_HTTP_H_

#include "enquery/http.h"
#include "enquery/shared.h"
#include "enquery/status.h"

namespace enquery {

class CurlHttp : public Http {
 public:
  virtual ~CurlHttp();

  // Create an instance of the http library.
  static Http* Create(Status* status);

  // Create an instance of an http client.
  virtual HttpClient* CreateClient(Status* status);

  // Do not use. This is for unit testing the library.
  static int GetReferenceCount();

 private:
  explicit CurlHttp(Shared<void>::Ptr ref);
  CurlHttp(const CurlHttp& copy_from);
  CurlHttp& operator=(const CurlHttp& assign_from);
  Shared<void>::Ptr module_ref_;
};

}  // namespace enquery

#endif  // HTTP_CURL_HTTP_H_
