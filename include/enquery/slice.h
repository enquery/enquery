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

#ifndef INCLUDE_ENQUERY_SLICE_H_
#define INCLUDE_ENQUERY_SLICE_H_

#include <assert.h>
#include <stdlib.h>
#include "enquery/buffer.h"

namespace enquery {

class Slice {
 public:
  // Create the empty slice.
  Slice() : data_(""), size_(0) {}

  // Default copy, assignment, and destructor are O.K.

  // Create a slice on a raw pointer and size.
  Slice(const char* data, size_t size) : data_(data), size_(size) {}

  // Create a slice on a Buffer
  explicit Slice(const Buffer& buf) : data_(buf.Data()), size_(buf.Size()) {}

  // Return a pointer to the start of the data.
  const char* data() const { return data_; }

  // Return size of data in bytes.
  size_t size() const { return size_; }

  // Return true if the length of the slice is zero.
  bool IsEmpty() const { return size_ == 0; }

  // Remove 'n' bytes from the front of the slice.
  void RemovePrefix(size_t n) {
    assert(n <= size_);
    data_ += n;
    size_ -= n;
  }

 private:
  const char* data_;
  size_t size_;
};

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_SLICE_H_
