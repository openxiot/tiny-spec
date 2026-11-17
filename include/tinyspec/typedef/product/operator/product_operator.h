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
#include <tinyspec/typedef/product/product.h>
#include <tinyspec/typedef/operation/property_operations.h>
#include <tinyspec/typedef/operation/action_operations.h>
#include <tinyspec/stanza/iq/device/manager/get_children.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
PropertyOperations * Product_GetAllProperties(Product *thiz);

TINYSPEC_API
void Product_TryReadProperties(Product *thiz, PropertyOperations *operations);

TINYSPEC_API
void Product_TryWriteProperties(Product *thiz, PropertyOperations *operations);

TINYSPEC_API
void Product_TryInvokeAction(Product *thiz, ActionOperation *operation);

TINYSPEC_API
void Product_TryInvokeActions(Product *thiz, ActionOperations *operations);

TINYSPEC_API
void Product_TryGetChildren(Product *thiz, ResultGetChildren *children);

TINYSPEC_API
TinyRet Product_TryChangePropertyValue(Product *thiz, PropertyOperation *o, bool save);

TINYSPEC_API
TinyRet Product_TryProduceEvent(Product *thiz, EventOperation *o);

TINYSPEC_API
TinyRet Product_DoChangePropertyValue(Product *thiz, PropertyOperation *o);

TINYSPEC_API
PropertyOperations * Product_GetChangedProperties(Product *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_PRODUCT_OPERATOR_PRODUCT_OPERATOR_H */
