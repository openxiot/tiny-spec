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
#include <tinyspec/typedef/operation/action_operation.h>
#include <tinyspec/json_util.h>

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

TINYCORE_RET ActionOperation_Construct(ActionOperation *thiz)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(ActionOperation));

    return ret;
}

void ActionOperation_Dispose(ActionOperation *thiz)
{
    RETURN_IF_FAIL(thiz);

    JsonArray_Dispose(&thiz->in);
    JsonArray_Dispose(&thiz->out);
}

TINYCORE_RET ActionOperation_Copy(ActionOperation *dst, ActionOperation *src)
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

        strncpy(dst->aid.did, src->aid.did, IOT_DEVICE_ID_LENGTH);
        dst->aid.siid = src->aid.siid;
        dst->aid.iid = src->aid.iid;
        dst->status = src->status;

        if (dst->in.values.size != 0)
        {
            JsonArray_Dispose(&dst->in);
        }

        if (dst->out.values.size != 0)
        {
            JsonArray_Dispose(&dst->out);
        }

        ret = JsonArray_CopyInto(&dst->in, &src->in);
        if (RET_FAILED(ret))
        {
            break;
        }

        ret = JsonArray_CopyInto(&dst->out, &src->out);
        if (RET_FAILED(ret))
        {
            break;
        }
    } while (false);

    return ret;
}
