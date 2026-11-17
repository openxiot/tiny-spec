/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_PROPERTY_CHANGED_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_PROPERTY_CHANGED_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_value.h>
#include <tinyspec/typedef/xid/property_id.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    PropertyId             pid;
    JsonValue            * value;
} DevicePropertyChanged;

TINYSPEC_API
TinyRet DevicePropertyChanged_Construct(DevicePropertyChanged *thiz);

TINYSPEC_API
void DevicePropertyChanged_Dispose(DevicePropertyChanged *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_PROPERTY_CHANGED_H */
