/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_EVENT_OCCURRED_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_EVENT_OCCURRED_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_value.h>
#include <tinyspec/typedef/xid/event_id.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    EventId             eid;
    JsonArray         * argument;
} DeviceEventOccurred;

TINYSPEC_API
TinyRet DeviceEventOccurred_Construct(DeviceEventOccurred *thiz);

TINYSPEC_API
void DeviceEventOccurred_Dispose(DeviceEventOccurred *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_EVENT_OCCURRED_H */
