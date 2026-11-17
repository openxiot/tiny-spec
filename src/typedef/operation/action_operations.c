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
#include <tinyspec/typedef/operation/action_operations.h>
#include <tinyspec/typedef/operation/action_operation.h>

static void OnActionOperationDelete(void *data, void *ctx)
{
    (void) ctx;

    ActionOperation_Delete((ActionOperation *) data);
}

ActionOperations * ActionOperations_New(void)
{
    ActionOperations *thiz = NULL;

    do
    {
        thiz = (ActionOperations *)tiny_malloc(sizeof(ActionOperations));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(ActionOperations_Construct(thiz)))
        {
            ActionOperations_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void ActionOperations_Delete(ActionOperations *thiz)
{
    ActionOperations_Dispose(thiz);
    tiny_free(thiz);
}

TinyRet ActionOperations_Construct(ActionOperations *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    TinyList_Construct(&thiz->actions);
    TinyList_SetDeleteListener(&thiz->actions, OnActionOperationDelete, NULL);

    return TINY_RET_OK;
}

void ActionOperations_Dispose(ActionOperations *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyList_Dispose(&thiz->actions);
}

TinyRet ActionOperations_Copy(ActionOperations *dst, ActionOperations *src)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(dst, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(src, TINY_RET_E_NULL_PTR);

    if (src != dst)
    {
        for (uint32_t i = 0; i < src->actions.size; ++i)
        {
            ActionOperation * o = (ActionOperation *) TinyList_GetAt(&src->actions, i);
            ActionOperation * newOperation = ActionOperation_New();
            if (newOperation == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            if (RET_FAILED(ActionOperation_Copy(newOperation, o)))
            {
                ActionOperation_Delete(newOperation);
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            if (TinyList_AddTail(&dst->actions, newOperation) == NULL)
            {
                ActionOperation_Delete(newOperation);
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }
        }
    }

    return ret;
}