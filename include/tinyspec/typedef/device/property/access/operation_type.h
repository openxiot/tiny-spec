/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_ACCESS_OPERATION_TYPE_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_ACCESS_OPERATION_TYPE_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef enum
{
    OPERATION_GET       = 1,
    OPERATION_SET       = 2,
    OPERATION_SUB       = 3,
    OPERATION_UNSUB     = 4,
} OperationType;

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_ACCESS_OPERATION_TYPE_H */
