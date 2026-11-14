/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/codec/iq/device/key/set_access_key_codec.h>

TINYCORE_RET SetAccessKeyCodec_DecodeQuery(JsonObject *content, QuerySetAccessKey *query)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(query, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *key = JsonObject_GetString(content, "key");
        if (key == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        strncpy(query->key, key->value, STANZA_ACCESS_KEY_LEN);
    } while (false);

    return ret;
}

bool SetAccessKeyCodec_EncodeQuery(QuerySetAccessKey *query, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(content, false);
    RETURN_VAL_IF_FAIL(query, false);

    if (RET_SUCCEEDED(JsonObject_PutString(content, "key", query->key)))
    {
        done = true;
    }

    return done;
}
