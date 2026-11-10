/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/verify/initialize.h>

TINYCORE_RET QueryInitialize_Construct(QueryInitialize *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(QueryInitialize));

    return TINY_RET_OK;
}

void QueryInitialize_Dispose(QueryInitialize *thiz)
{
    RETURN_IF_FAIL(thiz);
}
