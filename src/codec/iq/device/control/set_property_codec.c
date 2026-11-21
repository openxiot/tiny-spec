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
#include <tinyspec/typedef/operation/property_operation.h>
#include <tinyspec/typedef/xid/codec/property_id_codec.h>
#include <tinyspec/codec/iq/device/control/set_property_codec.h>

bool SetPropertyCodec_EncodeQuery(QuerySetProperty *query, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(query, false);
    RETURN_VAL_IF_FAIL(content, false);

    do
    {
        JsonObject *object = JsonObject_New();
        if (object == NULL)
        {
            break;
        }

        char pid[IOT_XID_LENGTH];

        PropertyId_ToString(pid, &query->property.pid);

        if (RET_FAILED(JsonObject_PutString(object, "pid", pid)))
        {
            JsonObject_Delete(object);
            break;
        }

        {
            JsonValue *wire = PropertyOperation_NewWireValue(&query->property);
            if (wire == NULL)
            {
                JsonObject_Delete(object);
                break;
            }

            if (RET_FAILED(JsonObject_AddValue(object, "value", wire)))
            {
                JsonValue_Delete(wire);
                JsonObject_Delete(object);
                break;
            }

            JsonValue_Delete(wire);
        }

        if (RET_FAILED(JsonObject_PutObject(content, "property", object)))
        {
            JsonObject_Delete(object);
            break;
        }

        done = true;
    } while (false);

    return done;
}

TinyRet SetPropertyCodec_DecodeQuery(JsonObject *content, QuerySetProperty *query)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(query, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonObject *object = JsonObject_GetObject(content, "property");
        if (object == NULL)
        {
            TINY_LOG_ERROR("property not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = PropertyOperation_Construct(&query->property);
        if (RET_FAILED(ret))
        {
            break;
        }

        JsonString *pid = NULL;
        JsonValue *value = NULL;

        pid = JsonObject_GetString(object, "pid");
        if (pid == NULL)
        {
            TINY_LOG_ERROR("pid not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        value = JsonObject_GetValue(object, "value");
        if (value == NULL)
        {
            TINY_LOG_ERROR("value not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = PropertyId_FromString(&query->property.pid, pid->value);
        if (RET_FAILED(ret))
        {
            query->property.status = -100;
        }
        else
        {
            ret = PropertyOperation_SetWireValue(&query->property, value);
            if (RET_FAILED(ret))
            {
                TINY_LOG_ERROR("PropertyOperation_SetWireValue failed");
                break;
            }
        }
    } while (false);

    return ret;
}

bool SetPropertyCodec_EncodeResult(ResultSetProperty *result, JsonObject *content)
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

        if (RET_FAILED(JsonObject_PutString(object, "pid", pid)))
        {
            JsonObject_Delete(object);
            break;
        }

        if (RET_FAILED(JsonObject_PutInteger(object, "status", result->property.status)))
        {
            JsonObject_Delete(object);
            break;
        }

        if (RET_FAILED(JsonObject_PutObject(content, "property", object)))
        {
            JsonObject_Delete(object);
            break;
        }

        done = true;
    } while (false);

    return done;
}

TinyRet SetPropertyCodec_DecodeResult(JsonObject *content, ResultSetProperty *result)
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

        result->property.status = (int) (status->value.intValue);
    } while (false);

    return ret;
}
