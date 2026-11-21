/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/json_array.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/typedef/xid/codec/property_id_codec.h>
#include <tinyspec/codec/iq/device/control/get_property_codec.h>

bool GetPropertyCodec_EncodeQuery(QueryGetProperty *query, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(query, false);
    RETURN_VAL_IF_FAIL(content, false);

    do
    {
        char pid[IOT_XID_LENGTH];

        PropertyId_ToString(pid, &query->property.pid);

        if (RET_FAILED(JsonObject_PutString(content, "pid", pid)))
        {
            break;
        }

        done = true;
    } while (false);

    return done;
}

TinyRet GetPropertyCodec_DecodeQuery(JsonObject *content, QueryGetProperty *query)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(query, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *string = JsonObject_GetString(content, "pid");
        if (string == NULL)
        {
            TINY_LOG_ERROR("pid not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = PropertyId_FromString(&query->property.pid, string->value);
        if (RET_FAILED(ret))
        {
            break;
        }
    } while (false);

    return ret;
}

bool GetPropertyCodec_EncodeResult(ResultGetProperty *result, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(content, false);
    RETURN_VAL_IF_FAIL(result, false);

    do
    {
        JsonObject *object = JsonObject_New();
        if (object == NULL)
        {
            break;
        }

        char pid[IOT_XID_LENGTH];

        PropertyId_ToString(pid, &result->property.pid);

        TinyRet ret = JsonObject_PutString(object, "pid", pid);
        if (RET_FAILED(ret))
        {
            JsonObject_Delete(object);
            break;
        }

        if (result->property.status == 0)
        {
            JsonValue *wire = PropertyOperation_NewWireValue(&result->property);
            if (wire == NULL)
            {
                JsonObject_Delete(object);
                break;
            }

            ret = JsonObject_AddValue(object, "value", wire);
            JsonValue_Delete(wire);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }
        }
        else
        {
            ret = JsonObject_PutInteger(object, "status", result->property.status);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }
        }

        ret = JsonObject_PutObject(content, "property", object);
        if (RET_FAILED(ret))
        {
            JsonObject_Delete(object);
            break;
        }

        done = true;
    } while (false);

    return done;
}

TinyRet GetPropertyCodec_DecodeResult(JsonObject *content, ResultGetProperty *result)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(result, TINY_RET_E_NULL_PTR);

    do
    {
        JsonObject *object = JsonObject_GetObject(content, "property");
        if (object == NULL)
        {
            TINY_LOG_ERROR("property not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonString *pid = JsonObject_GetString(object, "pid");
        if (pid == NULL)
        {
            TINY_LOG_ERROR("pid not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = PropertyOperation_Construct(&result->property);
        if (RET_FAILED(ret))
        {
            break;
        }

        ret = PropertyId_FromString(&result->property.pid, pid->value);
        if (RET_FAILED(ret))
        {
            result->property.status = -100;
        }

        if (JsonObject_ContainsKey(object, "status"))
        {
            JsonNumber *status = JsonObject_GetNumber(object, "status");
            if (status == NULL)
            {
                TINY_LOG_ERROR("status not found");
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }

            if (status->type != JSON_NUMBER_INTEGER)
            {
                TINY_LOG_ERROR("status not integer");
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }

            result->property.status = (int) (status->value.intValue);
        }
        else
        {
            result->property.status = 0;
        }

        if (result->property.status == 0)
        {
            JsonValue *value = JsonObject_GetValue(object, "value");
            if (value == NULL)
            {
                TINY_LOG_ERROR("value not found");
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }

            ret = PropertyOperation_SetWireValue(&result->property, value);
            if (RET_FAILED(ret))
            {
                TINY_LOG_ERROR("PropertyOperation_SetWireValue failed");
                break;
            }
        }
    } while (false);

    return ret;
}
