/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_log.h>
#include <tinycore/json_number.h>
#include <tinycore/json_string.h>
#include <tinyspec/stanza_codec/iq/iq_error_codec.h>

TINYCORE_RET IQErrorCodec_Encode(JsonObject *root, IQError *error)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(root, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(error, TINY_RET_E_NULL_PTR);

    do
    {
        JsonObject *content = JsonObject_New();
        if (content == NULL)
        {
            TINY_LOG_ERROR("out of memory");
            break;
        }

        ret = JsonObject_PutInteger(content, "status", error->status);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("out of memory");
            JsonObject_Delete(content);
            break;
        }

        ret = JsonObject_PutString(content, "description", error->description);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("out of memory");
            JsonObject_Delete(content);
            break;
        }

        ret = JsonObject_PutObject(root, "content", content);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("out of memory");
            JsonObject_Delete(content);
            break;
        }
    } while (false);

    return ret;
}

TINYCORE_RET IQErrorCodec_Decode(IQError* error, JsonObject *object)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(error, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(object, TINY_RET_E_NULL_PTR);

    do
    {
        JsonObject *content = NULL;
        JsonValue * status = NULL;
        JsonValue * description = NULL;

        content = JsonObject_GetObject(object, "content");
        if (content == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        status = JsonObject_GetValue(content, "status");
        if (status == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        description = JsonObject_GetValue(content, "description");
        if (description == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        if (status->type != JSON_NUMBER || description->type != JSON_STRING)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        if (status->data.number->type != JSON_NUMBER_INTEGER)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        error->status = (int)status->data.number->value.intValue;
        strncpy(error->description, description->data.string->value, STANZA_DESCRIPTION_LENGTH);
    } while (false);

    return ret;
}
