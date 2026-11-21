/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdlib.h>
#include <string.h>
#include <tinycore/json_number.h>
#include <tinycore/json_object.h>
#include <tinycore/tiny_alloc.h>
#include <tinycore/tiny_log.h>
#include <tinycore/tiny_map.h>
#include <tinycore/tiny_snprintf.h>
#include <tinyspec/typedef/operation/argument_operation.h>

#define IID_KEY_LENGTH 16

ArgumentOperation * ArgumentOperation_New(uint16_t piid)
{
    ArgumentOperation *thiz = NULL;

    do
    {
        thiz = (ArgumentOperation *) tiny_malloc(sizeof(ArgumentOperation));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(ArgumentOperation_Construct(thiz, piid)))
        {
            ArgumentOperation_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void ArgumentOperation_Delete(ArgumentOperation *thiz)
{
    RETURN_IF_FAIL(thiz);

    ArgumentOperation_Dispose(thiz);
    tiny_free(thiz);
}

TinyRet ArgumentOperation_Construct(ArgumentOperation *thiz, uint16_t piid)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(ArgumentOperation));
    thiz->piid = piid;

    return JsonArray_Construct(&thiz->values);
}

void ArgumentOperation_Dispose(ArgumentOperation *thiz)
{
    RETURN_IF_FAIL(thiz);

    JsonArray_Dispose(&thiz->values);
}

TinyRet ArgumentOperation_Copy(ArgumentOperation *dst, const ArgumentOperation *src)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(dst, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(src, TINY_RET_E_NULL_PTR);

    do
    {
        if (dst == src)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        dst->piid = src->piid;

        ret = JsonArray_CopyInto(&dst->values, &src->values);
        if (RET_FAILED(ret))
        {
            break;
        }
    } while (false);

    return ret;
}

ArgumentOperation * ArgumentOperation_CopyFrom(const ArgumentOperation *other)
{
    ArgumentOperation *thiz = NULL;

    RETURN_VAL_IF_FAIL(other, NULL);

    do
    {
        thiz = ArgumentOperation_New(other->piid);
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(ArgumentOperation_Copy(thiz, other)))
        {
            ArgumentOperation_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

TinyRet ArgumentOperation_AddValue(ArgumentOperation *thiz, const JsonValue *value)
{
    JsonValue *copy = NULL;
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(value, TINY_RET_E_NULL_PTR);

    do
    {
        copy = JsonValue_NewFrom(value);
        if (copy == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        ret = JsonArray_AddValue(&thiz->values, copy);
        if (RET_FAILED(ret))
        {
            JsonValue_Delete(copy);
            break;
        }
    } while (false);

    return ret;
}

static void OnArgumentOperationDelete(void *data, void *context)
{
    (void) context;

    ArgumentOperation_Delete((ArgumentOperation *) data);
}

static TinyRet ArgumentOperation_DecodeArrayItem(const JsonValue *item, TinyList *out)
{
    ArgumentOperation *operation = NULL;
    JsonNumber *piidNumber = NULL;
    JsonArray *values = NULL;
    uint16_t piid = 0;
    TinyRet ret = TINY_RET_OK;

    if (item == NULL)
    {
        return TINY_RET_E_PARAM_INVALID;
    }

    if (item->type == JSON_NUMBER)
    {
        piidNumber = item->data.number;
        if (! JsonNumber_IsInteger(piidNumber))
        {
            return TINY_RET_E_PARAM_INVALID;
        }

        operation = ArgumentOperation_New((uint16_t) JsonNumber_GetInteger(piidNumber));
        if (operation == NULL)
        {
            return TINY_RET_E_NO_MEMORY;
        }

        if (TinyList_AddTail(out, operation) == NULL)
        {
            ArgumentOperation_Delete(operation);
            return TINY_RET_E_NO_MEMORY;
        }

        return TINY_RET_OK;
    }

    if (item->type != JSON_OBJECT)
    {
        /* 与 Java 一致：忽略无法识别的元素 */
        return TINY_RET_OK;
    }

    piidNumber = JsonObject_GetNumber(item->data.object, "piid");
    if (piidNumber == NULL || ! JsonNumber_IsInteger(piidNumber))
    {
        return TINY_RET_E_PARAM_INVALID;
    }

    piid = (uint16_t) JsonNumber_GetInteger(piidNumber);
    operation = ArgumentOperation_New(piid);
    if (operation == NULL)
    {
        return TINY_RET_E_NO_MEMORY;
    }

    do
    {
        values = JsonObject_GetArray(item->data.object, "values");
        if (values != NULL)
        {
            ret = JsonArray_CopyInto(&operation->values, values);
            if (RET_FAILED(ret))
            {
                break;
            }
        }

        if (TinyList_AddTail(out, operation) == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        operation = NULL;
    } while (false);

    ArgumentOperation_Delete(operation);

    return ret;
}

static TinyRet ArgumentOperation_DecodeCompactItem(const char *key, const JsonValue *value, TinyList *out)
{
    ArgumentOperation *operation = NULL;
    char *end = NULL;
    long piid = 0;
    TinyRet ret = TINY_RET_OK;

    if (key == NULL || key[0] != '#')
    {
        return TINY_RET_E_PARAM_INVALID;
    }

    piid = strtol(key + 1, &end, 10);
    if (end == NULL || *end != '\0' || piid < 0 || piid > 65535)
    {
        return TINY_RET_E_PARAM_INVALID;
    }

    if (value == NULL || value->type != JSON_ARRAY)
    {
        return TINY_RET_E_PARAM_INVALID;
    }

    operation = ArgumentOperation_New((uint16_t) piid);
    if (operation == NULL)
    {
        return TINY_RET_E_NO_MEMORY;
    }

    ret = JsonArray_CopyInto(&operation->values, value->data.array);
    if (RET_FAILED(ret))
    {
        ArgumentOperation_Delete(operation);
        return ret;
    }

    if (TinyList_AddTail(out, operation) == NULL)
    {
        ArgumentOperation_Delete(operation);
        return TINY_RET_E_NO_MEMORY;
    }

    return TINY_RET_OK;
}

TinyRet ArgumentOperations_Decode(const JsonValue *wire, TinyList *out, bool *compact)
{
    TinyRet ret = TINY_RET_OK;
    bool unusedCompact = false;

    RETURN_VAL_IF_FAIL(wire, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(out, TINY_RET_E_NULL_PTR);

    if (compact == NULL)
    {
        compact = &unusedCompact;
    }

    *compact = false;

    if (wire->type == JSON_ARRAY)
    {
        uint32_t size = JsonArray_GetSize(wire->data.array);
        uint32_t i = 0;

        for (i = 0; i < size; ++i)
        {
            JsonValue *item = JsonArray_GetValue(wire->data.array, i);

            ret = ArgumentOperation_DecodeArrayItem(item, out);
            if (RET_FAILED(ret))
            {
                break;
            }
        }

        return ret;
    }

    if (wire->type == JSON_OBJECT)
    {
        TinyMap *map = &wire->data.object->data;
        uint32_t size = TinyMap_GetSize(map);
        uint32_t i = 0;

        for (i = 0; i < size; ++i)
        {
            TinyMapItem *item = TinyMap_GetItemAt(map, i);

            if (item == NULL)
            {
                return TINY_RET_E_PARAM_INVALID;
            }

            ret = ArgumentOperation_DecodeCompactItem(TinyMapItem_GetKey(item),
                                                     (const JsonValue *) TinyMapItem_GetValue(item),
                                                     out);
            if (RET_FAILED(ret))
            {
                break;
            }
        }

        *compact = true;
        return ret;
    }

    return TINY_RET_E_PARAM_INVALID;
}

JsonValue * ArgumentOperations_Encode(const TinyList *operations, bool compact)
{
    uint32_t size = 0;
    uint32_t i = 0;

    RETURN_VAL_IF_FAIL(operations, NULL);

    size = TinyList_GetSize(operations);

    if (compact)
    {
        JsonObject *object = JsonObject_New();
        if (object == NULL)
        {
            return NULL;
        }

        for (i = 0; i < size; ++i)
        {
            ArgumentOperation *operation = (ArgumentOperation *) TinyList_GetAt(operations, i);
            JsonArray *values = NULL;
            JsonValue *wireValues = NULL;
            char key[IID_KEY_LENGTH];

            if (operation == NULL)
            {
                JsonObject_Delete(object);
                return NULL;
            }

            values = JsonArray_NewFrom(&operation->values);
            if (values == NULL)
            {
                JsonObject_Delete(object);
                return NULL;
            }

            wireValues = JsonValue_NewArray(values);
            if (wireValues == NULL)
            {
                JsonArray_Delete(values);
                JsonObject_Delete(object);
                return NULL;
            }

            tiny_snprintf(key, sizeof(key), "#%u", (unsigned int) operation->piid);
            if (RET_FAILED(JsonObject_PutValue(object, key, wireValues)))
            {
                JsonValue_Delete(wireValues);
                JsonObject_Delete(object);
                return NULL;
            }
        }

        return JsonValue_NewObject(object);
    }

    {
        JsonArray *array = JsonArray_New();
        if (array == NULL)
        {
            return NULL;
        }

        for (i = 0; i < size; ++i)
        {
            ArgumentOperation *operation = (ArgumentOperation *) TinyList_GetAt(operations, i);
            JsonObject *entry = NULL;
            JsonArray *values = NULL;
            JsonValue *wireValues = NULL;

            if (operation == NULL)
            {
                JsonArray_Delete(array);
                return NULL;
            }

            entry = JsonObject_New();
            if (entry == NULL)
            {
                JsonArray_Delete(array);
                return NULL;
            }

            if (RET_FAILED(JsonObject_PutInteger(entry, "piid", (int32_t) operation->piid)))
            {
                JsonObject_Delete(entry);
                JsonArray_Delete(array);
                return NULL;
            }

            values = JsonArray_NewFrom(&operation->values);
            if (values == NULL)
            {
                JsonObject_Delete(entry);
                JsonArray_Delete(array);
                return NULL;
            }

            wireValues = JsonValue_NewArray(values);
            if (wireValues == NULL)
            {
                JsonArray_Delete(values);
                JsonObject_Delete(entry);
                JsonArray_Delete(array);
                return NULL;
            }

            if (RET_FAILED(JsonObject_PutValue(entry, "values", wireValues)))
            {
                JsonValue_Delete(wireValues);
                JsonObject_Delete(entry);
                JsonArray_Delete(array);
                return NULL;
            }

            if (RET_FAILED(JsonArray_AddObject(array, entry)))
            {
                JsonObject_Delete(entry);
                JsonArray_Delete(array);
                return NULL;
            }
        }

        return JsonValue_NewArray(array);
    }
}

TinyRet ArgumentOperations_SetDeleteListener(TinyList *operations)
{
    RETURN_VAL_IF_FAIL(operations, TINY_RET_E_NULL_PTR);

    TinyList_SetDeleteListener(operations, OnArgumentOperationDelete, NULL);

    return TINY_RET_OK;
}
