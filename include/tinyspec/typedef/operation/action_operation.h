/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_OPERATION_ACTION_OPERATION_H
#define TINYSPEC_IOT_TYPEDEF_OPERATION_ACTION_OPERATION_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_array.h>
#include <tinyspec/typedef/xid/action_id.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    ActionId                aid;
    int                     status;
    JsonArray               in;
    JsonArray               out;
} ActionOperation;

TINYSPEC_API
ActionOperation * ActionOperation_New(void);

TINYSPEC_API
void ActionOperation_Delete(ActionOperation *thiz);

TINYSPEC_API
TinyRet ActionOperation_Construct(ActionOperation *thiz);

TINYSPEC_API
void ActionOperation_Dispose(ActionOperation *thiz);

TINYSPEC_API
TinyRet ActionOperation_Copy(ActionOperation *dst, ActionOperation *src);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_OPERATION_ACTION_OPERATION_H */
