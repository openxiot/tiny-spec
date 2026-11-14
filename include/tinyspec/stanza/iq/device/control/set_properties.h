/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_DEVICE_CONTROL_SET_PROPERTIES_H
#define TINYSPEC_STANZA_IQ_DEVICE_CONTROL_SET_PROPERTIES_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/operation/property_operations.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    PropertyOperations      properties;
} QuerySetProperties;

TINYSPEC_API
TINYCORE_RET QuerySetProperties_Construct(QuerySetProperties *thiz);
TINYSPEC_API
void QuerySetProperties_Dispose(QuerySetProperties *thiz);

typedef struct
{
    PropertyOperations      properties;
} ResultSetProperties;

TINYSPEC_API
TINYCORE_RET ResultSetProperties_Construct(ResultSetProperties *thiz);
TINYSPEC_API
void ResultSetProperties_Dispose(ResultSetProperties *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_CONTROL_SET_PROPERTIES_H */
