/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_OWNER_OWNER_NOTICE_CODEC_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_OWNER_OWNER_NOTICE_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/iot_typedef/notice/owner/owner_notice.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
TINYCORE_RET OwnerNoticeCodec_Encode(JsonObject *payload, int type, OwnerNotice *notice);

TINYSPEC_API
TINYCORE_RET OwnerNoticeCodec_Decode(OwnerNotice *notice, int type, JsonObject *payload);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_OWNER_OWNER_NOTICE_CODEC_H */
