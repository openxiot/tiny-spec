/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_SET_PROPERTY_CODEC_H
#define TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_SET_PROPERTY_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/device/control/set_property.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
bool SetPropertyCodec_EncodeQuery(QuerySetProperty *query, JsonObject *content);

TINYSPEC_API
TINYCORE_RET SetPropertyCodec_DecodeQuery(JsonObject *content, QuerySetProperty *query);

TINYSPEC_API
bool SetPropertyCodec_EncodeResult(ResultSetProperty *result, JsonObject *content);

TINYSPEC_API
TINYCORE_RET SetPropertyCodec_DecodeResult(JsonObject *content, ResultSetProperty *result);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_SET_PROPERTY_CODEC_H */
