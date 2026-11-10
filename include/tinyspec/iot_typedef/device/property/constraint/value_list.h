/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_CONSTRAINT_VALUE_LIST_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_CONSTRAINT_VALUE_LIST_H

#include <tinycore/tiny_list.h>
#include <tinycore/json_value.h>
#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    TinyList        list;
} ValueList;

TINYSPEC_API
ValueList* ValueList_New(void);

TINYSPEC_API
ValueList* ValueList_NewFrom(JsonArray *array);

TINYSPEC_API
void ValueList_Delete(ValueList *thiz);

TINYSPEC_API
TINYCORE_RET ValueList_Put(ValueList *thiz, JsonValue *value);

TINYSPEC_API
bool ValueList_CheckValue(ValueList *thiz, JsonValue *value);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_CONSTRAINT_VALUE_LIST_H */
