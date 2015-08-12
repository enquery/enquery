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

#include "enquery/buffer.h"
#include <assert.h>
#include <string.h>
#include <algorithm>

namespace enquery {

Buffer::Buffer() {}

Buffer::Buffer(const std::string& str) {
  const size_t size = str.size();
  data_.resize(size + 1);
  memcpy(&(data_[0]), str.c_str(), size);
  data_[size] = 0;
}

Buffer::Buffer(const char* data, size_t size) {
  assert(data != NULL);
  data_.resize(size);
  memcpy(&(data_[0]), data, size);
}

Buffer::Buffer(const Buffer& copy_from) { data_ = copy_from.data_; }

Buffer& Buffer::operator=(const Buffer& assign_from) {
  Buffer tmp(assign_from);
  this->swap(tmp);
  return *this;
}

Buffer::~Buffer() {
  // Intentionally empty
}

size_t Buffer::Size() const { return data_.size(); }

const char* Buffer::Data() const { return &(data_[0]); }

char* Buffer::Data() { return &(data_[0]); }

Buffer& Buffer::Append(const char* data, size_t size) {
  const size_t current_size = data_.size();
  data_.resize(current_size + size);
  memcpy(&(data_[current_size]), data, size);
  return *this;
}

void Buffer::swap(Buffer& other) {
  using std::swap;
  swap(this->data_, other.data_);
}

void swap(Buffer& lhs, Buffer& rhs) { lhs.swap(rhs); }

}  // namespace enquery

bool operator==(const enquery::Buffer& lhs, const enquery::Buffer& rhs) {
  if (lhs.Size() != rhs.Size()) {
    return false;
  }
  return (memcmp(lhs.Data(), rhs.Data(), lhs.Size()) == 0);
}
