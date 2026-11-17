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
#include <tinyspec/typedef/operation/property_operations.h>
#include <tinyspec/typedef/operation/property_operation.h>

static void OnPropertyOperationDelete(void *data, void *ctx)
{
    (void) ctx;

    PropertyOperation_Delete((PropertyOperation *) data);
}

PropertyOperations * PropertyOperations_New(void)
{
    PropertyOperations *thiz = NULL;

    do
    {
        thiz = (PropertyOperations *)tiny_malloc(sizeof(PropertyOperations));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(PropertyOperations_Construct(thiz)))
        {
            PropertyOperations_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void PropertyOperations_Delete(PropertyOperations *thiz)
{
    PropertyOperations_Dispose(thiz);
    tiny_free(thiz);
}

TinyRet PropertyOperations_Construct(PropertyOperations *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    TinyList_Construct(&thiz->properties);
    TinyList_SetDeleteListener(&thiz->properties, OnPropertyOperationDelete, NULL);

    return TINY_RET_OK;
}

void PropertyOperations_Dispose(PropertyOperations *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyList_Dispose(&thiz->properties);
}

TinyRet PropertyOperations_Copy(PropertyOperations *dst, PropertyOperations *src)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(dst, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(src, TINY_RET_E_NULL_PTR);

    if (src != dst)
    {
        for (uint32_t i = 0; i < src->properties.size; ++i)
        {
            PropertyOperation * o = (PropertyOperation *) TinyList_GetAt(&src->properties, i);
            PropertyOperation * newOperation = PropertyOperation_CopyFrom(o);
            if (newOperation == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            if (TinyList_AddTail(&dst->properties, newOperation) == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }
        }
    }

    return ret;
}
