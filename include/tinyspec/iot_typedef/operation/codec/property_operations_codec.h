/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_OPERATION_CODEC_PROPERTY_OPERATIONS_CODEC_H
#define TINYSPEC_IOT_TYPEDEF_OPERATION_CODEC_PROPERTY_OPERATIONS_CODEC_H

#include <tinyspec/iot_typedef/operation/property_operations.h>
#include <tinycore/json_object.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
bool PropertyOperationsCodec_Set_Query_Encode(PropertyOperations *operations, JsonObject *content);

TINYSPEC_API
TINYCORE_RET PropertyOperationsCodec_Set_Query_Decode(JsonObject *content, PropertyOperations *operations);

TINYSPEC_API
bool PropertyOperationsCodec_Set_Result_Encode(PropertyOperations *operations, JsonObject *content);

TINYSPEC_API
TINYCORE_RET PropertyOperationsCodec_Set_Result_Decode(JsonObject *content, PropertyOperations *operations);

TINYSPEC_API
bool PropertyOperationsCodec_Get_Query_Encode(PropertyOperations *operations, JsonObject *content);

TINYSPEC_API
TINYCORE_RET PropertyOperationsCodec_Get_Query_Decode(JsonObject *content, PropertyOperations *operations);

TINYSPEC_API
bool PropertyOperationsCodec_Get_Result_Encode(PropertyOperations *operations, JsonObject *content);

TINYSPEC_API
TINYCORE_RET PropertyOperationsCodec_Get_Result_Decode(JsonObject *content, PropertyOperations *operations);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_OPERATION_CODEC_PROPERTY_OPERATIONS_CODEC_H */
