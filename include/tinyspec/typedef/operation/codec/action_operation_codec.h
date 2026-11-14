/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_OPERATION_CODEC_ACTION_OPERATION_CODEC_H
#define TINYSPEC_IOT_TYPEDEF_OPERATION_CODEC_ACTION_OPERATION_CODEC_H

#include <tinyspec/typedef/operation/action_operation.h>
#include <tinycore/json_object.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
bool ActionOperationCodec_Invoke_Query_Encode(ActionOperation *operation, JsonObject *content);

TINYSPEC_API
TINYCORE_RET ActionOperationCodec_Invoke_Query_Decode(JsonObject *content, ActionOperation *operation);

TINYSPEC_API
bool ActionOperationCodec_Invoke_Result_Encode(ActionOperation *operation, JsonObject *content);

TINYSPEC_API
TINYCORE_RET ActionOperationCodec_Invoke_Result_Decode(JsonObject *content, ActionOperation *operation);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_OPERATION_CODEC_ACTION_OPERATION_CODEC_H */
