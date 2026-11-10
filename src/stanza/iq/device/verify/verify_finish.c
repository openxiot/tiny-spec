/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/verify/verify_finish.h>

TINYCORE_RET QueryVerifyFinish_Construct(QueryVerifyFinish *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(QueryVerifyFinish));

    return TINY_RET_OK;
}

void QueryVerifyFinish_Dispose(QueryVerifyFinish *thiz)
{
    RETURN_IF_FAIL(thiz);
}
