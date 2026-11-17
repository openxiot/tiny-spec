/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_CODEC_IQ_DEVICE_KEY_GET_ACCESS_KEY_CODEC_H
#define TINYSPEC_STANZA_CODEC_IQ_DEVICE_KEY_GET_ACCESS_KEY_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/device/key/get_access_key.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
bool GetAccessKeyCodec_EncodeResult(ResultGetAccessKey *result, JsonObject *content);
TINYSPEC_API
TinyRet GetAccessKeyCodec_DecodeResult(JsonObject *content, ResultGetAccessKey *result);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_CODEC_IQ_DEVICE_KEY_GET_ACCESS_KEY_CODEC_H */
