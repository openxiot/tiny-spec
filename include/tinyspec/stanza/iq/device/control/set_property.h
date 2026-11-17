/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_DEVICE_CONTROL_SET_PROPERTY_H
#define TINYSPEC_STANZA_IQ_DEVICE_CONTROL_SET_PROPERTY_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/operation/property_operation.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    PropertyOperation      property;
} QuerySetProperty;

TINYSPEC_API
TinyRet QuerySetProperty_Construct(QuerySetProperty *thiz);
TINYSPEC_API
void QuerySetProperty_Dispose(QuerySetProperty *thiz);

typedef struct
{
    PropertyOperation      property;
} ResultSetProperty;

TINYSPEC_API
TinyRet ResultSetProperty_Construct(ResultSetProperty *thiz);
TINYSPEC_API
void ResultSetProperty_Dispose(ResultSetProperty *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_CONTROL_SET_PROPERTY_H */
