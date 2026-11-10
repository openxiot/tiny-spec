/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_log.h>
#include <tinycore/json_string.h>
#include <tinyspec/stanza_codec/iq/device/control/invoke_action_codec.h>
#include <tinyspec/iot_typedef/operation/codec/action_operation_codec.h>

bool InvokeActionCodec_EncodeQuery(QueryInvokeAction *query, JsonObject *content)
{
    return ActionOperationCodec_Invoke_Query_Encode(&query->action, content);
}

TINYCORE_RET InvokeActionCodec_DecodeQuery(JsonObject *content, QueryInvokeAction *query)
{
    return ActionOperationCodec_Invoke_Query_Decode(content, &query->action);
}

bool InvokeActionCodec_EncodeResult(ResultInvokeAction *result, JsonObject *content)
{
    return ActionOperationCodec_Invoke_Result_Encode(&result->action, content);
}

TINYCORE_RET InvokeActionCodec_DecodeResult(JsonObject *content, ResultInvokeAction *result)
{
    return ActionOperationCodec_Invoke_Result_Decode(content, &result->action);
}
