/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_TINYSPEC_API_H
#define TINYSPEC_TINYSPEC_API_H

#include <tinycore/tiny_platform.h>

/*
 * TINYSPEC_SHARED is defined by the build system for consumers of a shared
 * library.  TINYSPEC_EXPORTS is defined while compiling the library itself.
 * When neither is defined (static library or source drop) the API macro is
 * empty.
 *
 * TINYSPEC_STATIC may be defined by consumers that link a static library
 * explicitly.
 */
#if defined(TINYSPEC_STATIC) || !defined(TINYSPEC_SHARED)
#  define TINYSPEC_API
#elif defined(_WIN32) || defined(__CYGWIN__)
#  if defined(TINYSPEC_EXPORTS)
#    define TINYSPEC_API __declspec(dllexport)
#  else
#    define TINYSPEC_API __declspec(dllimport)
#  endif
#elif defined(TINYCORE_COMPILER_GCC) || defined(TINYCORE_COMPILER_CLANG)
#  define TINYSPEC_API __attribute__((visibility("default")))
#else
#  define TINYSPEC_API
#endif

#endif /* TINYSPEC_TINYSPEC_API_H */
