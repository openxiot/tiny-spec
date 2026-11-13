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
#include <tinycore/tiny_list.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    uint16_t            iid;
    Urn                 type;
    Format              format;
    Access              access;
    ValueList         * valueList;
    ValueRange        * valueRange;
    char              * unit;           /* 单位字符串，可为自定义任意单位；NULL 表示无 */
    uint16_t            maxLength;      /* FORMAT_STRING 最大长度，0 表示不限制 */
    JsonValue         * value;
    TinyList            members;        /* 组合属性(FORMAT_COMBINATION)的成员 Property* 列表（非持有） */
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

/** 设置属性单位（字符串，可为自定义任意单位）。内部拷贝；NULL 清除单位。 */
TINYSPEC_API
void Property_SetUnit(Property *thiz, const char *unit);

/** 组合属性：添加一个成员属性（非持有引用，成员须存活于同服务）。 */
TINYSPEC_API
TINYCORE_RET Property_MemberAdd(Property *thiz, Property *member);

TINYSPEC_API
uint32_t Property_GetMemberCount(Property *thiz);

TINYSPEC_API
Property* Property_GetMemberAt(Property *thiz, uint32_t index);

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
