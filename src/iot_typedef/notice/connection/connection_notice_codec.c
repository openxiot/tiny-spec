/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdint.h>
#include <tinycore/json_boolean.h>
#include <tinycore/json_number.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/iot_typedef/notice/connection/connection_notice_codec.h>

static TINYCORE_RET ConnectionStatusCodec_Encode(JsonObject *payload,
                                                 ConnectionStatus *status)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(status, TINY_RET_E_NULL_PTR);

    do
    {
        ret = JsonObject_PutBoolean(payload, "ready", status->ready);
        if (RET_FAILED(ret))
        {
            break;
        }

        /* JsonNumber only carries int32; the wire timestamp is in
         * milliseconds, so it is omitted when it does not fit. */
        if (status->timestamp > 0 && status->timestamp <= INT32_MAX)
        {
            ret = JsonObject_PutInteger(payload, "timestamp",
                                        (int32_t) status->timestamp);
        }
    } while (false);

    return ret;
}

static TINYCORE_RET ConnectionStatusCodec_Decode(ConnectionStatus *status,
                                                 JsonObject *payload)
{
    JsonBoolean *ready = NULL;
    JsonNumber *timestamp = NULL;

    RETURN_VAL_IF_FAIL(status, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);

    ready = JsonObject_GetBoolean(payload, "ready");
    status->ready = ready != NULL && JsonBoolean_GetValue(ready);

    timestamp = JsonObject_GetNumber(payload, "timestamp");
    status->timestamp = 0;
    if (timestamp != NULL && JsonNumber_IsInteger(timestamp))
    {
        status->timestamp = (int64_t) JsonNumber_GetInteger(timestamp);
    }

    return TINY_RET_OK;
}

TINYCORE_RET ConnectionNoticeCodec_Encode(JsonObject *payload, int type,
                                          ConnectionNotice *notice)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(notice, TINY_RET_E_NULL_PTR);

    switch (type)
    {
        case CONNECTION_NOTICE_STATUS_CHANGED:
            ret = ConnectionStatusCodec_Encode(payload, &notice->payload.status);
            break;

        default:
            TINY_LOG_ERROR("type not supported: %d", type);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}

TINYCORE_RET ConnectionNoticeCodec_Decode(ConnectionNotice *notice, int type,
                                          JsonObject *payload)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(notice, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);

    notice->type = (ConnectionNoticeType) type;

    switch (type)
    {
        case CONNECTION_NOTICE_STATUS_CHANGED:
            ret = ConnectionStatusCodec_Decode(&notice->payload.status, payload);
            break;

        default:
            TINY_LOG_ERROR("type not supported: %d", type);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}
