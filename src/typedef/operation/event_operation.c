/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <tinycore/tiny_alloc.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/typedef/operation/event_operation.h>

static TinyRet EventOperation_CopyArguments(TinyList *dst, const TinyList *src)
{
    uint32_t size = 0;
    uint32_t i = 0;
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(dst, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(src, TINY_RET_E_NULL_PTR);

    size = TinyList_GetSize(src);
    for (i = 0; i < size; ++i)
    {
        const ArgumentOperation *srcOperation = (const ArgumentOperation *) TinyList_GetAt(src, i);
        ArgumentOperation *dstOperation = NULL;

        if (srcOperation == NULL)
        {
            continue;
        }

        dstOperation = ArgumentOperation_CopyFrom(srcOperation);
        if (dstOperation == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        if (TinyList_AddTail(dst, dstOperation) == NULL)
        {
            ArgumentOperation_Delete(dstOperation);
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }
    }

    return ret;
}

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

TinyRet EventOperation_Construct(EventOperation *thiz)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(EventOperation));

    TinyList_Construct(&thiz->arguments);
    ret = ArgumentOperations_SetDeleteListener(&thiz->arguments);

    return ret;
}

void EventOperation_Dispose(EventOperation *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyList_Dispose(&thiz->arguments);
}

EventOperation * EventOperation_CopyFrom(EventOperation *other)
{
    EventOperation *thiz = NULL;

    RETURN_VAL_IF_FAIL(other, NULL);

    do
    {
        thiz = EventOperation_New();
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(EventOperation_Copy(thiz, other)))
        {
            EventOperation_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

TinyRet EventOperation_Copy(EventOperation *dst, EventOperation *src)
{
    TinyRet ret = TINY_RET_OK;

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
        dst->argumentsCompact = src->argumentsCompact;

        TinyList_RemoveAll(&dst->arguments);

        ret = EventOperation_CopyArguments(&dst->arguments, &src->arguments);
        if (RET_FAILED(ret))
        {
            break;
        }
    } while (false);

    return ret;
}

ArgumentOperation * EventOperation_GetArgument(EventOperation *thiz, uint16_t piid)
{
    uint32_t size = 0;
    uint32_t i = 0;

    RETURN_VAL_IF_FAIL(thiz, NULL);

    size = TinyList_GetSize(&thiz->arguments);
    for (i = 0; i < size; ++i)
    {
        ArgumentOperation *operation = (ArgumentOperation *) TinyList_GetAt(&thiz->arguments, i);

        if (operation != NULL && operation->piid == piid)
        {
            return operation;
        }
    }

    return NULL;
}

ArgumentOperation * EventOperation_EnsureArgument(EventOperation *thiz, uint16_t piid)
{
    ArgumentOperation *operation = NULL;
    uint32_t size = 0;
    uint32_t i = 0;

    RETURN_VAL_IF_FAIL(thiz, NULL);

    size = TinyList_GetSize(&thiz->arguments);
    for (i = 0; i < size; ++i)
    {
        operation = (ArgumentOperation *) TinyList_GetAt(&thiz->arguments, i);
        if (operation != NULL && operation->piid == piid)
        {
            return operation;
        }
    }

    operation = ArgumentOperation_New(piid);
    if (operation == NULL)
    {
        return NULL;
    }

    if (TinyList_AddTail(&thiz->arguments, operation) == NULL)
    {
        ArgumentOperation_Delete(operation);
        return NULL;
    }

    return operation;
}
