/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/json_string.h>
#include <tinyspec/stanza_codec/iq/device/verify/verify_start_codec.h>

TINYCORE_RET VerifyStartCodec_DecodeQuery(JsonObject *content, QueryVerifyStart *query)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(query, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *publicKey = JsonObject_GetString(content, "public-key");
        if (publicKey == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        strncpy(query->publicKey, publicKey->value, STANZA_PUBLIC_KEY_LENGTH);
    } while (false);

    return ret;
}

bool VerifyStartCodec_EncodeQuery(QueryVerifyStart *query, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(content, false);
    RETURN_VAL_IF_FAIL(query, false);

    if (RET_SUCCEEDED(JsonObject_PutString(content, "public-key", query->publicKey)))
    {
        done = true;
    }

    return done;
}

TINYCORE_RET VerifyStartCodec_DecodeResult(JsonObject *content, ResultVerifyStart *result)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(result, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *publicKey = JsonObject_GetString(content, "public-key");
        if (publicKey == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonString *signature = JsonObject_GetString(content, "signature");
        if (signature == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        strncpy(result->publicKey, publicKey->value, STANZA_PUBLIC_KEY_LENGTH);
        strncpy(result->signature, signature->value, STANZA_SIGNATURE_LENGTH);
    } while (false);

    return ret;
}

bool VerifyStartCodec_EncodeResult(ResultVerifyStart *result, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(result, false);
    RETURN_VAL_IF_FAIL(content, false);

    do
    {
        if (RET_FAILED(JsonObject_PutString(content, "public-key", result->publicKey)))
        {
            break;
        }

        if (RET_FAILED(JsonObject_PutString(content, "signature", result->signature)))
        {
            break;
        }

        done = true;
    } while (false);

    return done;
}
