/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_URN_URN_TYPE_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_URN_URN_TYPE_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef enum
{
    UNDEFINED   = 0,
    DEVICE      = 1,
    SERVICE     = 2,
    PROPERTY    = 3,
    ACTION      = 4,
    EVENT       = 5,
} UrnType;

TINYSPEC_API
UrnType UrnType_Retrieve(const char *string);

TINYSPEC_API
const char * UrnType_ToString(UrnType type);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_URN_URN_TYPE_H */
