/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_FORMAT_FORMAT_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_FORMAT_FORMAT_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef enum
{
    FORMAT_UNDEFINED  = 0,
    FORMAT_BOOL       = 1,
    FORMAT_STRING     = 2,
    FORMAT_FLOAT      = 3,
    FORMAT_UINT8      = 4,
    FORMAT_UINT16     = 5,
    FORMAT_UINT32     = 6,
    FORMAT_INT8       = 7,
    FORMAT_INT16      = 8,
    FORMAT_INT32      = 9,
    FORMAT_INT64      = 10,
    FORMAT_HEX        = 11,
    FORMAT_TLV8       = 12,
    FORMAT_INT        = 13,
    FORMAT_COMBINATION = 14,
} Format;

TINYSPEC_API
const char * Format_ToString(Format type);

TINYSPEC_API
Format Format_FromString(const char *value);

TINYSPEC_API
bool Format_CheckInteger(Format format, long value);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_FORMAT_FORMAT_H */
