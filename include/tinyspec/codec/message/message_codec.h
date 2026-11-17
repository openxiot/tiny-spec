/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_CODEC_MESSAGE_MESSAGE_CODEC_H
#define TINYSPEC_STANZA_CODEC_MESSAGE_MESSAGE_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_object.h>
#include <tinyspec/stanza/stanza.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
TinyRet MessageCodec_Encode(JsonObject *root, Message *message);

TINYSPEC_API
TinyRet MessageCodec_Decode(Message* message, JsonObject *object);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_CODEC_MESSAGE_MESSAGE_CODEC_H */
