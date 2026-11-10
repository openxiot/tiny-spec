/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_PRODUCT_OPERATOR_PRODUCT_OPERATOR_H
#define TINYSPEC_IOT_TYPEDEF_PRODUCT_OPERATOR_PRODUCT_OPERATOR_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/iot_typedef/product/product.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
PropertyOperations * Product_GetAllProperties(Product *thiz);

TINYSPEC_API
void Product_TryReadProperties(Product *thiz, PropertyOperations *operations);

TINYSPEC_API
void Product_TryWriteProperties(Product *thiz, PropertyOperations *operations);

#if 0
TINYSPEC_API
void Device_TrySubscribeProperties(Product *thiz, PropertyOperations *operations);

TINYSPEC_API
void Device_TryUnsubscribeProperties(Product *thiz, PropertyOperations *operations);
#endif

TINYSPEC_API
void Product_TryInvokeAction(Product *thiz, ActionOperation *operation);

TINYSPEC_API
TINYCORE_RET Product_TryChangePropertyValue(Product *thiz, PropertyOperation *o, bool save);

TINYSPEC_API
TINYCORE_RET Product_TryProduceEvent(Product *thiz, EventOperation *o);

TINYSPEC_API
TINYCORE_RET Product_DoChangePropertyValue(Product *thiz, PropertyOperation *o);

TINYSPEC_API
PropertyOperations * Product_GetChangedProperties(Product *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_PRODUCT_OPERATOR_PRODUCT_OPERATOR_H */
