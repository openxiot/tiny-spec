/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/iot_typedef/device/urn/urn_type.h>

#define STRING_UNDEFINED    "undefined"
#define STRING_DEVICE       "device"
#define STRING_SERVICE      "service"
#define STRING_ACTION       "action"
#define STRING_EVENT        "event"
#define STRING_PROPERTY     "property"

UrnType UrnType_Retrieve(const char *string)
{
    if (STR_EQUAL(STRING_DEVICE, string))
    {
        return DEVICE;
    }

    if (STR_EQUAL(STRING_SERVICE, string))
    {
        return SERVICE;
    }

    if (STR_EQUAL(STRING_ACTION, string))
    {
        return ACTION;
    }

    if (STR_EQUAL(STRING_EVENT, string))
    {
        return EVENT;
    }

    if (STR_EQUAL(STRING_PROPERTY, string))
    {
        return PROPERTY;
    }

    return UNDEFINED;
}

const char * UrnType_ToString(UrnType type)
{
    const char *p = NULL;

    switch (type)
    {
        case DEVICE:
            p = STRING_DEVICE;
            break;

        case SERVICE:
            p = STRING_SERVICE;
            break;

        case PROPERTY:
            p = STRING_PROPERTY;
            break;

        case ACTION:
            p = STRING_ACTION;
            break;

        case EVENT:
            p = STRING_EVENT;
            break;

        case UNDEFINED:
            p = STRING_UNDEFINED;
            break;
    }

    return p;
}
