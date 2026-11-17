/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_TYPEDEF_SEND_H
#define TINYSPEC_TYPEDEF_SEND_H

#include <tinycore/json.h>
#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

/*
 * 传输回调：由应用注入到 product->send，负责把一个已编码好的 JSON
 * 对象发给对端（如 WSS 帧）。product/sendCtx 由 dispatch 原样透传。
 * 返回非 0 表示发送失败。
 */
typedef int (*DataSend)(JsonObject *o, void *product, void *ctx);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_TYPEDEF_SEND_H */
