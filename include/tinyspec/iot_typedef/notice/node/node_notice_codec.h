/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_NODE_NODE_NOTICE_CODEC_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_NODE_NODE_NOTICE_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/iot_typedef/notice/node/node_notice.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
TINYCORE_RET NodeNoticeCodec_Encode(JsonObject *object, int type, NodeNotice *notice);
TINYSPEC_API
TINYCORE_RET NodeNoticeCodec_Decode(NodeNotice *notice, int type, JsonObject *payload);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_NODE_NODE_NOTICE_CODEC_H */
