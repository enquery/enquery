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

#include "enquery/status.h"
#include <string.h>
#include <string>
#include <algorithm>
#include "enquery/portability.h"

namespace {
// When constructing an error Status with MakeFromSystemError, the module
// field will specify "os" indicating the error was with an API provided
// by the operating system.
const char* const kModuleOS = "os";

// For the sake of efficiency, Status instances that represent success do
// not involve allocating a Rep on the heap. Because efficiency also
// dictates we return const string refs rather than string copies, we
// return a const ref to kEmptyString when accessors are called on Status
// objects that represent success.
const std::string kEmptyString("");

}  // namespace

namespace enquery {
class Status::Rep {
 public:
  std::string module_;
  std::string message_;
  errno_t code_;

  Rep() : code_(0) {}
};

Status::Status() : rep_(NULL) {}

Status::Status(Rep* rep) : rep_(rep) {}

Status::Status(const Status& other) : rep_(NULL) {
  if (other.rep_ != 0) {
    rep_ = new Rep(*other.rep_);
  }
}

Status& Status::operator=(const Status& other) {
  Status tmp(other);
  std::swap(rep_, tmp.rep_);
  return *this;
}

Status::~Status() { delete rep_; }

Status Status::OK() { return Status(); }

Status Status::MakeError(const char* module, const char* msg) {
  return Status::MakeError(module, msg, 0);
}

Status Status::MakeError(const char* module, const char* msg, errno_t code) {
  Rep* rep = new Rep();

  rep->module_ = module ? module : "";
  rep->message_ = msg ? msg : "";
  rep->code_ = code;

  return Status(rep);
}

Status Status::MakeFromSystemError(errno_t errorNum) {
  Rep* rep = new Rep();
  rep->code_ = errorNum;
  rep->module_ = kModuleOS;
  rep->message_ = SystemErrorToString(errorNum);
  return Status(rep);
}

errno_t Status::GetCode() const {
  if (rep_) {
    return rep_->code_;
  }
  return 0;
}

const char* Status::GetModule() const {
  if (rep_) {
    return rep_->module_.c_str();
  }
  return kEmptyString.c_str();
}

const char* Status::GetMessage() const {
  if (rep_) {
    return rep_->message_.c_str();
  }
  return kEmptyString.c_str();
}

}  // namespace enquery
