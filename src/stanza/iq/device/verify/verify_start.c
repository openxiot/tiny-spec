/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/verify/verify_start.h>

TINYCORE_RET QueryVerifyStart_Construct(QueryVerifyStart *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(QueryVerifyStart));

    return TINY_RET_OK;
}

void QueryVerifyStart_Dispose(QueryVerifyStart *thiz)
{
    RETURN_IF_FAIL(thiz);
}

TINYCORE_RET ResultVerifyStart_Construct(ResultVerifyStart *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(ResultVerifyStart));

    return TINY_RET_OK;
}

void ResultVerifyStart_Dispose(ResultVerifyStart *thiz)
{
    RETURN_IF_FAIL(thiz);
}
