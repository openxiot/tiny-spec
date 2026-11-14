/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_ACCESS_ACCESS_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_ACCESS_ACCESS_H

#include <tinycore/json_array.h>
#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef uint8_t Access;

#define ACCESS_READ         ((uint8_t)0x01)
#define ACCESS_WRITE        ((uint8_t)0x02)
#define ACCESS_NOTIFY       ((uint8_t)0x04)
#define ACCESS_RW           ((Access)((uint32_t)ACCESS_READ | (uint32_t)ACCESS_WRITE))
#define ACCESS_RN           ((Access)((uint32_t)ACCESS_READ | (uint32_t)ACCESS_NOTIFY))
#define ACCESS_WN           ((Access)((uint32_t)ACCESS_WRITE | (uint32_t)ACCESS_NOTIFY))
#define ACCESS_RWN          ((Access)(((uint32_t)ACCESS_READ | (uint32_t)ACCESS_WRITE) | (uint32_t)ACCESS_NOTIFY))

TINYSPEC_API
Access Access_FromJsonArray(JsonArray *array);

TINYSPEC_API
bool Access_IsReadable(Access access);

TINYSPEC_API
bool Access_IsWritable(Access access);

TINYSPEC_API
bool Access_IsNotifiable(Access access);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_ACCESS_ACCESS_H */
