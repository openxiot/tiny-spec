/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/typedef/notice/device/device_notice.h>

#define STRING_DEVICE_NOTICE_UNDEFINED            "undefined"
#define STRING_DEVICE_NOTICE_PROPERTY_CHANGED     "property-changed"
#define STRING_DEVICE_EVENT_OCCURRED_CHANGED      "event-occurred"

DeviceNoticeType device_notice_type_from_string(const char *string)
{
    RETURN_VAL_IF_FAIL(string, DEVICE_NOTICE_UNDEFINED);

    if (STR_EQUAL(STRING_DEVICE_NOTICE_PROPERTY_CHANGED, string))
    {
        return DEVICE_NOTICE_PROPERTY_CHANGED;
    }

    if (STR_EQUAL(STRING_DEVICE_EVENT_OCCURRED_CHANGED, string))
    {
        return DEVICE_NOTICE_EVENT_OCCURRED;
    }

    return DEVICE_NOTICE_UNDEFINED;
}

const char * device_notice_type_to_string(DeviceNoticeType type)
{
    switch (type)
    {
        case DEVICE_NOTICE_PROPERTY_CHANGED:
            return STRING_DEVICE_NOTICE_PROPERTY_CHANGED;

        case DEVICE_NOTICE_EVENT_OCCURRED:
            return STRING_DEVICE_EVENT_OCCURRED_CHANGED;

        default:
            return STRING_DEVICE_NOTICE_UNDEFINED;
    }
}
