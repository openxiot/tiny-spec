/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/json_string.h>
#include <tinyspec/iot_typedef/device/property/access/access.h>

Access Access_FromJsonArray(JsonArray *array)
{
    Access access = 0;

    RETURN_VAL_IF_FAIL(array, 0);

    for (uint32_t i = 0; i < array->values.size; ++i)
    {
        JsonValue *v = TinyList_GetAt(&array->values, i);
        if (v->type != JSON_STRING)
        {
            break;
        }

        if (STR_EQUAL("read", v->data.string->value))
        {
            access |= ACCESS_READ;
        }

        else if (STR_EQUAL("write", v->data.string->value))
        {
            access |= ACCESS_WRITE;
        }

        else if (STR_EQUAL("notify", v->data.string->value))
        {
            access |= ACCESS_NOTIFY;
        }
    }

    return access;
}

bool Access_IsReadable(Access access)
{
    return ((access & ACCESS_READ) != 0);
}

bool Access_IsWritable(Access access)
{
    return ((access & ACCESS_WRITE) != 0);
}

bool Access_IsNotifiable(Access access)
{
    return ((access & ACCESS_NOTIFY) != 0);
}
