/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_log.h>
#include <tinyspec/iot_typedef/notice/device/impl/device_event_occurred_codec.h>
#include <tinyspec/iot_typedef/notice/device/device_notice_codec.h>
#include <tinyspec/iot_typedef/notice/device/impl/device_property_changed_codec.h>

TINYCORE_RET DeviceNoticeCodec_Encode(JsonObject *payload, int type, DeviceNotice *notice)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(notice, TINY_RET_E_NULL_PTR);

    switch (type)
    {
        case DEVICE_NOTICE_PROPERTY_CHANGED:
            ret = DevicePropertyChangedCodec_Encode(payload, & notice->payload.propertyChanged);
            break;

        case DEVICE_NOTICE_EVENT_OCCURRED:
            ret = DeviceEventOccurredCodec_Encode(payload, & notice->payload.eventOccurred);
            break;

        default:
            TINY_LOG_ERROR("type not supported: %d", type);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}

TINYCORE_RET DeviceNoticeCodec_Decode(DeviceNotice *notice, int type, JsonObject *payload)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(notice, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);

    switch (type)
    {
        case DEVICE_NOTICE_PROPERTY_CHANGED:
            ret = DevicePropertyChangedCodec_Decode(&notice->payload.propertyChanged, payload);
            break;

        case DEVICE_NOTICE_EVENT_OCCURRED:
            ret = DeviceEventOccurredCodec_Decode(&notice->payload.eventOccurred, payload);
            break;

        default:
            TINY_LOG_ERROR("type not supported: %d", type);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}
