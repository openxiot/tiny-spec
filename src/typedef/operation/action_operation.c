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
#include <tinyspec/typedef/operation/action_operation.h>

static TinyRet ActionOperation_CopyArguments(TinyList *dst, const TinyList *src)
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

ActionOperation * ActionOperation_New(void)
{
    ActionOperation *thiz = NULL;

    do
    {
        thiz = (ActionOperation *)tiny_malloc(sizeof(ActionOperation));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(ActionOperation_Construct(thiz)))
        {
            ActionOperation_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void ActionOperation_Delete(ActionOperation *thiz)
{
    RETURN_IF_FAIL(thiz);

    ActionOperation_Dispose(thiz);
    tiny_free(thiz);
}

TinyRet ActionOperation_Construct(ActionOperation *thiz)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(ActionOperation));

    do
    {
        TinyList_Construct(&thiz->in);
        ret = ArgumentOperations_SetDeleteListener(&thiz->in);
        if (RET_FAILED(ret))
        {
            break;
        }

        TinyList_Construct(&thiz->out);
        ret = ArgumentOperations_SetDeleteListener(&thiz->out);
        if (RET_FAILED(ret))
        {
            break;
        }
    } while (false);

    return ret;
}

void ActionOperation_Dispose(ActionOperation *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyList_Dispose(&thiz->in);
    TinyList_Dispose(&thiz->out);
}

TinyRet ActionOperation_Copy(ActionOperation *dst, ActionOperation *src)
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

        strncpy(dst->aid.did, src->aid.did, IOT_DEVICE_ID_LENGTH);
        dst->aid.siid = src->aid.siid;
        dst->aid.iid = src->aid.iid;
        dst->status = src->status;
        dst->argumentsCompact = src->argumentsCompact;

        TinyList_RemoveAll(&dst->in);
        TinyList_RemoveAll(&dst->out);

        ret = ActionOperation_CopyArguments(&dst->in, &src->in);
        if (RET_FAILED(ret))
        {
            break;
        }

        ret = ActionOperation_CopyArguments(&dst->out, &src->out);
        if (RET_FAILED(ret))
        {
            break;
        }
    } while (false);

    return ret;
}

ArgumentOperation * ActionOperation_GetIn(ActionOperation *thiz, uint16_t piid)
{
    uint32_t size = 0;
    uint32_t i = 0;

    RETURN_VAL_IF_FAIL(thiz, NULL);

    size = TinyList_GetSize(&thiz->in);
    for (i = 0; i < size; ++i)
    {
        ArgumentOperation *operation = (ArgumentOperation *) TinyList_GetAt(&thiz->in, i);

        if (operation != NULL && operation->piid == piid)
        {
            return operation;
        }
    }

    return NULL;
}

ArgumentOperation * ActionOperation_GetOut(ActionOperation *thiz, uint16_t piid)
{
    uint32_t size = 0;
    uint32_t i = 0;

    RETURN_VAL_IF_FAIL(thiz, NULL);

    size = TinyList_GetSize(&thiz->out);
    for (i = 0; i < size; ++i)
    {
        ArgumentOperation *operation = (ArgumentOperation *) TinyList_GetAt(&thiz->out, i);

        if (operation != NULL && operation->piid == piid)
        {
            return operation;
        }
    }

    return NULL;
}

static ArgumentOperation * ActionOperation_Ensure(TinyList *operations, uint16_t piid)
{
    ArgumentOperation *operation = NULL;
    uint32_t size = 0;
    uint32_t i = 0;

    RETURN_VAL_IF_FAIL(operations, NULL);

    size = TinyList_GetSize(operations);
    for (i = 0; i < size; ++i)
    {
        operation = (ArgumentOperation *) TinyList_GetAt(operations, i);
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

    if (TinyList_AddTail(operations, operation) == NULL)
    {
        ArgumentOperation_Delete(operation);
        return NULL;
    }

    return operation;
}

ArgumentOperation * ActionOperation_EnsureIn(ActionOperation *thiz, uint16_t piid)
{
    RETURN_VAL_IF_FAIL(thiz, NULL);

    return ActionOperation_Ensure(&thiz->in, piid);
}

ArgumentOperation * ActionOperation_EnsureOut(ActionOperation *thiz, uint16_t piid)
{
    RETURN_VAL_IF_FAIL(thiz, NULL);

    return ActionOperation_Ensure(&thiz->out, piid);
}
