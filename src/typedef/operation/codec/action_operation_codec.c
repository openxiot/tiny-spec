/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/json_object.h>
#include <tinyspec/typedef/operation/codec/action_operation_codec.h>

#include <tinyspec/typedef/xid/codec/action_id_codec.h>
#include <tinycore/json_array.h>
#include <tinycore/json_object.h>
#include <tinycore/json_value.h>

bool ActionOperationCodec_Invoke_Query_Encode(ActionOperation *operation, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(operation, false);
    RETURN_VAL_IF_FAIL(content, false);

    do
    {
        char aid[IOT_XID_LENGTH];

        ActionId_ToString(aid, &operation->aid);

        if (RET_FAILED(JsonObject_PutString(content, "aid", aid)))
        {
            break;
        }

        if (TinyList_GetSize(&operation->in) > 0)
        {
            JsonValue *wire = ArgumentOperations_Encode(&operation->in, operation->argumentsCompact);
            if (wire == NULL)
            {
                break;
            }

            if (RET_FAILED(JsonObject_PutValue(content, "in", wire)))
            {
                JsonValue_Delete(wire);
                break;
            }
        }

        done = true;
    } while (false);

    return done;
}

TinyRet ActionOperationCodec_Invoke_Query_Decode(JsonObject *content, ActionOperation *operation)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(operation, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *aid = JsonObject_GetString(content, "aid");
        JsonValue *in = NULL;

        if (aid == NULL)
        {
            TINY_LOG_ERROR("aid not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = ActionId_FromString(&operation->aid, aid->value);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("ActionId_FromString FAILED");
            operation->status = -100;
            break;
        }

        in = JsonObject_GetValue(content, "in");
        if (in != NULL)
        {
            ret = ArgumentOperations_Decode(in, &operation->in, &operation->argumentsCompact);
            if (RET_FAILED(ret))
            {
                TINY_LOG_ERROR("ArgumentOperations_Decode FAILED");
                operation->status = -1000;
                break;
            }
        }
    } while (false);

    return ret;
}

bool ActionOperationCodec_Invoke_Result_Encode(ActionOperation *operation, JsonObject *content)
{
    bool done = false;

    RETURN_VAL_IF_FAIL(content, false);
    RETURN_VAL_IF_FAIL(operation, false);

    do
    {
        char aid[IOT_XID_LENGTH];

        ActionId_ToString(aid, &operation->aid);

        if (RET_FAILED(JsonObject_PutString(content, "aid", aid)))
        {
            break;
        }

        if (RET_FAILED(JsonObject_PutInteger(content, "status", operation->status)))
        {
            break;
        }

        if (operation->status != 0)
        {
            done = true;
            break;
        }

        if (TinyList_GetSize(&operation->out) > 0)
        {
            JsonValue *wire = ArgumentOperations_Encode(&operation->out, operation->argumentsCompact);
            if (wire == NULL)
            {
                break;
            }

            if (RET_FAILED(JsonObject_PutValue(content, "out", wire)))
            {
                JsonValue_Delete(wire);
                break;
            }
        }

        done = true;
    } while (false);

    return done;
}

TinyRet ActionOperationCodec_Invoke_Result_Decode(JsonObject *content, ActionOperation *operation)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(operation, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *aid = JsonObject_GetString(content, "aid");
        JsonNumber *status = NULL;
        JsonValue *out = NULL;

        if (aid == NULL)
        {
            TINY_LOG_ERROR("aid not found");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        ret = ActionId_FromString(&operation->aid, aid->value);
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("ActionId_FromString FAILED");
            operation->status = -100;
            break;
        }

        status = JsonObject_GetNumber(content, "status");
        if (status == NULL || status->type != JSON_NUMBER_INTEGER)
        {
            TINY_LOG_ERROR("status invalid");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        operation->status = (int) status->value.intValue;

        if (operation->status != 0)
        {
            break;
        }

        out = JsonObject_GetValue(content, "out");
        if (out != NULL)
        {
            ret = ArgumentOperations_Decode(out, &operation->out, &operation->argumentsCompact);
            if (RET_FAILED(ret))
            {
                TINY_LOG_ERROR("ArgumentOperations_Decode FAILED");
                break;
            }
        }
    } while (false);

    return ret;
}
