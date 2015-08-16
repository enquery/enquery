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

#include "enquery/portability.h"
#include <string.h>

namespace enquery {
  
std::string SystemErrorToString(errno_t error_number) {
  const size_t kMaxErrorLen = 256;
#ifdef _WIN32
  // Windows
  TCHAR tmp[kMaxErrorLen]={0};
  FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, error_number, 0, tmp,
    kMaxErrorLen, NULL);
  return std::string(tmp);
#else
  // UN*X
  char tmp[kMaxErrorLen]={0};
#if (((_POSIX_C_SOURCE >= 200112L || _XOPEN_SOURCE >= 600) && !_GNU_SOURCE) || \
     __MACH__)
  strerror_r(error_number, tmp, kMaxErrorLen);
  return std::string(tmp);
#else
  return std::string(strerror_r(error_number, tmp, kMaxErrorLen));
#endif  // #if (((_POSIX .... )))
#endif  // #ifdef _WIN32
}

}  // namespace enquery
