/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/key/set_access_key.h>

TinyRet QuerySetAccessKey_Construct(QuerySetAccessKey *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(QuerySetAccessKey));

    return TINY_RET_OK;
}

void QuerySetAccessKey_Dispose(QuerySetAccessKey *thiz)
{
    RETURN_IF_FAIL(thiz);
}
