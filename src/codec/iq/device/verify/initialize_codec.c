/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/codec/iq/device/verify/initialize_codec.h>

TINYCORE_RET InitializeCodec_DecodeQuery(JsonObject *content, QueryInitialize *query)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(query, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *version = JsonObject_GetString(content, "version");
        if (version == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        JsonString *authentication = JsonObject_GetString(content, "authentication");
        if (authentication == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        strncpy(query->version, version->value, STANZA_INITIALIZE_VERSION_LEN);
        strncpy(query->authentication, authentication->value, STANZA_AUTHENTICATION_LEN);
    } while (false);

    return ret;
}

bool InitializeCodec_EncodeQuery(QueryInitialize *query, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(content, false);
    RETURN_VAL_IF_FAIL(query, false);

    do
    {
        if (RET_FAILED(JsonObject_PutString(content, "version", query->version)))
        {
            break;
        }

        if (RET_FAILED(JsonObject_PutString(content, "authentication", query->authentication)))
        {
            break;
        }

        done = true;
    } while (false);

    return done;
}
