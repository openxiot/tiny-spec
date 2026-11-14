/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_CODEC_IQ_IQ_RESULT_CODEC_H
#define TINYSPEC_STANZA_CODEC_IQ_IQ_RESULT_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/iq_result.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
TINYCORE_RET IQResultCodec_Encode(JsonObject *root, IQResult *query);

TINYSPEC_API
TINYCORE_RET IQResultCodec_Decode(IQResult* result, JsonObject *object);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_CODEC_IQ_IQ_RESULT_CODEC_H */
