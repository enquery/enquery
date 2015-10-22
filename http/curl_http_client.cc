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

#include "http/curl_http_client.h"
#include <assert.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>
#include "http/curl_http_response.h"
#include "enquery/buffer.h"
#include "enquery/http_request.h"
#include "enquery/http_response.h"
#include "enquery/shared.h"
#include "enquery/slice.h"
#include "enquery/utility.h"

using enquery::Buffer;
using enquery::Slice;

namespace {
const char* const kCurlModule = "curl";

void curl_handle_deleter(CURL* curl) { curl_easy_cleanup(curl); }

size_t curl_read_buffer(char* data, size_t size, size_t nmemb, void* user) {
  assert(user != NULL);
  const size_t total_bytes = size * nmemb;
  if (total_bytes == 0) {
    return 0;
  }
  assert(data != NULL);
  Slice* slice = reinterpret_cast<Slice*>(user);
  // It's plausible that the curl library could ask for more data than we
  // actually reference in our slice. Determine the actual number that we
  // can copy and return that.
  const size_t num_read = std::min(total_bytes, slice->size());
  memcpy(data, slice->data(), num_read);
  return num_read;
}

// Callback function that appends received data to a Buffer.
size_t curl_write_buffer(char* data, size_t size, size_t nmemb, void* user) {
  assert(user != NULL);
  const size_t total_bytes = size * nmemb;
  if (total_bytes == 0) {
    return 0;
  }
  assert(data != NULL);
  Buffer* buffer = reinterpret_cast<Buffer*>(user);
  buffer->Append(data, total_bytes);
  return total_bytes;
}

}  // namespace

namespace enquery {

CurlHttpClient::CurlHttpClient(Shared<void>::Ptr library_ref)
    : library_ref_(library_ref) {
  // TODO(tdial): Implement
}

CurlHttpClient::~CurlHttpClient() {
  // TODO(tdial): Implement
}

HttpResponse* CurlHttpClient::SendRequest(const HttpRequest& request,
                                          Status* status_out) {
  // curl_easy_init() must be called to obtain a context.
  Status status(Status::OK());
  Shared<CURL>::Ptr curl(curl_easy_init(), curl_handle_deleter);
  if (curl.get() == NULL) {
    MaybeAssign(status_out,
                Status::MakeError(kCurlModule, "curl_easy_init() failed"));
    return NULL;
  }

  // Set the URL to use for the request
  CURLcode result;
  result = curl_easy_setopt(curl.get(), CURLOPT_URL, request.url());
  if (result != 0) {
    MaybeAssign(status_out,
                Status::MakeError(kCurlModule, curl_easy_strerror(result)));
    return NULL;
  }

  // Declare a slice that references the body data. It's O.K. if the data
  // is empty; we only set up the "read" callback in the case where we
  // actually have body data to send.
  Slice body_slice(request.body());

  // Do we have a body?
  if (request.HasBody()) {
    // Set up the read callback function
    result =
        curl_easy_setopt(curl.get(), CURLOPT_READFUNCTION, curl_read_buffer);
    if (result != 0) {
      MaybeAssign(status_out,
                  Status::MakeError(kCurlModule, curl_easy_strerror(result)));
      return NULL;
    }

    // Set up the read callback parameter
    result = curl_easy_setopt(curl.get(), CURLOPT_READDATA, &body_slice);
    if (result != 0) {
      MaybeAssign(status_out,
                  Status::MakeError(kCurlModule, curl_easy_strerror(result)));
      return NULL;
    }
  }

  // Set up the "write callback" - the function that the curl API uses to
  // write response data back to the client.
  result =
      curl_easy_setopt(curl.get(), CURLOPT_WRITEFUNCTION, curl_write_buffer);
  if (result != 0) {
    MaybeAssign(status_out,
                Status::MakeError(kCurlModule, curl_easy_strerror(result)));
    return NULL;
  }

  // Set the write callback parameter: we're passing a pointer to a Buffer
  // instance. Assuming the request succeeds, we'll construct the HttpResponse
  // object with the shared pointer to the buffer.
  Shared<Buffer>::Ptr buf(new Buffer());
  result = curl_easy_setopt(curl.get(), CURLOPT_WRITEDATA, buf.get());
  if (result != 0) {
    MaybeAssign(status_out,
                Status::MakeError(kCurlModule, curl_easy_strerror(result)));
    return NULL;
  }

  // Send the request
  result = curl_easy_perform(curl.get());
  if (result != 0) {
    MaybeAssign(status_out,
                Status::MakeError(kCurlModule, curl_easy_strerror(result)));
    return NULL;
  }

  MaybeAssign(status_out, status);
  return new CurlHttpResponse(buf);
}

}  // namespace enquery
