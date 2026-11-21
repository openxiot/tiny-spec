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
#include <tinycore/json_array.h>
#include <tinycore/json_number.h>
#include <tinycore/json_object.h>
#include <tinycore/tiny_log.h>
#include <tinycore/tiny_map.h>
#include <tinycore/tiny_snprintf.h>
#include <tinyspec/typedef/operation/combination_value.h>

#define IID_KEY_LENGTH 16

static bool CombinationValue_ParseIid(const char *text, uint16_t *iid)
{
    char *end = NULL;
    long value = 0;

    if (text == NULL || text[0] == '\0')
    {
        return false;
    }

    value = strtol(text, &end, 10);
    if (end == NULL || *end != '\0')
    {
        return false;
    }

    if (value < 0 || value > 65535)
    {
        return false;
    }

    *iid = (uint16_t) value;
    return true;
}

static JsonValue * CombinationValue_DecodeMember(const JsonValue *wire)
{
    JsonValue *decoded = NULL;
    bool compact = false;

    RETURN_VAL_IF_FAIL(wire, NULL);

    if (wire->type == JSON_OBJECT || wire->type == JSON_ARRAY)
    {
        decoded = CombinationValue_Decode(wire, &compact);
        if (decoded != NULL)
        {
            return decoded;
        }
    }

    return JsonValue_NewFrom(wire);
}

static JsonValue * CombinationValue_EncodeMember(const JsonValue *canonical, bool compact)
{
    RETURN_VAL_IF_FAIL(canonical, NULL);

    if (canonical->type == JSON_OBJECT)
    {
        return CombinationValue_Encode(canonical, compact);
    }

    return JsonValue_NewFrom(canonical);
}

static JsonValue * CombinationValue_DecodeObject(const JsonObject *wire, bool *compact)
{
    JsonObject *canonical = NULL;
    uint32_t size = 0;
    uint32_t i = 0;
    bool hasCompactKey = false;

    canonical = JsonObject_New();
    if (canonical == NULL)
    {
        return NULL;
    }

    size = TinyMap_GetSize(&wire->data);
    for (i = 0; i < size; ++i)
    {
        TinyMapItem *item = TinyMap_GetItemAt(&wire->data, i);
        const char *key = NULL;
        const char *iidText = NULL;
        JsonValue *member = NULL;
        uint16_t iid = 0;
        char canonicalKey[IID_KEY_LENGTH];

        if (item == NULL)
        {
            JsonObject_Delete(canonical);
            return NULL;
        }

        key = TinyMapItem_GetKey(item);
        if (key == NULL)
        {
            JsonObject_Delete(canonical);
            return NULL;
        }

        iidText = key;
        if (key[0] == '#')
        {
            hasCompactKey = true;
            iidText = key + 1;
        }

        if (! CombinationValue_ParseIid(iidText, &iid))
        {
            TINY_LOG_ERROR("combination member key invalid: %s", key);
            JsonObject_Delete(canonical);
            return NULL;
        }

        member = CombinationValue_DecodeMember((const JsonValue *) TinyMapItem_GetValue(item));
        if (member == NULL)
        {
            JsonObject_Delete(canonical);
            return NULL;
        }

        tiny_snprintf(canonicalKey, sizeof(canonicalKey), "%u", (unsigned int) iid);
        if (RET_FAILED(JsonObject_PutValue(canonical, canonicalKey, member)))
        {
            JsonValue_Delete(member);
            JsonObject_Delete(canonical);
            return NULL;
        }
    }

    *compact = hasCompactKey;
    return JsonValue_NewObject(canonical);
}

static JsonValue * CombinationValue_DecodeArray(const JsonArray *wire, bool *compact)
{
    JsonObject *canonical = NULL;
    uint32_t size = 0;
    uint32_t i = 0;

    canonical = JsonObject_New();
    if (canonical == NULL)
    {
        return NULL;
    }

    size = JsonArray_GetSize(wire);
    for (i = 0; i < size; ++i)
    {
        JsonValue *item = JsonArray_GetValue(wire, i);
        JsonObject *object = NULL;
        JsonNumber *piidNumber = NULL;
        JsonValue *value = NULL;
        JsonValue *member = NULL;
        uint16_t piid = 0;
        char canonicalKey[IID_KEY_LENGTH];

        if (item == NULL || item->type != JSON_OBJECT)
        {
            JsonObject_Delete(canonical);
            return NULL;
        }

        object = item->data.object;
        piidNumber = JsonObject_GetNumber(object, "piid");
        if (piidNumber == NULL || ! JsonNumber_IsInteger(piidNumber))
        {
            JsonObject_Delete(canonical);
            return NULL;
        }

        piid = (uint16_t) JsonNumber_GetInteger(piidNumber);
        value = JsonObject_GetValue(object, "value");

        if (piid == 0 && value == NULL)
        {
            /* 与 Java 一致：piid/value 都取不到，说明这是普通属性值列表 */
            JsonObject_Delete(canonical);
            return NULL;
        }

        member = (value != NULL) ? CombinationValue_DecodeMember(value) : JsonValue_NewNull();
        if (member == NULL)
        {
            JsonObject_Delete(canonical);
            return NULL;
        }

        tiny_snprintf(canonicalKey, sizeof(canonicalKey), "%u", (unsigned int) piid);
        if (RET_FAILED(JsonObject_PutValue(canonical, canonicalKey, member)))
        {
            JsonValue_Delete(member);
            JsonObject_Delete(canonical);
            return NULL;
        }
    }

    *compact = false;
    return JsonValue_NewObject(canonical);
}

JsonValue * CombinationValue_Decode(const JsonValue *wire, bool *compact)
{
    bool unusedCompact = false;

    RETURN_VAL_IF_FAIL(wire, NULL);

    if (compact == NULL)
    {
        compact = &unusedCompact;
    }

    *compact = false;

    if (wire->type == JSON_OBJECT)
    {
        return CombinationValue_DecodeObject(wire->data.object, compact);
    }

    if (wire->type == JSON_ARRAY)
    {
        return CombinationValue_DecodeArray(wire->data.array, compact);
    }

    return NULL;
}

static JsonValue * CombinationValue_EncodeObject(const JsonObject *canonical, bool compact)
{
    uint32_t size = TinyMap_GetSize(&canonical->data);
    uint32_t i = 0;

    if (compact)
    {
        JsonObject *object = JsonObject_New();
        if (object == NULL)
        {
            return NULL;
        }

        for (i = 0; i < size; ++i)
        {
            TinyMapItem *item = TinyMap_GetItemAt(&canonical->data, i);
            const char *key = NULL;
            JsonValue *member = NULL;
            uint16_t iid = 0;
            char compactKey[IID_KEY_LENGTH];

            if (item == NULL)
            {
                JsonObject_Delete(object);
                return NULL;
            }

            key = TinyMapItem_GetKey(item);
            if (key == NULL || ! CombinationValue_ParseIid(key, &iid))
            {
                JsonObject_Delete(object);
                return NULL;
            }

            member = CombinationValue_EncodeMember((const JsonValue *) TinyMapItem_GetValue(item), true);
            if (member == NULL)
            {
                JsonObject_Delete(object);
                return NULL;
            }

            tiny_snprintf(compactKey, sizeof(compactKey), "#%u", (unsigned int) iid);
            if (RET_FAILED(JsonObject_PutValue(object, compactKey, member)))
            {
                JsonValue_Delete(member);
                JsonObject_Delete(object);
                return NULL;
            }
        }

        return JsonValue_NewObject(object);
    }

    /* 标准形态：[{"piid": N, "value": V}, ...] */
    {
        JsonArray *array = JsonArray_New();
        if (array == NULL)
        {
            return NULL;
        }

        for (i = 0; i < size; ++i)
        {
            TinyMapItem *item = TinyMap_GetItemAt(&canonical->data, i);
            const char *key = NULL;
            JsonObject *entry = NULL;
            JsonValue *member = NULL;
            uint16_t iid = 0;

            if (item == NULL)
            {
                JsonArray_Delete(array);
                return NULL;
            }

            key = TinyMapItem_GetKey(item);
            if (key == NULL || ! CombinationValue_ParseIid(key, &iid))
            {
                JsonArray_Delete(array);
                return NULL;
            }

            member = CombinationValue_EncodeMember((const JsonValue *) TinyMapItem_GetValue(item), false);
            if (member == NULL)
            {
                JsonArray_Delete(array);
                return NULL;
            }

            entry = JsonObject_New();
            if (entry == NULL)
            {
                JsonValue_Delete(member);
                JsonArray_Delete(array);
                return NULL;
            }

            if (RET_FAILED(JsonObject_PutInteger(entry, "piid", (int32_t) iid)))
            {
                JsonValue_Delete(member);
                JsonObject_Delete(entry);
                JsonArray_Delete(array);
                return NULL;
            }

            if (RET_FAILED(JsonObject_PutValue(entry, "value", member)))
            {
                JsonValue_Delete(member);
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

JsonValue * CombinationValue_Encode(const JsonValue *canonical, bool compact)
{
    RETURN_VAL_IF_FAIL(canonical, NULL);

    if (canonical->type != JSON_OBJECT)
    {
        return JsonValue_NewFrom(canonical);
    }

    return CombinationValue_EncodeObject(canonical->data.object, compact);
}
