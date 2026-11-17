#ifndef TINYSPEC_CODEC_IQ_DEVICE_MANAGER_PROPERTIES_CHANGED_CODEC_H
#define TINYSPEC_CODEC_IQ_DEVICE_MANAGER_PROPERTIES_CHANGED_CODEC_H

#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/device/manager/properties_changed.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
bool PropertiesChangedCodec_EncodeQuery(QueryPropertiesChanged *query, JsonObject *content);
TINYSPEC_API
TinyRet PropertiesChangedCodec_DecodeQuery(JsonObject *content, QueryPropertiesChanged *query);
TINYSPEC_API
bool PropertiesChangedCodec_EncodeResult(ResultPropertiesChanged *result, JsonObject *content);
TINYSPEC_API
TinyRet PropertiesChangedCodec_DecodeResult(JsonObject *content, ResultPropertiesChanged *result);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_CODEC_IQ_DEVICE_MANAGER_PROPERTIES_CHANGED_CODEC_H */
