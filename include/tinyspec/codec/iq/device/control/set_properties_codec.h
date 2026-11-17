/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_SET_PROPERTIES_CODEC_H
#define TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_SET_PROPERTIES_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/device/control/set_properties.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
bool SetPropertiesCodec_EncodeQuery(QuerySetProperties *query, JsonObject *content);

TINYSPEC_API
TinyRet SetPropertiesCodec_DecodeQuery(JsonObject *content, QuerySetProperties *query);

TINYSPEC_API
bool SetPropertiesCodec_EncodeResult(ResultSetProperties *result, JsonObject *content);

TINYSPEC_API
TinyRet SetPropertiesCodec_DecodeResult(JsonObject *content, ResultSetProperties *result);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_CODEC_IQ_DEVICE_CONTROL_SET_PROPERTIES_CODEC_H */
