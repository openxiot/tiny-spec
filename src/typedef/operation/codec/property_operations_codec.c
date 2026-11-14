/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/typedef/operation/codec/property_operations_codec.h>
#include <tinycore/json_array.h>
#include <tinyspec/typedef/operation/property_operation.h>
#include <tinyspec/typedef/xid/codec/property_id_codec.h>
#include <tinyspec/typedef/status/iot_status.h>
#include <tinycore/tiny_log.h>

bool PropertyOperationsCodec_Set_Query_Encode(PropertyOperations *operations, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(content, false);
    RETURN_VAL_IF_FAIL(operations, false);

    do
    {
        TINYCORE_RET ret = TINY_RET_OK;

        JsonArray *array = JsonArray_New();
        if (array == NULL)
        {
            break;
        }

        for (uint32_t i = 0; i < operations->properties.size; ++i)
        {
            PropertyOperation *o = (PropertyOperation *) TinyList_GetAt(&operations->properties, i);
            JsonObject *object = NULL;
            char pid[IOT_XID_LENGTH];

            object = JsonObject_New();
            if (object == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            PropertyId_ToString(pid, &o->pid);

            ret = JsonObject_PutString(object, "pid", pid);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }

            ret = JsonObject_AddValue(object, "value", o->value);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }

            ret = JsonArray_AddObject(array, object);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }
        }

        if (RET_FAILED(ret))
        {
            JsonArray_Delete(array);
            break;
        }

        if (RET_FAILED(JsonObject_PutArray(content, "properties", array)))
        {
            JsonArray_Delete(array);
            break;
        }

        done = true;
    } while (false);

    return done;
}

TINYCORE_RET PropertyOperationsCodec_Set_Query_Decode(JsonObject *content, PropertyOperations *operations)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(operations, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonArray *array = JsonObject_GetArray(content, "properties");
        if (array == NULL)
        {
            TINY_LOG_ERROR("operations not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = PropertyOperations_Construct(operations);
        if (RET_FAILED(ret))
        {
            break;
        }

        for (uint32_t i = 0; i < array->values.size; ++i)
        {
            PropertyOperation *operation = NULL;
            JsonValue *item = (JsonValue *) TinyList_GetAt(&array->values, i);
            JsonObject *object = NULL;
            JsonString *pid = NULL;
            JsonValue *value = NULL;

            if (item->type != JSON_OBJECT)
            {
                TINY_LOG_ERROR("invalid content");
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }
            object = item->data.object;

            operation = PropertyOperation_New();
            if (operation == NULL)
            {
                TINY_LOG_ERROR("PropertyOperation_New failed");
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            pid = JsonObject_GetString(object, "pid");
            if (pid == NULL)
            {
                TINY_LOG_ERROR("invalid content");
                ret = TINY_RET_E_PARAM_INVALID;
                PropertyOperation_Delete(operation);
                break;
            }

            value = JsonObject_GetValue(object, "value");
            if (value == NULL)
            {
                TINY_LOG_ERROR("invalid content");
                ret = TINY_RET_E_PARAM_INVALID;
                PropertyOperation_Delete(operation);
                break;
            }

            ret = PropertyId_FromString(&operation->pid, pid->value);
            if (RET_FAILED(ret))
            {
                operation->status = -100;
            }
            else
            {
                operation->value = JsonValue_NewFrom(value);
                if (operation->value == NULL)
                {
                    TINY_LOG_ERROR("JsonValue_Copy failed");
                    ret = TINY_RET_E_NO_MEMORY;
                    PropertyOperation_Delete(operation);
                    break;
                }
            }

            if (TinyList_AddTail(&operations->properties, operation) == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                TINY_LOG_ERROR("TinyList_AddTail failed");
                PropertyOperation_Delete(operation);
                break;
            }
        }
    } while (false);

    return ret;
}

bool PropertyOperationsCodec_Set_Result_Encode(PropertyOperations *operations, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(operations, false);
    RETURN_VAL_IF_FAIL(content, false);

    do
    {
        TINYCORE_RET ret = TINY_RET_OK;

        JsonArray *array = JsonArray_New();
        if (array == NULL)
        {
            break;
        }

        for (uint32_t i = 0; i < operations->properties.size; ++i)
        {
            PropertyOperation *o = (PropertyOperation *) TinyList_GetAt(&operations->properties, i);
            JsonObject *object = NULL;
            char pid[IOT_XID_LENGTH];

            object = JsonObject_New();
            if (object == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            PropertyId_ToString(pid, &o->pid);

            ret = JsonObject_PutString(object, "pid", pid);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }

            ret = JsonObject_PutInteger(object, "status", o->status);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }

            if (o->status != 0)
            {
                ret = JsonObject_PutString(object, "description", o->description);
                if (RET_FAILED(ret))
                {
                    JsonObject_Delete(object);
                    break;
                }
            }

            ret = JsonArray_AddObject(array, object);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }
        }

        if (RET_FAILED(ret))
        {
            JsonArray_Delete(array);
            break;
        }

        if (RET_FAILED(JsonObject_PutArray(content, "properties", array)))
        {
            JsonArray_Delete(array);
            break;
        }

        done = true;
    } while (false);

    return done;
}

TINYCORE_RET PropertyOperationsCodec_Set_Result_Decode(JsonObject *content, PropertyOperations *operations)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(operations, TINY_RET_E_NULL_PTR);

    do
    {
        JsonArray *array = JsonObject_GetArray(content, "properties");
        if (array == NULL)
        {
            TINY_LOG_ERROR("properties not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = PropertyOperations_Construct(operations);
        if (RET_FAILED(ret))
        {
            break;
        }

        for (uint32_t i = 0; i < array->values.size; ++i)
        {
            PropertyOperation *property = NULL;
            JsonValue *item = (JsonValue *) TinyList_GetAt(&array->values, i);
            JsonObject *object = NULL;
            JsonString *pid = NULL;

            if (item->type != JSON_OBJECT)
            {
                TINY_LOG_ERROR("invalid content");
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }
            object = item->data.object;

            property = PropertyOperation_New();
            if (property == NULL)
            {
                TINY_LOG_ERROR("PropertyOperation_New failed");
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            pid = JsonObject_GetString(object, "pid");
            if (pid == NULL)
            {
                TINY_LOG_ERROR("pid not found");
                ret = TINY_RET_E_PARAM_INVALID;
                PropertyOperation_Delete(property);
                break;
            }

            PropertyId_FromString(&property->pid, pid->value);

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

                property->status = (int) (status->value.intValue);
            }
            else
            {
                property->status = 0;
            }

            if (JsonObject_ContainsKey(object, "description"))
            {
                JsonString *description = JsonObject_GetString(object, "description");
                if (description != NULL)
                {
                    strncpy(property->description, description->value, IOT_DESCRIPTION_LENGTH);
                }
            }

            if (TinyList_AddTail(&operations->properties, property) == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                TINY_LOG_ERROR("TinyList_AddTail failed");
                PropertyOperation_Delete(property);
                break;
            }
        }
    } while (false);

    return ret;
}

bool PropertyOperationsCodec_Get_Query_Encode(PropertyOperations *operations, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(content, false);
    RETURN_VAL_IF_FAIL(operations, false);

    do
    {
        TINYCORE_RET ret = TINY_RET_OK;

        JsonArray *array = JsonArray_New();
        if (array == NULL)
        {
            break;
        }

        for (uint32_t i = 0; i < operations->properties.size; ++i)
        {
            char pid[IOT_XID_LENGTH];
            PropertyOperation *o = TinyList_GetAt(&operations->properties, i);

            XID3_ToString(pid, &o->pid);

            ret = JsonArray_AddString(array, pid);
            if (RET_FAILED(ret))
            {
                break;
            }
        }

        if (RET_FAILED(ret))
        {
            JsonArray_Delete(array);
            break;
        }

        if (RET_FAILED(JsonObject_PutArray(content, "properties", array)))
        {
            JsonArray_Delete(array);
            break;
        }

        done = true;
    } while (false);

    return done;
}

TINYCORE_RET PropertyOperationsCodec_Get_Query_Decode(JsonObject *content, PropertyOperations *operations)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(operations, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonArray *array = JsonObject_GetArray(content, "properties");
        if (array == NULL)
        {
            TINY_LOG_ERROR("properties not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = PropertyOperations_Construct(operations);
        if (RET_FAILED(ret))
        {
            break;
        }

        for (uint32_t i = 0; i < array->values.size; ++i)
        {
            PropertyOperation *operation = NULL;
            JsonValue *value = (JsonValue *) TinyList_GetAt(&array->values, i);
            if (value->type != JSON_STRING)
            {
                TINY_LOG_ERROR("invalid content");
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }

            operation = PropertyOperation_New();
            if (operation == NULL)
            {
                TINY_LOG_ERROR("PropertyOperation_New failed");
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            ret = PropertyId_FromString(&operation->pid, value->data.string->value);
            if (RET_FAILED(ret))
            {
                operation->status = IOT_STATUS_PID_INVALID;
            }

            if (TinyList_AddTail(&operations->properties, operation) == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                TINY_LOG_ERROR("TinyList_AddTail failed");
                PropertyOperation_Delete(operation);
                break;
            }
        }
    } while (false);

    return ret;
}

bool PropertyOperationsCodec_Get_Result_Encode(PropertyOperations *operations, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(content, false);
    RETURN_VAL_IF_FAIL(operations, false);

    do
    {
        TINYCORE_RET ret = TINY_RET_OK;

        JsonArray *array = JsonArray_New();
        if (array == NULL)
        {
            break;
        }

        for (uint32_t i = 0; i < operations->properties.size; ++i)
        {
            PropertyOperation *o = (PropertyOperation *) TinyList_GetAt(&operations->properties, i);
            JsonObject *object = NULL;
            char pid[IOT_XID_LENGTH];

            object = JsonObject_New();
            if (object == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            PropertyId_ToString(pid, &o->pid);

            ret = JsonObject_PutString(object, "pid", pid);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }

            if (o->status == 0)
            {
                if (o->value == NULL)
                {
                    JsonObject_Delete(object);
                    continue;
                }

                ret = JsonObject_AddValue(object, "value", o->value);
                if (RET_FAILED(ret))
                {
                    JsonObject_Delete(object);
                    break;
                }
            }
            else
            {
                ret = JsonObject_PutInteger(object, "status", o->status);
                if (RET_FAILED(ret))
                {
                    JsonObject_Delete(object);
                    break;
                }

                ret = JsonObject_PutString(object, "description", o->description);
                if (RET_FAILED(ret))
                {
                    JsonObject_Delete(object);
                    break;
                }
            }

            ret = JsonArray_AddObject(array, object);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }
        }

        if (RET_FAILED(ret))
        {
            JsonArray_Delete(array);
            break;
        }

        if (RET_FAILED(JsonObject_PutArray(content, "properties", array)))
        {
            JsonArray_Delete(array);
            break;
        }

        done = true;
    } while (false);

    return done;
}

TINYCORE_RET PropertyOperationsCodec_Get_Result_Decode(JsonObject *content, PropertyOperations *operations)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(operations, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonArray *array = JsonObject_GetArray(content, "properties");
        if (array == NULL)
        {
            TINY_LOG_ERROR("properties not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = PropertyOperations_Construct(operations);
        if (RET_FAILED(ret))
        {
            break;
        }

        for (uint32_t i = 0; i < array->values.size; ++i)
        {
            PropertyOperation *property = NULL;
            JsonValue *item = (JsonValue *) TinyList_GetAt(&array->values, i);
            JsonObject *object = NULL;
            JsonString *pid = NULL;

            if (item->type != JSON_OBJECT)
            {
                TINY_LOG_ERROR("invalid content");
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }
            object = item->data.object;

            property = PropertyOperation_New();
            if (property == NULL)
            {
                TINY_LOG_ERROR("PropertyOperation_New failed");
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            pid = JsonObject_GetString(object, "pid");
            if (pid == NULL)
            {
                TINY_LOG_ERROR("pid not found");
                ret = TINY_RET_E_PARAM_INVALID;
                PropertyOperation_Delete(property);
                break;
            }

            PropertyId_FromString(&property->pid, pid->value);

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

                property->status = (int) (status->value.intValue);
            }
            else
            {
                property->status = 0;
            }

            if (property->status == 0)
            {
                JsonValue *value = JsonObject_GetValue(object, "value");
                if (value == NULL)
                {
                    TINY_LOG_ERROR("value not found");
                    ret = TINY_RET_E_PARAM_INVALID;
                    break;
                }

                property->value = JsonValue_NewFrom(value);
                if (property->value == NULL)
                {
                    TINY_LOG_ERROR("JsonValue_NewFrom failed");
                    ret = TINY_RET_E_NO_MEMORY;
                    break;
                }
            }
            else
            {
                if (JsonObject_ContainsKey(object, "description"))
                {
                    JsonString *description = JsonObject_GetString(object, "description");
                    if (description != NULL)
                    {
                        strncpy(property->description, description->value, IOT_DESCRIPTION_LENGTH);
                    }
                }
            }

            if (TinyList_AddTail(&operations->properties, property) == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                TINY_LOG_ERROR("TinyList_AddTail failed");
                PropertyOperation_Delete(property);
                break;
            }
        }
    } while (false);

    return ret;
}
