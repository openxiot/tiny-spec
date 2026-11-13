/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_PRODUCT_HANDLER_PROPERTY_HANDLER_H
#define TINYSPEC_IOT_TYPEDEF_PRODUCT_HANDLER_PROPERTY_HANDLER_H

#include <tinyspec/iot_typedef/operation/property_operation.h>
#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef void (*PropertyOnGet)(PropertyOperation *operation, void *product);
typedef void (*PropertyOnSet)(PropertyOperation *operation, void *product);

/**
 * 属性读写 listener（单例）。
 *
 * 生成代码的最内层 doGet/doSet 通过 PropertyHandler_NotifyGet/NotifySet 派发到应用/BSP 层
 * 注册的物理读写实现（getter 填 operation->value；setter 消费 operation->value）。
 * 未注册 listener 时，Notify* 会置 operation->status = IOT_STATUS_INTERNAL_ERROR。
 */
typedef void (*PropertyListener)(PropertyOperation *operation, void *ctx);

TINYSPEC_API
void PropertyHandler_SetListener(PropertyListener getter, PropertyListener setter);

TINYSPEC_API
void PropertyHandler_NotifyGet(PropertyOperation *operation, void *ctx);

TINYSPEC_API
void PropertyHandler_NotifySet(PropertyOperation *operation, void *ctx);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_PRODUCT_HANDLER_PROPERTY_HANDLER_H */
