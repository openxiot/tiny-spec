/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/iot_typedef/node/node.h>
#include <tinyspec/stanza/iq/node/control/get_nodes.h>

static void OnNodeDelete(void *data, void *ctx)
{
    (void) ctx;

    Node_Delete((Node *) data);
}

TINYCORE_RET ResultGetNodes_Construct(ResultGetNodes *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    TinyList_Construct(&thiz->nodes);
    TinyList_SetDeleteListener(&thiz->nodes, OnNodeDelete, NULL);

    return TINY_RET_OK;
}

void ResultGetNodes_Dispose(ResultGetNodes *thiz)
{
    RETURN_IF_FAIL(thiz);
    TinyList_Dispose(&thiz->nodes);
}
