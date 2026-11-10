/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_PROPERTIES_CHANGED_CODEC_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_PROPERTIES_CHANGED_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/iot_typedef/notice/device/impl/device_properties_changed.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
TINYCORE_RET DevicePropertiesChangedCodec_Encode(JsonObject *payload,
                                                 DevicePropertiesChanged *propertiesChanged);

TINYSPEC_API
TINYCORE_RET DevicePropertiesChangedCodec_Decode(DevicePropertiesChanged *propertiesChanged,
                                                 JsonObject *payload);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_DEVICE_IMPL_DEVICE_PROPERTIES_CHANGED_CODEC_H */
