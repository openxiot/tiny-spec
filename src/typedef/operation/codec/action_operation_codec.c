/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/typedef/operation/codec/action_operation_codec.h>

#include <tinyspec/typedef/xid/codec/action_id_codec.h>
#include <tinycore/json_array.h>
#include <tinycore/json_object.h>
#include <tinycore/json_value.h>

/*
 * 平台下发的 action 入参是属性式数组（按动作声明的输入顺序）：
 *   "in": [ {"piid":2,"values":[134144]}, {"piid":3,"values":["..."]}, ... ]
 * 而生成的 action handler 按位置读取 in[0..n]（JsonNumber/JsonString）。
 * 这里把每个条目的 values 按顺序展开成位置参数；老式纯值数组保持原样。
 */
static TinyRet copyInvocationInput(JsonArray *dst, const JsonArray *src)
{
    TinyRet ret = TINY_RET_OK;
    uint32_t i, j;

    JsonArray_Clean(dst);

    for (i = 0; i < JsonArray_GetSize(src); i++)
    {
        JsonValue *value = JsonArray_GetValue(src, i);
        JsonObject *input = JsonArray_GetObject(src, i);
        JsonArray *values = NULL;

        if (value == NULL)
        {
            continue;
        }

        if (input != NULL)
        {
            values = JsonObject_GetArray(input, "values");
            if (values != NULL)
            {
                /* values 为空也占一个位置，避免后续参数错位 */
                if (JsonArray_GetSize(values) == 0)
                {
                    if (RET_FAILED(JsonArray_AddNull(dst)))
                    {
                        ret = TINY_RET_E_NO_MEMORY;
                        break;
                    }
                }

                for (j = 0; j < JsonArray_GetSize(values); j++)
                {
                    JsonValue *v = JsonArray_GetValue(values, j);
                    JsonValue *copy = (v != NULL) ? JsonValue_NewFrom(v) : NULL;

                    if (copy == NULL || RET_FAILED(JsonArray_AddValue(dst, copy)))
                    {
                        if (copy != NULL)
                        {
                            JsonValue_Delete(copy);
                        }
                        ret = TINY_RET_E_NO_MEMORY;
                        break;
                    }
                }
                if (RET_FAILED(ret))
                {
                    break;
                }
                continue;
            }
        }

        {
            JsonValue *copy = JsonValue_NewFrom(value);

            if (copy == NULL || RET_FAILED(JsonArray_AddValue(dst, copy)))
            {
                if (copy != NULL)
                {
                    JsonValue_Delete(copy);
                }
                ret = TINY_RET_E_NO_MEMORY;
                break;
            }
        }
    }

    return ret;
}

bool ActionOperationCodec_Invoke_Query_Encode(ActionOperation *operation, JsonObject *content)
{
    (void) operation;
    (void) content;

    return false;
}

TinyRet ActionOperationCodec_Invoke_Query_Decode(JsonObject *content, ActionOperation *operation)
{
    TinyRet ret = TINY_RET_OK;

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
            if (RET_FAILED(copyInvocationInput(&operation->in, in)))
            {
                TINY_LOG_ERROR("action input decode FAILED");
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

TinyRet ActionOperationCodec_Invoke_Result_Decode(JsonObject *content, ActionOperation *operation)
{
    (void) content;
    (void) operation;

    return TINY_RET_E_NOT_IMPLEMENT;
}
