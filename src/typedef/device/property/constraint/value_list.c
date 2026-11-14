/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_alloc.h>
#include <tinycore/tiny_log.h>
#include <tinycore/json_number.h>
#include <tinycore/json_array.h>
#include <tinycore/json_string.h>
#include <tinycore/tiny_str_equal.h>
#include <tinycore/json_object.h>
#include <tinyspec/typedef/device/property/constraint/value_list.h>

static void _OnValueDelete(void *data, void *ctx)
{
    (void) ctx;

    JsonValue_Delete((JsonValue *)data);
}

static TINYCORE_RET ValueList_Construct(ValueList *thiz, JsonArray *array)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    do
    {
        TinyList_Construct(&thiz->list);
        TinyList_SetDeleteListener(&thiz->list, _OnValueDelete, thiz);

        if (array != NULL)
        {
            for (uint32_t i = 0; i < array->values.size; ++i)
            {
                JsonValue * value = NULL;
                JsonValue * object = NULL;
                JsonValue * newValue = NULL;
                object = (JsonValue *) TinyList_GetAt(&array->values, i);
                if (object->type != JSON_OBJECT)
                {
                    TINY_LOG_ERROR("JsonValue not a JsonObject");
                    ret = TINY_RET_E_PARAM_INVALID;
                    break;
                }

                value = JsonObject_GetValue(object->data.object, "value");
                if (value == NULL)
                {
                    TINY_LOG_ERROR("item not contains 'value' field");
                    ret = TINY_RET_E_PARAM_INVALID;
                    break;
                }

                newValue = JsonValue_NewFrom(value);
                if (newValue == NULL)
                {
                    TINY_LOG_ERROR("JsonValue_Copy FAILED");
                    ret = TINY_RET_E_OUT_OF_MEMORY;
                    break;
                }

                if (TinyList_AddTail(&thiz->list, newValue) == NULL)
                {
                    ret = TINY_RET_E_NO_MEMORY;
                    TINY_LOG_ERROR("TinyList_AddTail FAILED");
                    JsonValue_Delete(newValue);
                    break;
                }
            }
        }
    } while (false);

    return ret;
}

static void ValueList_Dispose(ValueList *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyList_Dispose(&thiz->list);
}

ValueList* ValueList_New(void)
{
    ValueList *thiz = NULL;

    do
    {
        thiz = (ValueList *)tiny_malloc(sizeof(ValueList));
        if (thiz == NULL)
        {
            TINY_LOG_ERROR("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(ValueList_Construct(thiz, NULL)))
        {
            TINY_LOG_ERROR("ValueList_Construct FAILED");
            ValueList_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

ValueList* ValueList_NewFrom(JsonArray *array)
{
    ValueList *thiz = NULL;

    do
    {
        thiz = (ValueList *)tiny_malloc(sizeof(ValueList));
        if (thiz == NULL)
        {
            TINY_LOG_ERROR("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(ValueList_Construct(thiz, array)))
        {
            TINY_LOG_ERROR("ValueList_Construct FAILED");
            ValueList_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void ValueList_Delete(ValueList *thiz)
{
    RETURN_IF_FAIL(thiz);

    ValueList_Dispose(thiz);
    tiny_free(thiz);
}

TINYCORE_RET ValueList_Put(ValueList *thiz, JsonValue *value)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(value, TINY_RET_E_NULL_PTR);

    do
    {
        JsonValue *newValue = JsonValue_NewFrom(value);
        if (newValue == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        if (TinyList_AddTail(&thiz->list, newValue) == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            JsonValue_Delete(newValue);
            break;
        }
    } while (false);

    return  ret;
}

static bool ValueList_CheckIntegerValue(ValueList *thiz, JsonNumber *number)
{
    RETURN_VAL_IF_FAIL(thiz, false);
    RETURN_VAL_IF_FAIL(number, false);

    if (number->type != JSON_NUMBER_INTEGER)
    {
        TINY_LOG_ERROR("ValueList_CheckIntegerValue FAILED, number not integer");
        return false;
    }

    TINY_LOG_DEBUG("ValueList_CheckIntegerValue: %d", (int) (number->value.intValue));

    for (uint32_t i = 0; i < thiz->list.size; ++i)
    {
        JsonValue * v = (JsonValue *) TinyList_GetAt(&thiz->list, i);

        if (v->type != JSON_NUMBER)
        {
            continue;
        }

        if (v->data.number->value.intValue == number->value.intValue)
        {
            return true;
        }
    }

    TINY_LOG_ERROR("ValueList_CheckIntegerValue FAILED, invalid value: %d", (int) (number->value.intValue));

    return false;
}

static bool ValueList_CheckStringValue(ValueList *thiz, JsonString *string)
{
    RETURN_VAL_IF_FAIL(thiz, false);
    RETURN_VAL_IF_FAIL(string, false);

    TINY_LOG_DEBUG("ValueList_CheckStringValue: %s", string->value);

    for (uint32_t i = 0; i < thiz->list.size; ++i)
    {
        JsonValue * v = (JsonValue *) TinyList_GetAt(&thiz->list, i);

        if (v->type != JSON_STRING)
        {
            continue;
        }

        if (tiny_str_equal(v->data.string->value, string->value, true))
        {
            return true;
        }
    }

    TINY_LOG_ERROR("ValueList_CheckStringValue FAILED, invalid value: %s", string->value);

    return false;
}

bool ValueList_CheckValue(ValueList *thiz, JsonValue *value)
{
    RETURN_VAL_IF_FAIL(thiz, false);
    RETURN_VAL_IF_FAIL(value, false);

    TINY_LOG_DEBUG("ValueList_CheckValue");

    switch (value->type)
    {
        case JSON_NUMBER:
            return ValueList_CheckIntegerValue(thiz, value->data.number);

        case JSON_STRING:
            return ValueList_CheckStringValue(thiz, value->data.string);

        default:
            TINY_LOG_ERROR("ValueList_CheckValue FAILED, value type error: %d", value->type);
            return false;
    }
}
