/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_log.h>
#include <tinyspec/iot_typedef/notice/node/impl/node_changed_codec.h>

TINYCORE_RET NodeInfoCodec_Encode(JsonObject *payload, Node *node)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(node, TINY_RET_E_NULL_PTR);

    do
    {
        ret = JsonObject_PutString(payload, "serialNumber", node->serialNumber);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("out of memory");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        ret = JsonObject_PutInteger(payload, "productId", node->productId);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("out of memory");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        ret = JsonObject_PutInteger(payload, "productVersion", node->productVersion);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("out of memory");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }
    } while (false);

    return ret;
}

TINYCORE_RET NodeInfoCodec_Decode(Node *node, JsonObject *payload)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(node, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *serialNumber = JsonObject_GetString(payload, "serialNumber");
        if (serialNumber == NULL)
        {
            TINY_LOG_ERROR("serialNumber not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonNumber *productId = JsonObject_GetNumber(payload, "productId");
        if (productId == NULL)
        {
            TINY_LOG_ERROR("productId not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        if (productId->type != JSON_NUMBER_INTEGER)
        {
            TINY_LOG_ERROR("productId not integer");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonNumber *productVersion = JsonObject_GetNumber(payload, "productVersion");
        if (productVersion == NULL)
        {
            TINY_LOG_ERROR("productVersion not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        if (productVersion->type != JSON_NUMBER_INTEGER)
        {
            TINY_LOG_ERROR("productVersion not integer");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        strncpy(node->serialNumber, serialNumber->value, NODE_SERIAL_NUMBER_LENGTH);
        node->productId = productId->value.intValue;
        node->productVersion = productVersion->value.intValue;
    } while (false);

    return ret;
}
