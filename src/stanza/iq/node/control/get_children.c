/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/iot_typedef/node/child.h>
#include <tinyspec/stanza/iq/node/control/get_children.h>

static void OnChildDelete(void *data, void *ctx)
{
    (void) ctx;

    Child_Delete((Child *) data);
}

TINYCORE_RET ResultGetChildren_Construct(ResultGetChildren *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    TinyList_Construct(&thiz->children);
    TinyList_SetDeleteListener(&thiz->children, OnChildDelete, NULL);

    return TINY_RET_OK;
}

void ResultGetChildren_Dispose(ResultGetChildren *thiz)
{
    RETURN_IF_FAIL(thiz);
    TinyList_Dispose(&thiz->children);
}
