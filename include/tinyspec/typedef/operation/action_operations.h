/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_OPERATION_ACTION_OPERATIONS_H
#define TINYSPEC_IOT_TYPEDEF_OPERATION_ACTION_OPERATIONS_H

#include <tinycore/tiny_list.h>
#include <tinyspec/typedef/xid/action_id.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    TinyList        actions;  /* ActionOperation* */
} ActionOperations;

TINYSPEC_API
ActionOperations * ActionOperations_New(void);

TINYSPEC_API
void ActionOperations_Delete(ActionOperations *thiz);

TINYSPEC_API
TINYCORE_RET ActionOperations_Construct(ActionOperations *thiz);

TINYSPEC_API
void ActionOperations_Dispose(ActionOperations *thiz);

TINYSPEC_API
TINYCORE_RET ActionOperations_Copy(ActionOperations *dst, ActionOperations *src);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_OPERATION_ACTION_OPERATIONS_H */