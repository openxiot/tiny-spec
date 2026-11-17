/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_CODEC_IQ_IQ_QUERY_CODEC_H
#define TINYSPEC_STANZA_CODEC_IQ_IQ_QUERY_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/iq_query.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
TinyRet IQQueryCodec_Encode(JsonObject *root, IQQuery *query);

TINYSPEC_API
TinyRet IQQueryCodec_Decode(IQQuery* query, JsonObject *object);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_CODEC_IQ_IQ_QUERY_CODEC_H */
