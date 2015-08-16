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

#ifndef INCLUDE_ENQUERY_STATUS_H_
#define INCLUDE_ENQUERY_STATUS_H_

#include <stdlib.h>
#include "enquery/portability.h"

namespace enquery {

class Status {
 public:
  Status();
  Status(const Status&);
  Status& operator=(const Status&);
  ~Status();

  // Return a successful status object.
  static Status OK();

  // Make error status with module, message strings.
  // The 'module' and 'message' arguments may be NULL.
  static Status MakeError(const char* module, const char* message);

  // Make error status with module, message strings and integer code.
  // The 'module' and 'message' arguments may be NULL.
  static Status MakeError(const char* module, const char* message,
                          errno_t code);

  // Make error status constructed from an operating system error.
  // GetCode() will return the error, GetMessage() with the string that
  // would result from a call to strerror_r.
  static Status MakeFromSystemError(errno_t errorNumber);

  // Return true iff the status indicates success.
  bool IsSuccess() const { return (rep_ == NULL); }

  // Return true iff the status indicates failure.
  bool IsFailure() const { return (rep_ != NULL); }

  // Return the integer error code of the status object if one was set.
  // Returns zero if no error code was set, or if status represents success.
  errno_t GetCode() const;

  // Returns the name of the module that caused the error. For successful
  // status objects, returns a pointer to the empty string.
  const char* GetModule() const;

  // Returns a message describing the error. For successful status objects,
  // returns a pointer to the empty string.
  const char* GetMessage() const;

 private:
  class Rep;
  explicit Status(Rep* rep);
  Rep* rep_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_STATUS_H_
