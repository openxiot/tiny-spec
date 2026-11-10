/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_CODEC_IQ_NODE_CONTROL_GET_NODES_CODEC_H
#define TINYSPEC_STANZA_CODEC_IQ_NODE_CONTROL_GET_NODES_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/node/control/get_nodes.h>

TINYSPEC_BEGIN_DECLS

//bool GetNodesCodec_EncodeQuery(QueryGetNodes *query, JsonObject *content);
//TINYCORE_RET GetNodesCodec_DecodeQuery(JsonObject *content, QueryGetNodes *query);

TINYSPEC_API
bool GetNodesCodec_EncodeResult(ResultGetNodes *result, JsonObject *content);

TINYSPEC_API
TINYCORE_RET GetNodesCodec_DecodeResult(JsonObject *content, ResultGetNodes *result);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_CODEC_IQ_NODE_CONTROL_GET_NODES_CODEC_H */
