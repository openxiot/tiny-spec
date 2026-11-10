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

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_PRODUCT_HANDLER_PROPERTY_HANDLER_H */
