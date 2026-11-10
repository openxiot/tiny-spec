/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_NODE_IMPL_NODE_CHANGED_CODEC_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_NODE_IMPL_NODE_CHANGED_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/iot_typedef/node/node.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
TINYCORE_RET NodeInfoCodec_Encode(JsonObject *payload, Node *node);
TINYSPEC_API
TINYCORE_RET NodeInfoCodec_Decode(Node *node, JsonObject *payload);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_NODE_IMPL_NODE_CHANGED_CODEC_H */
