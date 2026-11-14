/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_DEVICE_CONTROL_GET_PROPERTIES_H
#define TINYSPEC_STANZA_IQ_DEVICE_CONTROL_GET_PROPERTIES_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/operation/property_operations.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    PropertyOperations      properties;
} QueryGetProperties;

TINYSPEC_API
TINYCORE_RET QueryGetProperties_Construct(QueryGetProperties *thiz);
TINYSPEC_API
void QueryGetProperties_Dispose(QueryGetProperties *thiz);

typedef struct
{
    PropertyOperations      properties;
} ResultGetProperties;

TINYSPEC_API
TINYCORE_RET ResultGetProperties_Construct(ResultGetProperties *thiz);
TINYSPEC_API
void ResultGetProperties_Dispose(ResultGetProperties *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_CONTROL_GET_PROPERTIES_H */
