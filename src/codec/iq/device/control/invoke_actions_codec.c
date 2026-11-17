#include <tinycore/json_array.h>
#include <tinycore/json_object.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/codec/iq/device/control/invoke_actions_codec.h>
#include <tinyspec/typedef/operation/codec/action_operation_codec.h>

bool InvokeActionsCodec_EncodeQuery(QueryInvokeActions *query, JsonObject *content)
{
    RETURN_VAL_IF_FAIL(query, false);
    RETURN_VAL_IF_FAIL(content, false);

    JsonArray *actions = JsonArray_New();
    RETURN_VAL_IF_FAIL(actions, false);

    for (uint32_t i = 0; i < TinyList_GetSize(&query->actions); ++i)
    {
        ActionOperation *op = (ActionOperation *)TinyList_GetAt(&query->actions, i);
        JsonObject *ao = JsonObject_New();
        if (ao == NULL)
        {
            continue;
        }
        ActionOperationCodec_Invoke_Query_Encode(op, ao);
        JsonArray_AddObject(actions, ao);
    }
    JsonObject_PutArray(content, "actions", actions);
    return true;
}

TINYCORE_RET InvokeActionsCodec_DecodeQuery(JsonObject *content, QueryInvokeActions *query)
{
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_PARAM_INVALID);
    RETURN_VAL_IF_FAIL(query, TINY_RET_E_PARAM_INVALID);

    if (RET_FAILED(QueryInvokeActions_Construct(query)))
    {
        return TINY_RET_E_NULL_PTR;
    }

    JsonArray *actions = JsonObject_GetArray(content, "actions");
    if (actions != NULL)
    {
        for (uint32_t i = 0; i < JsonArray_GetSize(actions); ++i)
        {
            JsonObject *ao = JsonArray_GetObject(actions, i);
            ActionOperation *op = ActionOperation_New();
            if (op == NULL || ao == NULL)
            {
                continue;
            }
            if (RET_SUCCEEDED(ActionOperationCodec_Invoke_Query_Decode(ao, op)))
            {
                TinyList_AddTail(&query->actions, op);
            }
            else
            {
                ActionOperation_Delete(op);
            }
        }
    }
    return TINY_RET_OK;
}

bool InvokeActionsCodec_EncodeResult(ResultInvokeActions *result, JsonObject *content)
{
    RETURN_VAL_IF_FAIL(result, false);
    RETURN_VAL_IF_FAIL(content, false);

    JsonArray *actions = JsonArray_New();
    RETURN_VAL_IF_FAIL(actions, false);

    for (uint32_t i = 0; i < TinyList_GetSize(&result->actions); ++i)
    {
        ActionOperation *op = (ActionOperation *)TinyList_GetAt(&result->actions, i);
        JsonObject *ao = JsonObject_New();
        if (ao == NULL)
        {
            continue;
        }
        ActionOperationCodec_Invoke_Result_Encode(op, ao);
        JsonArray_AddObject(actions, ao);
    }
    JsonObject_PutArray(content, "actions", actions);
    return true;
}

TINYCORE_RET InvokeActionsCodec_DecodeResult(JsonObject *content, ResultInvokeActions *result)
{
    RETURN_VAL_IF_FAIL(content, TINY_RET_E_PARAM_INVALID);
    RETURN_VAL_IF_FAIL(result, TINY_RET_E_PARAM_INVALID);

    if (RET_FAILED(ResultInvokeActions_Construct(result)))
    {
        return TINY_RET_E_NULL_PTR;
    }

    JsonArray *actions = JsonObject_GetArray(content, "actions");
    if (actions != NULL)
    {
        for (uint32_t i = 0; i < JsonArray_GetSize(actions); ++i)
        {
            JsonObject *ao = JsonArray_GetObject(actions, i);
            ActionOperation *op = ActionOperation_New();
            if (op == NULL || ao == NULL)
            {
                continue;
            }
            if (RET_SUCCEEDED(ActionOperationCodec_Invoke_Result_Decode(ao, op)))
            {
                TinyList_AddTail(&result->actions, op);
            }
            else
            {
                ActionOperation_Delete(op);
            }
        }
    }
    return TINY_RET_OK;
}
