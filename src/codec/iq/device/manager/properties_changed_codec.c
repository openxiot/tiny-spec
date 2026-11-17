#include <tinycore/tiny_log.h>
#include <tinyspec/codec/iq/device/manager/properties_changed_codec.h>
#include <tinyspec/typedef/operation/codec/property_operations_codec.h>

bool PropertiesChangedCodec_EncodeQuery(QueryPropertiesChanged *query, JsonObject *content)
{
    return PropertyOperationsCodec_Set_Query_Encode(&query->properties, content);
}

TINYCORE_RET PropertiesChangedCodec_DecodeQuery(JsonObject *content, QueryPropertiesChanged *query)
{
    return PropertyOperationsCodec_Set_Query_Decode(content, &query->properties);
}

bool PropertiesChangedCodec_EncodeResult(ResultPropertiesChanged *result, JsonObject *content)
{
    return PropertyOperationsCodec_Set_Result_Encode(&result->properties, content);
}

TINYCORE_RET PropertiesChangedCodec_DecodeResult(JsonObject *content, ResultPropertiesChanged *result)
{
    return PropertyOperationsCodec_Set_Result_Decode(content, &result->properties);
}
