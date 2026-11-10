/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/iot_typedef/device/urn/urn.h>
#include <tinyspec/iot_typedef/device/property/unit/unit.h>
#include <tinyspec/iot_typedef/device/property/access/access.h>
#include <tinyspec/iot_typedef/device/property/format/format.h>
#include <tinyspec/iot_typedef/device/property/constraint/value_list.h>
#include <tinyspec/iot_typedef/device/property/constraint/value_range.h>
#include <tinyspec/iot_typedef/operation/property_operation.h>
#include <tinycore/json_value.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    uint16_t            iid;
    Urn                 type;
    Format              format;
    Access              access;
    ValueList         * valueList;
    ValueRange        * valueRange;
    Unit                unit;
    JsonValue         * value;
} Property;

TINYSPEC_API
Property* Property_New(uint16_t iid);

TINYSPEC_API
Property *Property_NewInstance(uint16_t iid,
                               const char *ns,
                               const char *name,
                               uint32_t uuid,
                               const char *vendor,
                               Format format,
                               Access access,
                               Unit unit);

TINYSPEC_API
void Property_Delete(Property *thiz);

TINYSPEC_API
bool Property_CheckValue(Property *thiz, JsonValue* value);

TINYSPEC_API
void Property_TryRead(Property *thiz, PropertyOperation *o);

TINYSPEC_API
void Property_TryWrite(Property *thiz, PropertyOperation *o);

TINYSPEC_API
void Property_TryChange(Property *thiz, PropertyOperation *o, bool save);

TINYSPEC_API
void Property_TrySubscribe(Property *thiz, PropertyOperation *o);

TINYSPEC_API
void Property_TryUnsubscribe(Property *thiz, PropertyOperation *o);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_H */
