#ifndef TINYSPEC_CODEC_IQ_DEVICE_CONTROL_INVOKE_ACTIONS_CODEC_H
#define TINYSPEC_CODEC_IQ_DEVICE_CONTROL_INVOKE_ACTIONS_CODEC_H

#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/device/control/invoke_actions.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
bool InvokeActionsCodec_EncodeQuery(QueryInvokeActions *query, JsonObject *content);
TINYSPEC_API
TinyRet InvokeActionsCodec_DecodeQuery(JsonObject *content, QueryInvokeActions *query);
TINYSPEC_API
bool InvokeActionsCodec_EncodeResult(ResultInvokeActions *result, JsonObject *content);
TINYSPEC_API
TinyRet InvokeActionsCodec_DecodeResult(JsonObject *content, ResultInvokeActions *result);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_CODEC_IQ_DEVICE_CONTROL_INVOKE_ACTIONS_CODEC_H */
