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
#include <tinycore/tiny_list.h>
#include <tinyspec/typedef/operation/argument_operation.h>
#include <tinyspec/typedef/xid/action_id.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    ActionId            aid;
    int                 status;
    TinyList            in;                     /* ArgumentOperation* */
    TinyList            out;                    /* ArgumentOperation* */
    bool                argumentsCompact;
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

/** 按 piid 查入参/出参运行值，不存在返回 NULL。 */
TINYSPEC_API
ArgumentOperation * ActionOperation_GetIn(ActionOperation *thiz, uint16_t piid);

TINYSPEC_API
ArgumentOperation * ActionOperation_GetOut(ActionOperation *thiz, uint16_t piid);

/** 按 piid 查入参/出参运行值，不存在则创建一个空参数并加入列表。 */
TINYSPEC_API
ArgumentOperation * ActionOperation_EnsureIn(ActionOperation *thiz, uint16_t piid);

TINYSPEC_API
ArgumentOperation * ActionOperation_EnsureOut(ActionOperation *thiz, uint16_t piid);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_OPERATION_ACTION_OPERATION_H */
