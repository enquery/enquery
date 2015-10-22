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

#include "http/curl_http.h"
#include "http/curl_http_client.h"
#include <assert.h>
#include <curl/curl.h>
#include <pthread.h>
#include <stdlib.h>
#include "enquery/scope_lock.h"
#include "enquery/status.h"
#include "enquery/utility.h"

using ::enquery::MaybeAssign;
using ::enquery::ScopeLock;
using ::enquery::Shared;
using ::enquery::Status;

namespace {

// TODO(tdial): This should probably be in a different location.
const char* const kCurlModule = "curl";

// The global mutex used to protect access to the reference count.
pthread_mutex_t g_curl_library_mutex = PTHREAD_MUTEX_INITIALIZER;

// The global library reference count
int g_curl_library_reference_count = 0;

Status LoadLibrary() {
  assert(g_curl_library_reference_count >= 0);
  ScopeLock lock(&g_curl_library_mutex);
  if (g_curl_library_reference_count == 0) {
    CURLcode code = curl_global_init(CURL_GLOBAL_ALL);
    if (code != 0) {
      return Status::MakeError(kCurlModule, curl_easy_strerror(code), code);
    }
  }
  ++g_curl_library_reference_count;
  return Status::OK();
}

void UnloadLibrary(void* not_used) {
  (void)not_used;
  ScopeLock lock(&g_curl_library_mutex);
  --g_curl_library_reference_count;
  assert(g_curl_library_reference_count >= 0);
  if (g_curl_library_reference_count == 0) {
    curl_global_cleanup();
  }
}

}  // namespace

namespace enquery {

CurlHttp::CurlHttp(Shared<void>::Ptr ref) : module_ref_(ref) {}

CurlHttp::~CurlHttp() {}

Http* CurlHttp::Create(Status* out_status) {
  Status status = LoadLibrary();
  MaybeAssign(out_status, status);
  if (status.IsFailure()) {
    return NULL;
  }
  return new CurlHttp(Shared<void>::Ptr(NULL, UnloadLibrary));
}

HttpClient* CurlHttp::CreateClient(Status* out_status) {
  Status status;
  MaybeAssign(out_status, status);
  return new CurlHttpClient(module_ref_);
}

int CurlHttp::GetReferenceCount() {
  ScopeLock lock(&g_curl_library_mutex);
  return g_curl_library_reference_count;
}

}  // namespace enquery
