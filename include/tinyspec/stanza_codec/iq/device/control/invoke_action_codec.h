/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_INVOKE_ACTION_CODEC_H
#define TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_INVOKE_ACTION_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/device/control/invoke_action.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
bool InvokeActionCodec_EncodeQuery(QueryInvokeAction *query, JsonObject *content);

TINYSPEC_API
TINYCORE_RET InvokeActionCodec_DecodeQuery(JsonObject *content, QueryInvokeAction *query);

TINYSPEC_API
bool InvokeActionCodec_EncodeResult(ResultInvokeAction *result, JsonObject *content);

TINYSPEC_API
TINYCORE_RET InvokeActionCodec_DecodeResult(JsonObject *content, ResultInvokeAction *result);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_INVOKE_ACTION_CODEC_H */
