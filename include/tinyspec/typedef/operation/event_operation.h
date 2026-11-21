/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_OPERATION_EVENT_OPERATION_H
#define TINYSPEC_IOT_TYPEDEF_OPERATION_EVENT_OPERATION_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/tiny_list.h>
#include <tinyspec/typedef/operation/argument_operation.h>
#include <tinyspec/typedef/xid/event_id.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    EventId             eid;
    int                 status;
    TinyList            arguments;      /* ArgumentOperation* */
    bool                argumentsCompact;
} EventOperation;

TINYSPEC_API
EventOperation * EventOperation_New(void);

TINYSPEC_API
void EventOperation_Delete(EventOperation *thiz);

TINYSPEC_API
TinyRet EventOperation_Construct(EventOperation *thiz);

TINYSPEC_API
void EventOperation_Dispose(EventOperation *thiz);

TINYSPEC_API
EventOperation * EventOperation_CopyFrom(EventOperation *other);

TINYSPEC_API
TinyRet EventOperation_Copy(EventOperation *dst, EventOperation *src);

/** 按 piid 查参数运行值，不存在返回 NULL。 */
TINYSPEC_API
ArgumentOperation * EventOperation_GetArgument(EventOperation *thiz, uint16_t piid);

/** 按 piid 查参数运行值，不存在则创建一个空参数并加入列表。 */
TINYSPEC_API
ArgumentOperation * EventOperation_EnsureArgument(EventOperation *thiz, uint16_t piid);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_OPERATION_EVENT_OPERATION_H */
