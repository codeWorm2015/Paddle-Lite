// Copyright (c) 2019 PaddlePaddle Authors. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/*
 * This file implements an lightweight alternative for glog, which is more
 * friendly for mobile.
 */
#pragma once

#include <sys/time.h>
#include <sys/types.h>
#include <time.h>
#include <cstdlib>
#include <cstring>
#include <string>
#include "lite/utils/replace_stl/stream.h"

// NOLINTFILE()

// LOG()
#ifdef LITE_SHUTDOWN_LOG
#define LOG(status) LOG_##status
#define LOG_INFO paddle::lite::Voidify()
#define LOG_ERROR LOG_INFO
#define LOG_WARNING LOG_INFO
#define LOG_FATAL paddle::lite::VoidifyFatal()
#else
#define LOG(status) LOG_##status.stream()
#define LOG_INFO paddle::lite::LogMessage(__FILE__, __FUNCTION__, __LINE__, "I")
#define LOG_ERROR LOG_INFO
#define LOG_WARNING \
  paddle::lite::LogMessage(__FILE__, __FUNCTION__, __LINE__, "W")
#define LOG_FATAL \
  paddle::lite::LogMessageFatal(__FILE__, __FUNCTION__, __LINE__)
#endif

#ifdef LITE_SHUTDOWN_LOG
#define VLOG(level) paddle::lite::Voidify()
#else
// VLOG()
#define VLOG(level) \
  paddle::lite::VLogMessage(__FILE__, __FUNCTION__, __LINE__, level).stream()
#endif

// CHECK()
// clang-format off
#ifdef LITE_SHUTDOWN_LOG
#define CHECK(x) if (!(x)) paddle::lite::VoidifyFatal()
#define _CHECK_BINARY(x, cmp, y) CHECK(x cmp y)
#else
#define CHECK(x) if (!(x)) paddle::lite::LogMessageFatal(__FILE__, __FUNCTION__, __LINE__).stream() << "Check failed: " #x << ": " // NOLINT(*)
#define _CHECK_BINARY(x, cmp, y) CHECK(x cmp y) << x << "!" #cmp << y << " "
#endif

// clang-format on
#define CHECK_EQ(x, y) _CHECK_BINARY(x, ==, y)
#define CHECK_NE(x, y) _CHECK_BINARY(x, !=, y)
#define CHECK_LT(x, y) _CHECK_BINARY(x, <, y)
#define CHECK_LE(x, y) _CHECK_BINARY(x, <=, y)
#define CHECK_GT(x, y) _CHECK_BINARY(x, >, y)
#define CHECK_GE(x, y) _CHECK_BINARY(x, >=, y)

namespace paddle {
namespace lite {

#ifndef LITE_SHUTDOWN_LOG
void gen_log(STL::ostream& log_stream_,
             const char* file,
             const char* func,
             int lineno,
             const char* level,
             const int kMaxLen = 20);

// LogMessage
class LogMessage {
 public:
  LogMessage(const char* file,
             const char* func,
             int lineno,
             const char* level = "I") {
    paddle::lite::gen_log(log_stream_, file, func, lineno, level);
  }

  ~LogMessage() {
    log_stream_ << '\n';
    fprintf(stderr, "%s", log_stream_.str().c_str());
  }

  STL::ostream& stream() { return log_stream_; }

 protected:
  STL::stringstream log_stream_;

  LogMessage(const LogMessage&) = delete;
  void operator=(const LogMessage&) = delete;
};

// LogMessageFatal
class LogMessageFatal : public LogMessage {
 public:
  LogMessageFatal(const char* file,
                  const char* func,
                  int lineno,
                  const char* level = "F")
      : LogMessage(file, func, lineno, level) {}

  ~LogMessageFatal() {
    log_stream_ << '\n';
    fprintf(stderr, "%s", log_stream_.str().c_str());
#ifndef LITE_ON_TINY_PUBLISH
    abort();
#else
    assert(false);
#endif
  }
};

// VLOG
class VLogMessage {
 public:
  VLogMessage(const char* file,
              const char* func,
              int lineno,
              const int32_t level_int = 0) {
    const char* GLOG_v = std::getenv("GLOG_v");
    GLOG_v_int = (GLOG_v && atoi(GLOG_v) > 0) ? atoi(GLOG_v) : 0;
    this->level_int = level_int;
    if (GLOG_v_int < level_int) {
      return;
    }
    const char* level = std::to_string(level_int).c_str();
    paddle::lite::gen_log(log_stream_, file, func, lineno, level);
  }

  ~VLogMessage() {
    if (GLOG_v_int < this->level_int) {
      return;
    }
    log_stream_ << '\n';
    fprintf(stderr, "%s", log_stream_.str().c_str());
  }

  STL::ostream& stream() { return log_stream_; }

 protected:
  STL::stringstream log_stream_;
  int32_t GLOG_v_int;
  int32_t level_int;

  VLogMessage(const VLogMessage&) = delete;
  void operator=(const VLogMessage&) = delete;
};
#else
class Voidify {
 public:
  Voidify() {}
  ~Voidify() {}

  template <typename T>
  Voidify& operator<<(const T& obj) {
    return *this;
  }
};

class VoidifyFatal : public Voidify {
 public:
  ~VoidifyFatal() { assert(false); }
};

#endif

}  // namespace lite
}  // namespace paddle
