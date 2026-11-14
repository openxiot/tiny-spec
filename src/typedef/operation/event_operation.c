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
#include <tinyspec/typedef/operation/event_operation.h>

EventOperation * EventOperation_New(void)
{
    EventOperation *thiz = NULL;

    do
    {
        thiz = (EventOperation *)tiny_malloc(sizeof(EventOperation));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(EventOperation_Construct(thiz)))
        {
            EventOperation_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void EventOperation_Delete(EventOperation *thiz)
{
    RETURN_IF_FAIL(thiz);

    EventOperation_Dispose(thiz);
    tiny_free(thiz);
}

TINYCORE_RET EventOperation_Construct(EventOperation *thiz)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(EventOperation));

    return ret;
}

void EventOperation_Dispose(EventOperation *thiz)
{
    RETURN_IF_FAIL(thiz);

    if (thiz->arguments != NULL)
    {
        JsonArray_Delete(thiz->arguments);
    }
}

EventOperation * EventOperation_CopyFrom(EventOperation *other)
{
    EventOperation * thiz = NULL;

    RETURN_VAL_IF_FAIL(other, NULL);

    do
    {
        thiz = (EventOperation *)tiny_malloc(sizeof(EventOperation));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(EventOperation_Construct(thiz)))
        {
            EventOperation_Delete(thiz);
            thiz = NULL;
            break;
        }

        strncpy(thiz->eid.did, other->eid.did, IOT_DEVICE_ID_LENGTH);
        thiz->eid.siid = other->eid.siid;
        thiz->eid.iid = other->eid.iid;
        thiz->status = other->status;

        if (other->arguments == NULL)
        {
            break;
        }

        thiz->arguments = JsonArray_NewFrom(other->arguments);
        if (thiz->arguments == NULL)
        {
            EventOperation_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

TINYCORE_RET EventOperation_Copy(EventOperation *dst, EventOperation *src)
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

        strncpy(dst->eid.did, src->eid.did, IOT_DEVICE_ID_LENGTH);
        dst->eid.siid = src->eid.siid;
        dst->eid.iid = src->eid.iid;
        dst->status = src->status;

        if (src->arguments == NULL)
        {
            break;
        }

        if (dst->arguments != NULL)
        {
            JsonArray_Delete(dst->arguments);
            dst->arguments = NULL;
        }

        dst->arguments = JsonArray_NewFrom(src->arguments);
        if (dst->arguments == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }
    } while (false);

    return ret;
}
