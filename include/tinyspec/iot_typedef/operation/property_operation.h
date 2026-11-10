/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_OPERATION_PROPERTY_OPERATION_H
#define TINYSPEC_IOT_TYPEDEF_OPERATION_PROPERTY_OPERATION_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/json_value.h>
#include <tinyspec/iot_typedef/xid/property_id.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    PropertyId      pid;
    JsonValue     * value;
    int             status;
    char            description[IOT_DESCRIPTION_LENGTH];
} PropertyOperation;

TINYSPEC_API
PropertyOperation * PropertyOperation_New(void);

TINYSPEC_API
PropertyOperation * PropertyOperation_NewFrom(const char *did, uint16_t siid, uint16_t piid);

TINYSPEC_API
PropertyOperation * PropertyOperation_NewValue(const char *did, uint16_t siid, uint16_t piid, JsonValue *value);

TINYSPEC_API
PropertyOperation * PropertyOperation_CopyFrom(PropertyOperation *other);

TINYSPEC_API
TINYCORE_RET PropertyOperation_Copy(PropertyOperation *thiz, PropertyOperation *other);

TINYSPEC_API
void PropertyOperation_Delete(PropertyOperation *thiz);

TINYSPEC_API
TINYCORE_RET PropertyOperation_Construct(PropertyOperation *thiz);

TINYSPEC_API
void PropertyOperation_Dispose(PropertyOperation *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_OPERATION_PROPERTY_OPERATION_H */
