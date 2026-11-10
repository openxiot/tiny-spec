/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/key/get_access_key.h>

TINYCORE_RET ResultGetAccessKey_Construct(ResultGetAccessKey *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(ResultGetAccessKey));

    return TINY_RET_OK;
}

void ResultGetAccessKey_Dispose(ResultGetAccessKey *thiz)
{
    RETURN_IF_FAIL(thiz);
}
