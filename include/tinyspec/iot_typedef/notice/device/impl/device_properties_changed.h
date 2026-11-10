/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_PROPERTIES_CHANGED_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_PROPERTIES_CHANGED_H

#include <tinycore/tiny_list.h>
#include <tinyspec/tinyspec_base.h>
#include <tinyspec/iot_typedef/notice/device/impl/device_property_changed.h>

TINYSPEC_BEGIN_DECLS

/* Batch form of property-changed / properties-notify.  The list owns
 * DevicePropertyChanged instances added through _Add(). */
typedef struct
{
    TinyList    properties;
    int64_t     timestamp;
} DevicePropertiesChanged;

TINYSPEC_API
DevicePropertiesChanged * DevicePropertiesChanged_New(void);

TINYSPEC_API
void DevicePropertiesChanged_Delete(DevicePropertiesChanged *thiz);

TINYSPEC_API
TINYCORE_RET DevicePropertiesChanged_Construct(DevicePropertiesChanged *thiz);

TINYSPEC_API
void DevicePropertiesChanged_Dispose(DevicePropertiesChanged *thiz);

/* Takes ownership of @p item. */
TINYSPEC_API
TINYCORE_RET DevicePropertiesChanged_Add(DevicePropertiesChanged *thiz,
                                         DevicePropertyChanged *item);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_PROPERTIES_CHANGED_H */
