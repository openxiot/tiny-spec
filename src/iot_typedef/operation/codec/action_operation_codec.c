/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/iot_typedef/operation/codec/action_operation_codec.h>

#include <tinyspec/iot_typedef/xid/codec/action_id_codec.h>
#include <tinyspec/json_util.h>

bool ActionOperationCodec_Invoke_Query_Encode(ActionOperation *operation, JsonObject *content)
{
    (void) operation;
    (void) content;

    return false;
}

TINYCORE_RET ActionOperationCodec_Invoke_Query_Decode(JsonObject *content, ActionOperation *operation)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(operation, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_NULL_PTR);

    do
    {
        JsonArray *in = JsonObject_GetArray(content, "in");
        JsonString *aid = JsonObject_GetString(content, "aid");
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

        if (in != NULL)
        {
            if (RET_FAILED(JsonArray_CopyInto(&operation->in, in)))
            {
                TINY_LOG_ERROR("JsonArray_Copy FAILED");
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
        JsonArray *out = NULL;

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
            break;
        }

        if (operation->out.values.size == 0)
        {
            break;
        }

        out = JsonArray_NewFrom(&operation->out);
        if (out == NULL)
        {
            break;
        }

        if (RET_FAILED(JsonObject_PutArray(content, "out", out)))
        {
            JsonArray_Delete(out);
            break;
        }

        done = true;
    } while (false);

    return done;
}

TINYCORE_RET ActionOperationCodec_Invoke_Result_Decode(JsonObject *content, ActionOperation *operation)
{
    (void) content;
    (void) operation;

    return TINY_RET_E_NOT_IMPLEMENT;
}
