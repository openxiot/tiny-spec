/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_EVENT_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_EVENT_H

#include <tinycore/tiny_list.h>
#include <tinyspec/typedef/operation/event_operation.h>
#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/device/urn/urn.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    uint16_t            iid;
    Urn                 type;
    TinyList            arguments;
    void *              service;
} Event;

TINYSPEC_API
Event* Event_New(uint16_t iid);

TINYSPEC_API
Event* Event_NewInstance(uint16_t iid, const char *ns, const char *name, uint32_t uuid, const char *vendor);

TINYSPEC_API
void Event_Delete(Event *thiz);

TINYSPEC_API
TinyRet Event_ArgumentAdd(Event *thiz, uint16_t iid);

/** 带 repeat 范围的参数（等价于 spec 的 repeat: [min, max]）。 */
TINYSPEC_API
TinyRet Event_ArgumentAddEx(Event *thiz, uint16_t iid, uint16_t min, uint16_t max);

TINYSPEC_API
void Event_TryProduce(Event *thiz, EventOperation *o);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_EVENT_H */
