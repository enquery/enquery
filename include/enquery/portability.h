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

#ifndef INCLUDE_ENQUERY_PORTABILITY_H_
#define INCLUDE_ENQUERY_PORTABILITY_H_

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#endif  // _WIN32

#include <string>

namespace enquery {

// Windows uses the DWORD for system errors. UN*X uses int.
#ifdef _WIN32
typedef DWORD errno_t;
#else
typedef int errno_t;
#endif

// Make a human-readable string from a system error number.
std::string SystemErrorToString(errno_t error_number);

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_PORTABILITY_H_
