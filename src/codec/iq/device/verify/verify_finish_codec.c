/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/codec/iq/device/verify/verify_finish_codec.h>
#include <tinycore/tiny_log.h>

TINYCORE_RET VerifyFinishCodec_DecodeQuery(JsonObject *content, QueryVerifyFinish *query)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(query, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *did = JsonObject_GetString(content, "device-id");
        if (did == NULL)
        {
            TINY_LOG_ERROR("#did not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonString *type = JsonObject_GetString(content, "device-type");
        if (type == NULL)
        {
            TINY_LOG_ERROR("#type not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonString *signature = JsonObject_GetString(content, "signature");
        if (signature == NULL)
        {
            TINY_LOG_ERROR("#signature not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonNumber *codec = JsonObject_GetNumber(content, "codec");
        if (codec == NULL)
        {
            TINY_LOG_ERROR("#codec not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        if (codec->type != JSON_NUMBER_INTEGER)
        {
            TINY_LOG_ERROR("#codec not integer");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        strncpy(query->did, did->value, IOT_DEVICE_ID_LENGTH);
        strncpy(query->type, type->value, IOT_DEVICE_TYPE_LENGTH);
        strncpy(query->signature, signature->value, STANZA_SIGNATURE_LENGTH);
        query->codec = (uint8_t) (codec->value.intValue);
    } while (false);

    return ret;
}

bool VerifyFinishCodec_EncodeQuery(QueryVerifyFinish *query, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(content, false);
    RETURN_VAL_IF_FAIL(query, false);

    do
    {
        if (RET_FAILED(JsonObject_PutString(content, "device-id", query->did)))
        {
            break;
        }

        if (RET_FAILED(JsonObject_PutString(content, "device-type", query->type)))
        {
            break;
        }

        if (RET_FAILED(JsonObject_PutString(content, "signature", query->signature)))
        {
            break;
        }

        if (RET_FAILED(JsonObject_PutInteger(content, "codec", query->codec)))
        {
            break;
        }

        done = true;
    } while (false);

    return done;
}
