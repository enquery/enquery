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

#ifndef INCLUDE_ENQUERY_UTILITY_H_
#define INCLUDE_ENQUERY_UTILITY_H_

namespace enquery {

// Assign the value in the second argument 'rhs' to the value pointed to
// by 'lhs'. If 'lhs' is NULL, the function has no effect.
template <typename Typ_>
void MaybeAssign(Typ_* lhs, const Typ_& rhs) {
  if (lhs) {
    *lhs = rhs;
  }
}

}  // namespace enquery

#endif  // INCLUDE_ENQUERY_UTILITY_H_
