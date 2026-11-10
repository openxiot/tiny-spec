/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/json_util.h>

TINYCORE_RET JsonArray_CopyInto(JsonArray *dst, const JsonArray *src)
{
    JsonArray *copy = NULL;

    RETURN_VAL_IF_FAIL(dst, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(src, TINY_RET_E_NULL_PTR);

    copy = JsonArray_NewFrom(src);
    if (copy == NULL)
    {
        return TINY_RET_E_NO_MEMORY;
    }

    JsonArray_Dispose(dst);
    *dst = *copy;
    tiny_free(copy);

    return TINY_RET_OK;
}
