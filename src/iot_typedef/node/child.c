/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_alloc.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/iot_typedef/node/child.h>

Child * Child_New(void)
{
    Child *thiz = NULL;

    do
    {
        thiz = (Child *)tiny_malloc(sizeof(Child));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(Child_Construct(thiz)))
        {
            Child_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void Child_Delete(Child *thiz)
{
    RETURN_IF_FAIL(thiz);

    Child_Dispose(thiz);
    tiny_free(thiz);
}

TINYCORE_RET Child_Construct(Child *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(Child));

    return TINY_RET_OK;
}

void Child_Dispose(Child *thiz)
{
    RETURN_IF_FAIL(thiz);
}

TINYCORE_RET Child_Copy(Child *dst, Child *src)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(dst, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(src, TINY_RET_E_NULL_PTR);

    do
    {
        if (src == dst)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        strncpy(dst->did, src->did, CHILD_DID_LENGTH);
        strncpy(dst->type, src->type, CHILD_TYPE_LENGTH);
        dst->productId = src->productId;
        dst->productVersion = src->productVersion;
        dst->online = src->online;
        strncpy(dst->protocol, src->protocol, CHILD_PROTOCOL_LENGTH);
        strncpy(dst->parentId, src->parentId, CHILD_PARENT_ID_LENGTH);
    } while (false);

    return ret;
}
