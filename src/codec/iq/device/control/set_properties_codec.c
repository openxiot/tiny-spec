/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/json_array.h>
#include <tinyspec/codec/iq/device/control/set_properties_codec.h>
#include <tinyspec/typedef/operation/codec/property_operations_codec.h>

bool SetPropertiesCodec_EncodeQuery(QuerySetProperties *query, JsonObject *content)
{
    return PropertyOperationsCodec_Set_Query_Encode(&query->properties, content);
}

TINYCORE_RET SetPropertiesCodec_DecodeQuery(JsonObject *content, QuerySetProperties *query)
{
    return PropertyOperationsCodec_Set_Query_Decode(content, &query->properties);
}

bool SetPropertiesCodec_EncodeResult(ResultSetProperties *result, JsonObject *content)
{
    return PropertyOperationsCodec_Set_Result_Encode(&result->properties, content);
}

TINYCORE_RET SetPropertiesCodec_DecodeResult(JsonObject *content, ResultSetProperties *result)
{
    return PropertyOperationsCodec_Set_Result_Decode(content, &result->properties);
}
