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
#include <tinyspec/iot_typedef/node/node.h>
#include <tinyspec/stanza_codec/iq/node/control/get_nodes_codec.h>

bool GetNodesCodec_EncodeResult(ResultGetNodes *result, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(content, false);
    RETURN_VAL_IF_FAIL(result, false);

    do
    {
        TINYCORE_RET ret = TINY_RET_OK;

        JsonArray *array = JsonArray_New();
        if (array == NULL)
        {
            break;
        }

        for (uint32_t i = 0; i < result->nodes.size; ++i)
        {
            Node *node = (Node *) TinyList_GetAt(&result->nodes, i);
            JsonObject *object = NULL;

            object = JsonObject_New();
            if (object == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            ret = JsonObject_PutString(object, "serialNumber", node->serialNumber);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }

            ret = JsonObject_PutInteger(object, "productId", node->productId);
            if (RET_FAILED(ret))
            {
                JsonObject_Delete(object);
                break;
            }

            ret = JsonObject_PutInteger(object, "productVersion", node->productVersion);
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

        if (RET_FAILED(JsonObject_PutArray(content, "nodes", array)))
        {
            JsonArray_Delete(array);
            break;
        }

        done = true;
    } while (false);

    return done;
}

TINYCORE_RET GetNodesCodec_DecodeResult(JsonObject *content, ResultGetNodes *result)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(result, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonArray *array = JsonObject_GetArray(content, "nodes");
        if (array == NULL)
        {
            TINY_LOG_ERROR("nodes not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        for (uint32_t i = 0; i < array->values.size; ++i)
        {
            Node *node = NULL;
            JsonValue *item = (JsonValue *) TinyList_GetAt(&array->values, i);
            JsonObject *object = NULL;
            JsonString *serialNumber = NULL;
            JsonNumber *productId = NULL;
            JsonNumber *productVersion = NULL;

            if (item->type != JSON_OBJECT)
            {
                TINY_LOG_ERROR("invalid content");
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }
            object = item->data.object;

            node = Node_New();
            if (node == NULL)
            {
                TINY_LOG_ERROR("Node_New failed");
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }

            serialNumber = JsonObject_GetString(object, "serialNumber");
            if (serialNumber == NULL)
            {
                TINY_LOG_ERROR("serialNumber not found");
                ret = TINY_RET_E_PARAM_INVALID;
                Node_Delete(node);
                break;
            }

            productId = JsonObject_GetNumber(object, "productId");
            if (productId == NULL)
            {
                TINY_LOG_ERROR("productId not found");
                ret = TINY_RET_E_PARAM_INVALID;
                Node_Delete(node);
                break;
            }

            productVersion = JsonObject_GetNumber(object, "productVersion");
            if (productVersion == NULL)
            {
                TINY_LOG_ERROR("productVersion not found");
                ret = TINY_RET_E_PARAM_INVALID;
                Node_Delete(node);
                break;
            }

            strncpy(node->serialNumber, serialNumber->value, NODE_SERIAL_NUMBER_LENGTH);
            node->productId = productId->value.intValue;
            node->productVersion = productVersion->value.intValue;

            if (TinyList_AddTail(&result->nodes, node) == NULL)
            {
                ret = TINY_RET_E_NO_MEMORY;
                TINY_LOG_ERROR("TinyList_AddTail failed");
                Node_Delete(node);
                break;
            }
        }
    } while (false);

    return ret;
}
