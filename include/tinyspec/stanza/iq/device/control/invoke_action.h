/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_DEVICE_CONTROL_INVOKE_ACTION_H
#define TINYSPEC_STANZA_IQ_DEVICE_CONTROL_INVOKE_ACTION_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/operation/action_operation.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    ActionOperation      action;
} QueryInvokeAction;

TINYSPEC_API
TINYCORE_RET QueryInvokeAction_Construct(QueryInvokeAction *thiz);
TINYSPEC_API
void QueryInvokeAction_Dispose(QueryInvokeAction *thiz);

typedef struct
{
    ActionOperation      action;
} ResultInvokeAction;

TINYSPEC_API
TINYCORE_RET ResultInvokeAction_Construct(ResultInvokeAction *thiz);
TINYSPEC_API
void ResultInvokeAction_Dispose(ResultInvokeAction *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_CONTROL_INVOKE_ACTION_H */
