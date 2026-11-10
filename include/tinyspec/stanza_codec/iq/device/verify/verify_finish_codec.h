/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_CODEC_IQ_DEVICE_VERIFY_VERIFY_FINISH_CODEC_H
#define TINYSPEC_STANZA_CODEC_IQ_DEVICE_VERIFY_VERIFY_FINISH_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/device/verify/verify_finish.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
bool VerifyFinishCodec_EncodeQuery(QueryVerifyFinish *query, JsonObject *content);
TINYSPEC_API
TINYCORE_RET VerifyFinishCodec_DecodeQuery(JsonObject *content, QueryVerifyFinish *query);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_CODEC_IQ_DEVICE_VERIFY_VERIFY_FINISH_CODEC_H */
