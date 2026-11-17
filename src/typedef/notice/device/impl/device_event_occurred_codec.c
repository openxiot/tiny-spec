/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/typedef/xid/codec/property_id_codec.h>
#include <tinycore/tiny_log.h>
#include <tinycore/json_array.h>
#include <tinyspec/typedef/xid/codec/event_id_codec.h>
#include <tinyspec/typedef/notice/device/impl/device_event_occurred_codec.h>

/**
 * {
 *      "eid": "x.1.1",
 *      "arguments": [
 *          {
 *              "piid": 1,
 *              "values": ["a", "b", "c"]
 *          }
 *      ]
 * }
 */
TinyRet DeviceEventOccurredCodec_Encode(JsonObject *payload, DeviceEventOccurred *eventOccurred)
{
    TinyRet ret = TINY_RET_OK;
    JsonValue * arguments = NULL;
    JsonArray * argumentCopy = NULL;

    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(eventOccurred, TINY_RET_E_NULL_PTR);

    do
    {
        char eid[IOT_XID_LENGTH];
        EventId_ToString(eid, &eventOccurred->eid);

        ret = JsonObject_PutString(payload, "eid", eid);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("out of memory");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        if (eventOccurred->argument == NULL)
        {
            break;
        }

        argumentCopy = JsonArray_NewFrom(eventOccurred->argument);
        if (argumentCopy == NULL)
        {
            TINY_LOG_ERROR("JsonArray_NewFrom FAILED!");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        arguments = JsonValue_NewArray(argumentCopy);
        if (arguments == NULL)
        {
            TINY_LOG_ERROR("JsonValue_NewArray FAILED!");
            JsonArray_Delete(argumentCopy);
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        ret = JsonObject_AddValue(payload, "arguments", arguments);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("JsonObject_AddValue FAILED!");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        /* AddValue stores a deep copy; release the temporary wrapper */
        JsonValue_Delete(arguments);
        arguments = NULL;
    } while (false);

    if (RET_FAILED(ret))
    {
        if (arguments != NULL)
        {
            JsonValue_Delete(arguments);
        }
    }

    return ret;
}

TinyRet DeviceEventOccurredCodec_Decode(DeviceEventOccurred *eventOccurred, JsonObject *payload)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(eventOccurred, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *eid = JsonObject_GetString(payload, "eid");
        if (eid == NULL)
        {
            TINY_LOG_ERROR("eid not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = EventId_FromString(&eventOccurred->eid, eid->value);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("eid invalid: %s", eid->value);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonArray *arguments = JsonObject_GetArray(payload, "arguments");
        if (arguments == NULL)
        {
            break;
        }

        eventOccurred->argument = JsonArray_NewFrom(arguments);
        if (eventOccurred->argument == NULL)
        {
            TINY_LOG_ERROR("JsonValue_NewFrom failed");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }
    } while (false);

    return ret;
}
