#ifndef TINYSPEC_CODEC_IQ_DEVICE_MANAGER_GET_CHILDREN_CODEC_H
#define TINYSPEC_CODEC_IQ_DEVICE_MANAGER_GET_CHILDREN_CODEC_H

#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/device/manager/get_children.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
bool GetChildrenCodec_EncodeQuery(QueryGetChildren *query, JsonObject *content);
TINYSPEC_API
TinyRet GetChildrenCodec_DecodeQuery(JsonObject *content, QueryGetChildren *query);
TINYSPEC_API
bool GetChildrenCodec_EncodeResult(ResultGetChildren *result, JsonObject *content);
TINYSPEC_API
bool GetChildrenCodec_EncodeChild(const Child *child, JsonObject *obj);
TINYSPEC_API
TinyRet GetChildrenCodec_DecodeResult(JsonObject *content, ResultGetChildren *result);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_CODEC_IQ_DEVICE_MANAGER_GET_CHILDREN_CODEC_H */
