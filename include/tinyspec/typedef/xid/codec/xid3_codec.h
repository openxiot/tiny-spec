/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_XID_CODEC_XID3_CODEC_H
#define TINYSPEC_IOT_TYPEDEF_XID_CODEC_XID3_CODEC_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/xid/xid3.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
TINYCORE_RET XID3_FromString(XID3 *xid, const char *string);
TINYSPEC_API
void XID3_ToString(char string[IOT_XID_LENGTH], XID3 *xid);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_XID_CODEC_XID3_CODEC_H */
