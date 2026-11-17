/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/iq.h>

TinyRet IQ_InitializeQuery(IQ *thiz, const char *id, const IQMethod method)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(id, TINY_RET_E_NULL_PTR);

    thiz->type = IQ_TYPE_QUERY;
    thiz->content.query.method = method;
    strncpy(thiz->id, id, IQ_ID_LENGTH);
    return IQQuery_Initialize(&thiz->content.query);
}

TinyRet IQ_InitializeResult(IQ *thiz, const char *id, const IQMethod method)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(id, TINY_RET_E_NULL_PTR);

    thiz->type = IQ_TYPE_RESULT;
    thiz->content.result.method = method;
    strncpy(thiz->id, id, IQ_ID_LENGTH);
    return IQResult_Initialize(&thiz->content.result);
}

void IQ_Dispose(IQ *thiz)
{
    switch (thiz->type)
    {
        case IQ_TYPE_QUERY:
            IQQuery_Dispose(&thiz->content.query);
            break;

        case IQ_TYPE_RESULT:
            IQResult_Dispose(&thiz->content.result);
            break;

        case IQ_TYPE_ERROR:
            IQError_Dispose(&thiz->content.error);
            break;

        default:
            break;
    }
}
