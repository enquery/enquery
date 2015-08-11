# Copyright 2015 The Enquery Authors.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License. See the AUTHORS file for names of
# contributors.


# Uncomment exactly one of the lines labelled (A), (B), and (C) below
# to switch between compilation modes.

# A: Production use (full optimizations)
#OPT ?= -O3 -DNDEBUG

# B: Debug mode, with full line-level debugging symbols
OPT ?= -g2

# C: Profiling mode: optimizations, but w/debugging symbols
#OPT ?= -O3 -g2 -DNDEBUG

PREFIX ?= /usr/local

# Warning Flags
WARNINGFLAGS = -Wall -Werror

# Detect what platform we're building on
$(shell CXX="$(CXX)" TARGET_OS="$(TARGET_OS)" \
	./build_config build_config.mk .)

# Include the file generated by the previous line to set build flags, sources
include build_config.mk

CXXFLAGS += -I. -I./include $(PLATFORM_CXXFLAGS) $(OPT) $(WARNINGFLAGS) $(FEATURES)
BASE_OBJECTS = $(BASE_FILES:.cc=.o)
HTTP_OBJECTS = $(HTTP_FILES:.cc=.o)
TESTS = atomic_test futures_test shared_pointer_test shared_test status_test
DEV = demo

# Targets
all: libenquery.a $(DEV)

.PHONY:
check: $(TESTS)
	for t in $(TESTS); do echo "** Running $$t"; ./$$t || exit 1; done

.PHONY:
clean:
	-rm -f */*.o build_config.mk *.a $(TESTS) $(UTILS)

.PHONY:
count:
	wc -l $(CPPLINT_SOURCES)

demo: dev/demo.o libenquery.a
	$(CXX) $(CXXFLAGS) dev/demo.o libenquery.a $(LIBRARIES) -o $@

.PHONY: install
install: libenquery.a
	cp libenquery.a "$(PREFIX)/lib"
	mkdir -p "$(PREFIX)/include/enquery/base"
	cp include/enquery/base/*.h "$(PREFIX)/include/base"

libenquery.a: $(BASE_OBJECTS) $(HTTP_OBJECTS)
	ar rcs libenquery.a $(BASE_OBJECTS) $(HTTP_OBJECTS)
	ranlib libenquery.a

.PHONY:
lint:
	$(LINT_TOOL) $(CPPLINT_SOURCES)

.PHONY:
neat: clean
	-rm -f *~ .*~ */*~ ./include/*/*~

.PHONY:
reformat:
	clang-format -i $(CPPLINT_SOURCES)

# Tests
atomic_test: base/atomic_test.o $(BASE_OBJECTS)                                \
	$(BASE_OBJECTS)
	$(CXX) base/atomic_test.o $(BASE_OBJECTS)                                    \
	$(LIBRARIES) -o $@

futures_test: base/futures_test.o $(BASE_OBJECTS)
	$(CXX) base/futures_test.o $(BASE_OBJECTS)                                   \
	$(LIBRARIES) -o $@

shared_pointer_test: base/shared_pointer_test.o $(BASE_OBJECTS)
	$(CXX) base/shared_pointer_test.o $(BASE_OBJECTS)                            \
	$(LIBRARIES) -o $@

shared_test: base/shared_test.o $(BASE_OBJECTS) $(HTTP_OBJECTS)
	$(CXX) base/shared_test.o $(BASE_OBJECTS)	$(HTTP_OBJECTS) $(LIBRARIES) -o $@

status_test: base/status_test.o $(BASE_OBJECTS) $(HTTP_OBJECTS)
	$(CXX) base/status_test.o $(BASE_OBJECTS) $(HTTP_OBJECTS) $(LIBRARIES) -o $@ 

# Suffix Rules
.cc.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

