/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_JSON_UTIL_H
#define TINYSPEC_JSON_UTIL_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_array.h>

TINYSPEC_BEGIN_DECLS

/*
 * Deep copy src into an already constructed dst.  Any previous content of
 * dst is released.  TinyCore exposes JsonArray_NewFrom() rather than an
 * in-place copy, so this helper bridges the two styles.
 */
TINYSPEC_API TINYCORE_RET JsonArray_CopyInto(JsonArray *dst, const JsonArray *src);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_JSON_UTIL_H */
