/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_DEVICE_CONTROL_GET_PROPERTY_H
#define TINYSPEC_STANZA_IQ_DEVICE_CONTROL_GET_PROPERTY_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/operation/property_operation.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    PropertyOperation      property;
} QueryGetProperty;

TINYSPEC_API
TinyRet QueryGetProperty_Construct(QueryGetProperty *thiz);
TINYSPEC_API
void QueryGetProperty_Dispose(QueryGetProperty *thiz);

typedef struct
{
    PropertyOperation      property;
} ResultGetProperty;

TINYSPEC_API
TinyRet ResultGetProperty_Construct(ResultGetProperty *thiz);
TINYSPEC_API
void ResultGetProperty_Dispose(ResultGetProperty *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_CONTROL_GET_PROPERTY_H */
