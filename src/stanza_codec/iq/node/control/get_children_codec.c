/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/json_array.h>
#include <tinycore/json_boolean.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/iot_typedef/node/child.h>
#include <tinyspec/stanza_codec/iq/node/control/get_children_codec.h>

static void copy_string(char *dst, size_t capacity, const char *src)
{
    if (src == NULL)
    {
        dst[0] = '\0';
        return;
    }

    strncpy(dst, src, capacity - 1);
    dst[capacity - 1] = '\0';
}

static bool encode_child(JsonObject *object, const Child *child)
{
    JsonObject *summary = JsonObject_New();
    bool done = false;

    if (summary == NULL)
    {
        return false;
    }

    do
    {
        if (RET_FAILED(JsonObject_PutString(object, "did", child->did)))
        {
            break;
        }

        if (child->type[0] != '\0')
        {
            if (RET_FAILED(JsonObject_PutString(summary, "type", child->type)))
            {
                break;
            }
        }
        else
        {
            if (RET_FAILED(JsonObject_PutInteger(summary, "productId", child->productId)) ||
                RET_FAILED(JsonObject_PutInteger(summary, "productVersion", child->productVersion)))
            {
                break;
            }
        }

        if (RET_FAILED(JsonObject_PutBoolean(summary, "online", child->online)) ||
            RET_FAILED(JsonObject_PutString(summary, "protocol", child->protocol)) ||
            RET_FAILED(JsonObject_PutString(summary, "parentId", child->parentId)))
        {
            break;
        }

        /* JsonObject_PutObject() frees the value when the insert fails. */
        if (RET_FAILED(JsonObject_PutObject(object, "summary", summary)))
        {
            summary = NULL;
            break;
        }
        summary = NULL; /* ownership moved into the child object */
        done = true;
    } while (false);

    if (summary != NULL)
    {
        JsonObject_Delete(summary);
    }

    return done;
}

bool GetChildrenCodec_EncodeResult(ResultGetChildren *result, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(content, false);
    RETURN_VAL_IF_FAIL(result, false);

    do
    {
        JsonArray *array = JsonArray_New();
        if (array == NULL)
        {
            break;
        }

        for (uint32_t i = 0; i < result->children.size; ++i)
        {
            Child *child = (Child *) TinyList_GetAt(&result->children, i);
            JsonObject *object = JsonObject_New();

            if (object == NULL ||
                ! encode_child(object, child) ||
                RET_FAILED(JsonArray_AddObject(array, object)))
            {
                JsonObject_Delete(object);
                break;
            }
        }

        /* JsonObject_PutArray() frees the array when the insert fails. */
        if (RET_FAILED(JsonObject_PutArray(content, "children", array)))
        {
            break;
        }

        done = true;
    } while (false);

    return done;
}

TINYCORE_RET GetChildrenCodec_DecodeResult(JsonObject *content, ResultGetChildren *result)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(result, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonArray *array = JsonObject_GetArray(content, "children");
        if (array == NULL)
        {
            TINY_LOG_ERROR("children not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        for (uint32_t i = 0; i < array->values.size; ++i)
        {
            JsonValue *item = (JsonValue *) TinyList_GetAt(&array->values, i);
            JsonObject *object = NULL;
            JsonObject *summary = NULL;
            const char *did = NULL;
            const char *type = NULL;
            Child *child = NULL;

            if (item->type != JSON_OBJECT)
            {
                TINY_LOG_ERROR("invalid content");
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }
            object = item->data.object;

            did = JsonObject_GetStringValue(object, "did");
            summary = JsonObject_GetObject(object, "summary");
            if (did == NULL || summary == NULL)
            {
                TINY_LOG_ERROR("did or summary not found");
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }

            child = Child_New();
            if (child == NULL)
            {
                TINY_LOG_ERROR("Child_New failed");
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            copy_string(child->did, CHILD_DID_LENGTH, did);
            type = JsonObject_GetStringValue(summary, "type");
            if (type != NULL)
            {
                copy_string(child->type, CHILD_TYPE_LENGTH, type);
            }
            else
            {
                JsonNumber *productId = JsonObject_GetNumber(summary, "productId");
                JsonNumber *productVersion = JsonObject_GetNumber(summary, "productVersion");

                if (productId != NULL)
                {
                    child->productId = productId->value.intValue;
                }
                if (productVersion != NULL)
                {
                    child->productVersion = productVersion->value.intValue;
                }
            }

            JsonBoolean *online = JsonObject_GetBoolean(summary, "online");
            child->online = online != NULL && JsonBoolean_GetValue(online);
            copy_string(child->protocol, CHILD_PROTOCOL_LENGTH,
                        JsonObject_GetStringValue(summary, "protocol"));
            copy_string(child->parentId, CHILD_PARENT_ID_LENGTH,
                        JsonObject_GetStringValue(summary, "parentId"));

            if (TinyList_AddTail(&result->children, child) == NULL)
            {
                TINY_LOG_ERROR("TinyList_AddTail failed");
                Child_Delete(child);
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }
        }
    } while (false);

    return ret;
}
