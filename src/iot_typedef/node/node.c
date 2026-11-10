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
#include <tinyspec/iot_typedef/node/node.h>

Node *Node_New(void)
{
    Node *thiz = NULL;

    do
    {
        thiz = (Node *)tiny_malloc(sizeof(Node));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(Node_Construct(thiz)))
        {
            Node_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void Node_Delete(Node *thiz)
{
    RETURN_IF_FAIL(thiz);

    Node_Dispose(thiz);
    tiny_free(thiz);
}

TINYCORE_RET Node_Construct(Node *thiz)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(Node));

    return ret;
}

void Node_Dispose(Node *thiz)
{
    RETURN_IF_FAIL(thiz);
}

TINYCORE_RET Node_Copy(Node *dst, Node *src)
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

        strncpy(dst->serialNumber, src->serialNumber, NODE_SERIAL_NUMBER_LENGTH);

        dst->productId = src->productId;
        dst->productVersion = src->productVersion;
    } while (false);

    return ret;
}
