/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_UNIT_UNIT_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_UNIT_UNIT_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef enum
{
    NONE                    = 0,
    HOMEKIT_PERCENTAGE      = 1,
    HOMEKIT_CELSIUS         = 2,
    HOMEKIT_LUX             = 3,
    HOMEKIT_UNITLESS        = 4,
    HOMEKIT_SECONDS         = 5,
    HOMEKIT_ARC_DEGREES     = 6,
} Unit;

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_UNIT_UNIT_H */
