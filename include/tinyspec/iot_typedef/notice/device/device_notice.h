/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_DEVICE_NOTICE_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_DEVICE_NOTICE_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/iot_typedef/notice/device/impl/device_event_occurred.h>
#include <tinyspec/iot_typedef/notice/device/impl/device_property_changed.h>
#include <tinyspec/iot_typedef/notice/device/impl/device_properties_changed.h>

TINYSPEC_BEGIN_DECLS

typedef enum
{
    DEVICE_NOTICE_UNDEFINED = 0,
    DEVICE_NOTICE_PROPERTY_CHANGED  = 1,
    DEVICE_NOTICE_EVENT_OCCURRED  = 2,
    DEVICE_NOTICE_PROPERTIES_CHANGED = 3,
    DEVICE_NOTICE_PROPERTIES_NOTIFY = 4,
} DeviceNoticeType;

typedef struct
{
    DeviceNoticeType            type;

    union
    {
        DevicePropertyChanged   propertyChanged;
        DeviceEventOccurred     eventOccurred;
        DevicePropertiesChanged propertiesChanged;
    } payload;
} DeviceNotice;

TINYSPEC_API
DeviceNoticeType device_notice_type_from_string(const char *string);
TINYSPEC_API
const char * device_notice_type_to_string(DeviceNoticeType type);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_DEVICE_NOTICE_H */
