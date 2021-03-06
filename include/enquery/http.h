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

#ifndef INCLUDE_ENQUERY_HTTP_H_
#define INCLUDE_ENQUERY_HTTP_H_

#include "enquery/shared.h"
#include "enquery/status.h"

namespace enquery {

class HttpClient;

class Http {
 public:
  virtual ~Http();

  // Create an instance of the library.
  static Http* Create(Status* status);

  // Create an HTTP client
  virtual HttpClient* CreateClient(Status* status) = 0;

 protected:
  Http();

 private:
  Http(const Http& no_copy);
  Http& operator=(const Http& no_assign);
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_HTTP_H_
