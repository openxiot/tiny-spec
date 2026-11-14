/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/json_array.h>
#include <tinyspec/codec/iq/device/control/get_properties_codec.h>
#include <tinyspec/typedef/operation/codec/property_operations_codec.h>

bool GetPropertiesCodec_EncodeQuery(QueryGetProperties *query, JsonObject *content)
{
    return PropertyOperationsCodec_Get_Query_Encode(&query->properties, content);
}

TINYCORE_RET GetPropertiesCodec_DecodeQuery(JsonObject *content, QueryGetProperties *query)
{
    return PropertyOperationsCodec_Get_Query_Decode(content, &query->properties);
}

bool GetPropertiesCodec_EncodeResult(ResultGetProperties *result, JsonObject *content)
{
    return PropertyOperationsCodec_Get_Result_Encode(&result->properties, content);
}

TINYCORE_RET GetPropertiesCodec_DecodeResult(JsonObject *content, ResultGetProperties *result)
{
    return PropertyOperationsCodec_Get_Result_Decode(content, &result->properties);
}
