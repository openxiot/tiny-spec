/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_log.h>
#include <tinyspec/typedef/device/property/format/format.h>

#define FORMAT_STRING_UNDEFINED   "undefined"
#define FORMAT_STRING_BOOL        "bool"
#define FORMAT_STRING_STRING      "string"
#define FORMAT_STRING_FLOAT       "float"
#define FORMAT_STRING_UINT8       "uint8"
#define FORMAT_STRING_UINT16      "uint16"
#define FORMAT_STRING_UINT32      "uint32"
#define FORMAT_STRING_INT8        "int8"
#define FORMAT_STRING_INT16       "int16"
#define FORMAT_STRING_INT32       "int32"
#define FORMAT_STRING_INT64       "int64"
#define FORMAT_STRING_HEX         "hex"
#define FORMAT_STRING_TLV8        "tlv8"
#define FORMAT_STRING_INT         "int"
#define FORMAT_STRING_COMBINATION "combination"

const char * Format_ToString(Format type)
{
    switch (type)
    {
        case FORMAT_BOOL:
            return FORMAT_STRING_BOOL;

        case FORMAT_FLOAT:
            return FORMAT_STRING_FLOAT;

        case FORMAT_STRING:
            return FORMAT_STRING_STRING;

        case FORMAT_UINT8:
            return FORMAT_STRING_UINT8;

        case FORMAT_UINT16:
            return FORMAT_STRING_UINT16;

        case FORMAT_UINT32:
            return FORMAT_STRING_UINT32;

        case FORMAT_INT8:
            return FORMAT_STRING_INT8;

        case FORMAT_INT16:
            return FORMAT_STRING_INT16;

        case FORMAT_INT32:
            return FORMAT_STRING_INT32;

        case FORMAT_INT64:
            return FORMAT_STRING_INT64;

        case FORMAT_HEX:
            return FORMAT_STRING_HEX;

        case FORMAT_TLV8:
            return FORMAT_STRING_TLV8;

        case FORMAT_INT:
            return FORMAT_STRING_INT;

        case FORMAT_COMBINATION:
            return FORMAT_STRING_COMBINATION;

        default:
            break;
    }

    return FORMAT_STRING_UNDEFINED;
}

Format Format_FromString(const char *value)
{
    TINY_LOG_DEBUG("Format_FromString: %s", value);

    if (STR_EQUAL(FORMAT_STRING_BOOL, value))
    {
        return FORMAT_BOOL;
    }

    if (STR_EQUAL(FORMAT_STRING_UINT8, value))
    {
        return FORMAT_UINT8;
    }

    if (STR_EQUAL(FORMAT_STRING_UINT16, value))
    {
        return FORMAT_UINT16;
    }

    if (STR_EQUAL(FORMAT_STRING_UINT32, value))
    {
        return FORMAT_UINT32;
    }

    if (STR_EQUAL(FORMAT_STRING_INT8, value))
    {
        return FORMAT_INT8;
    }

    if (STR_EQUAL(FORMAT_STRING_INT16, value))
    {
        return FORMAT_INT16;
    }

    if (STR_EQUAL(FORMAT_STRING_INT32, value))
    {
        return FORMAT_INT32;
    }

    if (STR_EQUAL(FORMAT_STRING_FLOAT, value))
    {
        return FORMAT_FLOAT;
    }

    if (STR_EQUAL(FORMAT_STRING_INT64, value))
    {
        return FORMAT_INT64;
    }

    if (STR_EQUAL(FORMAT_STRING_STRING, value))
    {
        return FORMAT_STRING;
    }

    if (STR_EQUAL(FORMAT_STRING_HEX, value))
    {
        return FORMAT_HEX;
    }

    if (STR_EQUAL(FORMAT_STRING_TLV8, value))
    {
        return FORMAT_TLV8;
    }

    if (STR_EQUAL(FORMAT_STRING_INT, value))
    {
        return FORMAT_INT;
    }

    if (STR_EQUAL(FORMAT_STRING_COMBINATION, value))
    {
        return FORMAT_COMBINATION;
    }

    return FORMAT_UNDEFINED;
}

bool Format_CheckInteger(Format format, long value)
{
    /*
     * 范围常量一律用 64 位：目标板是 ILP32（long 只有 32 位），
     * 0xFFFFFFFF 赋给 long 会回绕成 -1，导致 uint32 属性任何非负数
     * 都被判非法（宿主机 LP64 上测不出来）。
     */
    int64_t min = 0;
    int64_t max = 0;

    switch (format)
    {
        case FORMAT_UINT8:
            min = 0;
            max = 0xFF;
            break;

        case FORMAT_UINT16:
            min = 0;
            max = 0xFFFF;
            break;

        case FORMAT_UINT32:
            min = 0;
            max = 4294967295LL;   /* UINT32_MAX */
            break;

        case FORMAT_INT8:
            min = 0 - 0x7F;
            max = 0x7F;
            break;

        case FORMAT_INT16:
            min = 0 - 0x7FFF;
            max = 0x7FFF;
            break;

        case FORMAT_INT32:
            min = -2147483648LL;  /* INT32_MIN */
            max = 2147483647LL;   /* INT32_MAX */
            break;

        case FORMAT_INT64:
            return true;

        case FORMAT_INT:
            return true;

        default:
            return false;
    }

    return ((int64_t)value >= min && (int64_t)value <= max);
}
