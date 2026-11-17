/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/json_string.h>
#include <tinycore/tiny_log.h>
#include <tinycore/json_number.h>
#include <tinyspec/codec/iq/iq_codec.h>
#include <tinyspec/codec/iq/iq_error_codec.h>
#include <tinyspec/codec/iq/iq_query_codec.h>
#include <tinyspec/codec/iq/iq_result_codec.h>

TinyRet IQCodec_Encode(JsonObject *root, IQ *iq)
{
    TinyRet ret = TINY_RET_OK;
    JsonObject *object = NULL;

    do
    {
        object = JsonObject_New();
        if (object == NULL)
        {
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        ret = JsonObject_PutString(object, "id", iq->id);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("JsonObject_PutString FAILED");
            ret = TINY_RET_E_OUT_OF_MEMORY;
            break;
        }

        switch (iq->type)
        {
            case IQ_TYPE_QUERY:
                ret = JsonObject_PutString(object, "type", "query");
                if (RET_SUCCEEDED(ret))
                {
                    ret = IQQueryCodec_Encode(object, &iq->content.query);
                }
                break;

            case IQ_TYPE_RESULT:
                ret = JsonObject_PutString(object, "type", "result");
                if (RET_SUCCEEDED(ret))
                {
                    ret = IQResultCodec_Encode(object, &iq->content.result);
                }
                break;

            case IQ_TYPE_ERROR:
                ret = JsonObject_PutString(object, "type", "error");
                if (RET_SUCCEEDED(ret))
                {
                    ret = IQErrorCodec_Encode(object, &iq->content.error);
                }
                break;

            default:
                ret = TINY_RET_E_PARAM_INVALID;
                break;
        }

        if (RET_FAILED(ret))
        {
            break;
        }

        ret = JsonObject_PutObject(root, "iq", object);
        if (RET_FAILED(ret))
        {
            break;
        }
    } while (false);

    if (RET_FAILED(ret))
    {
        if (object != NULL)
        {
            JsonObject_Delete(object);
        }
    }

    return ret;
}

TinyRet IQCodec_Decode(IQ* iq, JsonObject *object)
{
    TinyRet ret = TINY_RET_OK;

    do
    {
        JsonString *id = JsonObject_GetString(object, "id");
        if (id == NULL)
        {
            TINY_LOG_ERROR("#id not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        strncpy(iq->id, id->value, IQ_ID_LENGTH);

        JsonString *type = JsonObject_GetString(object, "type");
        if (type == NULL)
        {
            TINY_LOG_ERROR("#type not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        if (STR_EQUAL("query", type->value))
        {
            iq->type = IQ_TYPE_QUERY;
            ret = IQQueryCodec_Decode(&iq->content.query, object);
            break;
        }
        else if (STR_EQUAL("result", type->value))
        {
            iq->type = IQ_TYPE_RESULT;
            ret = IQResultCodec_Decode(&iq->content.result, object);
            break;
        }
        else if (STR_EQUAL("error", type->value))
        {
            iq->type = IQ_TYPE_ERROR;
            ret = IQErrorCodec_Decode(&iq->content.error, object);
            break;
        }
        else
        {
            TINY_LOG_ERROR("iq type not support");
            ret = TINY_RET_E_PARAM_INVALID;
        }
    } while (false);

    return ret;
}
