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

#ifndef INCLUDE_ENQUERY_BUFFER_H_
#define INCLUDE_ENQUERY_BUFFER_H_

#include <stdlib.h>
#include <algorithm>
#include <vector>
#include <string>

namespace enquery {

class Buffer {
 public:
  // Construct empty.
  Buffer();

  // Construct from string.
  explicit Buffer(const std::string& str);

  // Construct from existing raw buffer.
  Buffer(const char* data, size_t size);

  // Copy constructor.
  Buffer(const Buffer& copy_from);

  // Copy assignment.
  Buffer& operator=(const Buffer& assign_from);

  // Destructor.
  ~Buffer();

  // Return the size in bytes.
  size_t Size() const;

  // Return pointer to raw data (const.)
  const char* Data() const;

  // Return pointer to raw data (non-const.)
  char* Data();

  // Append data at the end of the buffer.
  // Returns reference to self so that calls may be chained.
  Buffer& Append(const char* data, size_t size);

  // Canonical swap support
  void swap(Buffer& other);

 private:
  std::vector<char> data_;
};

// Canonical swap support
void swap(Buffer& lhs, Buffer& rhs);

}  // namespace enquery

bool operator==(const enquery::Buffer& lhs, const enquery::Buffer& rhs);

#endif  // INCLUDE_ENQUERY_BUFFER_H_
