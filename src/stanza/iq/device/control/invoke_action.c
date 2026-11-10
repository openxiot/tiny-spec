/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/control/invoke_action.h>

TINYCORE_RET QueryInvokeAction_Construct(QueryInvokeAction *thiz)
{
    return ActionOperation_Construct(&thiz->action);
}

void QueryInvokeAction_Dispose(QueryInvokeAction *thiz)
{
    ActionOperation_Dispose(&thiz->action);
}

TINYCORE_RET ResultInvokeAction_Construct(ResultInvokeAction *thiz)
{
    return ActionOperation_Construct(&thiz->action);
}

void ResultInvokeAction_Dispose(ResultInvokeAction *thiz)
{
    ActionOperation_Dispose(&thiz->action);
}
