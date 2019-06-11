// ======================================================================== //
// Copyright 2009-2019 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#pragma once

#include "platform.h"
// std
#include <algorithm>  // std::min etc on windows
#include <mutex>
#include <stdexcept>

#ifdef _WIN32
// ----------- windows only -----------
typedef unsigned long long id_t;
#ifndef _USE_MATH_DEFINES
#define _USE_MATH_DEFINES
#endif
#include <cmath>
#ifdef _M_X64
typedef long long ssize_t;
#else
typedef int ssize_t;
#endif
#else
// ----------- NOT windows -----------
#include "unistd.h"
#endif

#include "math/ospmath.h"

#include <stdint.h>

#ifdef _WIN32
#ifdef ospray_common_EXPORTS
#define OSPCOMMON_INTERFACE __declspec(dllexport)
#else
#define OSPCOMMON_INTERFACE __declspec(dllimport)
#endif
#else
#define OSPCOMMON_INTERFACE
#endif

#ifdef NDEBUG
#define Assert(expr)        /* nothing */
#define Assert2(expr, expl) /* nothing */
#define AssertError(errMsg) /* nothing */
#else
#define Assert(expr)                                         \
  ((void)((expr) ? 0                                         \
                 : ((void)ospcommon::doAssertion(            \
                        __FILE__, __LINE__, #expr, nullptr), \
                    0)))
#define Assert2(expr, expl)                               \
  ((void)((expr) ? 0                                      \
                 : ((void)ospcommon::doAssertion(         \
                        __FILE__, __LINE__, #expr, expl), \
                    0)))
#define AssertError(errMsg) doAssertion(__FILE__, __LINE__, (errMsg), nullptr)
#endif

#ifdef _WIN32
#define __PRETTY_FUNCTION__ __FUNCSIG__
#endif
#define NOTIMPLEMENTED                                        \
  throw std::runtime_error(std::string(__PRETTY_FUNCTION__) + \
                           ": not implemented...")

#define SCOPED_LOCK(x)                  \
  std::lock_guard<std::mutex> _lock(x); \
  (void)_lock;

namespace ospcommon {

  using byte_t = unsigned char;

  OSPCOMMON_INTERFACE void doAssertion(const char *file,
                                       int line,
                                       const char *expr,
                                       const char *expl);

  /*! remove specified num arguments from an ac/av arglist */
  OSPCOMMON_INTERFACE void removeArgs(int &ac,
                                      const char **&av,
                                      int where,
                                      int howMany);
  OSPCOMMON_INTERFACE void loadLibrary(const std::string &name,
                                       bool anchor = true);
  OSPCOMMON_INTERFACE void loadDefaultLibrary();
  OSPCOMMON_INTERFACE void *getSymbol(const std::string &name);

  // NOTE(jda) - Implement make_unique() as it didn't show up until C++14...
  template <typename T, typename... Args>
  inline std::unique_ptr<T> make_unique(Args &&... args)
  {
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
  }

}  // namespace ospcommon
