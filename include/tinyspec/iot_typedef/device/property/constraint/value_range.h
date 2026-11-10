/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_CONSTRAINT_VALUE_RANGE_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_CONSTRAINT_VALUE_RANGE_H

#include <tinycore/json_value.h>
#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    JsonValue   *min;
    JsonValue   *max;
    JsonValue   *step;
} ValueRange;

TINYSPEC_API
ValueRange* ValueRange_New(JsonValue *min, JsonValue *max, JsonValue *step);

TINYSPEC_API
ValueRange* ValueRange_NewFrom(JsonArray *range);

TINYSPEC_API
void ValueRange_Delete(ValueRange *thiz);

TINYSPEC_API
bool ValueRange_CheckValue(ValueRange *thiz, JsonValue *value);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_PROPERTY_CONSTRAINT_VALUE_RANGE_H */
