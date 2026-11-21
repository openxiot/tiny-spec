/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_log.h>
#include <tinycore/tiny_alloc.h>
#include <tinyspec/typedef/device/argument.h>

static TinyRet Argument_Construct(Argument *thiz, uint16_t iid)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    thiz->iid = iid;
    thiz->minRepeat = 1;
    thiz->maxRepeat = 1;

    return TINY_RET_OK;
}

static void Argument_Dispose(Argument *thiz)
{
    RETURN_IF_FAIL(thiz);
}

Argument* Argument_NewValue(uint16_t iid)
{
    Argument *thiz = NULL;

    do
    {
        thiz = (Argument *)tiny_malloc(sizeof(Argument));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(Argument_Construct(thiz, iid)))
        {
            Argument_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

Argument* Argument_New(void)
{
    return Argument_NewValue(0);
}

void Argument_Delete(Argument *thiz)
{
    RETURN_IF_FAIL(thiz);

    Argument_Dispose(thiz);
    tiny_free(thiz);
}
