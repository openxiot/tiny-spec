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
#include <tinyspec/typedef/notice/device/impl/device_property_changed_codec.h>

TINYCORE_RET DevicePropertyChangedCodec_Encode(JsonObject *payload, DevicePropertyChanged *propertyChanged)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(propertyChanged, TINY_RET_E_NULL_PTR);

    do
    {
        char pid[IOT_XID_LENGTH];
        PropertyId_ToString(pid, &propertyChanged->pid);

        ret = JsonObject_PutString(payload, "pid", pid);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("out of memory");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        ret = JsonObject_AddValue(payload, "value", propertyChanged->value);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("out of memory");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }
    } while (false);

    return ret;
}

TINYCORE_RET DevicePropertyChangedCodec_Decode(DevicePropertyChanged *propertyChanged, JsonObject *payload)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(propertyChanged, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *pid = JsonObject_GetString(payload, "pid");
        if (pid == NULL)
        {
            TINY_LOG_ERROR("pid not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonValue *value = JsonObject_GetValue(payload, "value");
        if (value == NULL)
        {
            TINY_LOG_ERROR("value not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = PropertyId_FromString(&propertyChanged->pid, pid->value);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("pid invalid: %s", pid->value);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        propertyChanged->value = JsonValue_NewFrom(value);
        if (propertyChanged->value == NULL)
        {
            TINY_LOG_ERROR("JsonValue_NewFrom failed");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }
    } while (false);

    return ret;
}
