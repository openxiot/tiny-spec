/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_GET_PROPERTY_CODEC_H
#define TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_GET_PROPERTY_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/device/control/get_property.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
bool GetPropertyCodec_EncodeQuery(QueryGetProperty *query, JsonObject *content);

TINYSPEC_API
TinyRet GetPropertyCodec_DecodeQuery(JsonObject *content, QueryGetProperty *query);

TINYSPEC_API
bool GetPropertyCodec_EncodeResult(ResultGetProperty *result, JsonObject *content);

TINYSPEC_API
TinyRet GetPropertyCodec_DecodeResult(JsonObject *content, ResultGetProperty *result);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_GET_PROPERTY_CODEC_H */
