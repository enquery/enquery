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

#ifndef INCLUDE_ENQUERY_TESTING_H_
#define INCLUDE_ENQUERY_TESTING_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ASSERT_EQUALS(lhs, rhs)                                          \
  for (;;) {                                                             \
    if (!((lhs) == (rhs))) {                                             \
      fprintf(stderr,                                                    \
              "Test failure on line %d: assertion (%s == %s) failed.\n", \
              __LINE__, (#lhs), (#rhs));                                 \
      exit(EXIT_FAILURE);                                                \
    }                                                                    \
    break;                                                               \
  }

#define ASSERT_NOT_EQUALS(lhs, rhs)                                      \
  for (;;) {                                                             \
    if (((lhs) == (rhs))) {                                              \
      fprintf(stderr,                                                    \
              "Test failure on line %d: assertion (%s != %s) failed.\n", \
              __LINE__, (#lhs), (#rhs));                                 \
      exit(EXIT_FAILURE);                                                \
    }                                                                    \
    break;                                                               \
  }

#define ASSERT_STRING_EQUALS(lhs, rhs)                                   \
  for (;;) {                                                             \
    if (0 != strcmp((lhs), (rhs))) {                                     \
      fprintf(stderr,                                                    \
              "Test failure on line %d: assertion (%s == %s) failed.\n", \
              __LINE__, (#lhs), (#rhs));                                 \
      exit(EXIT_FAILURE);                                                \
    }                                                                    \
    break;                                                               \
  }

#define ASSERT_FALSE(cond)                                                 \
  for (;;) {                                                               \
    if ((cond)) {                                                          \
      fprintf(stderr, "Test failure on line %d: assertion (%s) failed.\n", \
              __LINE__, (#cond));                                          \
      exit(EXIT_FAILURE);                                                  \
    }                                                                      \
    break;                                                                 \
  }

#define ASSERT_GREATER_THAN(lhs, rhs)                                   \
  for (;;) {                                                            \
    if (!((lhs) > (rhs))) {                                             \
      fprintf(stderr,                                                   \
              "Test failure on line %d: assertion (%s > %s) failed.\n", \
              __LINE__, (#lhs), (#rhs));                                \
      exit(EXIT_FAILURE);                                               \
    }                                                                   \
    break;                                                              \
  }

#define ASSERT_LESS_THAN(lhs, rhs)                                      \
  for (;;) {                                                            \
    if (!((lhs) < (rhs))) {                                             \
      fprintf(stderr,                                                   \
              "Test failure on line %d: assertion (%s < %s) failed.\n", \
              __LINE__, (#lhs), (#rhs));                                \
      exit(EXIT_FAILURE);                                               \
    }                                                                   \
    break;                                                              \
  }

#define ASSERT_TRUE(cond)                                                  \
  for (;;) {                                                               \
    if (!(cond)) {                                                         \
      fprintf(stderr, "Test failure on line %d: assertion (%s) failed.\n", \
              __LINE__, (#cond));                                          \
      exit(EXIT_FAILURE);                                                  \
    }                                                                      \
    break;                                                                 \
  }

#define ASSERT_VALID_POINTER(ptr)                                          \
  for (;;) {                                                               \
    if ((ptr) == NULL) {                                                   \
      fprintf(stderr, "Test failure on line %d: assertion (%s) failed.\n", \
              __LINE__, (#ptr));                                           \
      exit(EXIT_FAILURE);                                                  \
    }                                                                      \
    break;                                                                 \
  }

#endif  // INCLUDE_ENQUERY_TESTING_H_
