/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_OPERATION_PROPERTY_OPERATIONS_H
#define TINYSPEC_IOT_TYPEDEF_OPERATION_PROPERTY_OPERATIONS_H

#include <tinycore/tiny_list.h>
#include <tinyspec/iot_typedef/xid/property_id.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    TinyList        properties;
} PropertyOperations;

TINYSPEC_API
PropertyOperations * PropertyOperations_New(void);

TINYSPEC_API
void PropertyOperations_Delete(PropertyOperations *thiz);

TINYSPEC_API
TINYCORE_RET PropertyOperations_Construct(PropertyOperations *thiz);

TINYSPEC_API
void PropertyOperations_Dispose(PropertyOperations *thiz);

TINYSPEC_API
TINYCORE_RET PropertyOperations_Copy(PropertyOperations *dst, PropertyOperations *src);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_OPERATION_PROPERTY_OPERATIONS_H */
