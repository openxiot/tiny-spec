/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_XID_CODEC_ACTION_ID_CODEC_H
#define TINYSPEC_IOT_TYPEDEF_XID_CODEC_ACTION_ID_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/xid/codec/xid3_codec.h>

TINYSPEC_BEGIN_DECLS

#define ActionId_FromString(x, y)   XID3_FromString(x, y)
#define ActionId_ToString(x, y)     XID3_ToString(x, y)

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_XID_CODEC_ACTION_ID_CODEC_H */
