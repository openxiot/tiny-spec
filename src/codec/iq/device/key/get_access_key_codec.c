/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/codec/iq/device/key/get_access_key_codec.h>

bool GetAccessKeyCodec_EncodeResult(ResultGetAccessKey *result, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(result, false);
    RETURN_VAL_IF_FAIL(content, false);

    do
    {
        if (RET_FAILED(JsonObject_PutString(content, "key", result->key)))
        {
            break;
        }

        done = true;
    } while (false);

    return done;
}

TINYCORE_RET GetAccessKeyCodec_DecodeResult(JsonObject *content, ResultGetAccessKey *result)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(result, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *key = JsonObject_GetString(content, "key");
        if (key == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        strncpy(result->key, key->value, STANZA_ACCESS_KEY_LEN);
    } while (false);

    return ret;
}
