/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_alloc.h>
#include <tinycore/tiny_log.h>
#include <tinycore/json_number.h>
#include <tinycore/json_array.h>
#include <tinycore/json_string.h>
#include <tinyspec/typedef/device/property/constraint/value_range.h>

static TINYCORE_RET ValueRange_Construct(ValueRange *thiz, JsonValue *min, JsonValue *max, JsonValue *step)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(min, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(max, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(step, TINY_RET_E_NULL_PTR);

    do
    {
        memset(thiz, 0, sizeof(ValueRange));

        if (min->type != max->type || min->type != step->type)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        thiz->min = JsonValue_NewFrom(min);
        if (thiz->min == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        thiz->max = JsonValue_NewFrom(max);
        if (thiz->max == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        thiz->step = JsonValue_NewFrom(step);
        if (thiz->step == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }
    } while (false);

    return ret;
}

static TINYCORE_RET ValueRange_Construct2(ValueRange *thiz, JsonArray *range)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(range, TINY_RET_E_NULL_PTR);

    do
    {
        JsonValue *min = NULL;
        JsonValue *max = NULL;
        JsonValue *step = NULL;

        memset(thiz, 0, sizeof(ValueRange));

        if (range->values.size != 3)
        {
            TINY_LOG_ERROR("range.length invalid: %d", (int) (range->values.size));
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        min = (JsonValue *)TinyList_GetAt(&range->values, 0);
        max = (JsonValue *)TinyList_GetAt(&range->values, 1);
        step = (JsonValue *)TinyList_GetAt(&range->values, 2);

        if (min->type != max->type || min->type != step->type)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        thiz->min = JsonValue_NewFrom(min);
        if (thiz->min == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        thiz->max = JsonValue_NewFrom(max);
        if (thiz->max == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        thiz->step = JsonValue_NewFrom(step);
        if (thiz->step == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }
    } while (false);

    return ret;
}

static void ValueRange_Dispose(ValueRange *thiz)
{
    RETURN_IF_FAIL(thiz);

    if (thiz->min != NULL)
    {
        JsonValue_Delete(thiz->min);
        thiz->min = NULL;
    }

    if (thiz->max != NULL)
    {
        JsonValue_Delete(thiz->max);
        thiz->max = NULL;
    }

    if (thiz->step != NULL)
    {
        JsonValue_Delete(thiz->step);
        thiz->step = NULL;
    }
}

ValueRange* ValueRange_New(JsonValue *min, JsonValue *max, JsonValue *step)
{
    ValueRange *thiz = NULL;

    RETURN_VAL_IF_FAIL(min, NULL);
    RETURN_VAL_IF_FAIL(max, NULL);
    RETURN_VAL_IF_FAIL(step, NULL);

    do
    {
        thiz = (ValueRange *) tiny_malloc(sizeof(ValueRange));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(ValueRange_Construct(thiz, min, max, step)))
        {
            ValueRange_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

ValueRange *ValueRange_NewFrom(JsonArray *range)
{
    ValueRange *thiz = NULL;

    RETURN_VAL_IF_FAIL(range, NULL);

    do
    {
        thiz = (ValueRange *) tiny_malloc(sizeof(ValueRange));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(ValueRange_Construct2(thiz, range)))
        {
            ValueRange_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void ValueRange_Delete(ValueRange *thiz)
{
    RETURN_IF_FAIL(thiz);

    ValueRange_Dispose(thiz);
    tiny_free(thiz);
}

static bool ValueRange_CheckIntegerValue(ValueRange *thiz, long value)
{
    long min = 0;
    long max = 0;
    long step = 0;

    RETURN_VAL_IF_FAIL(thiz, false);

    TINY_LOG_DEBUG("ValueRange_CheckIntegerValue: %ld", value);

    min = thiz->min->data.number->value.intValue;
    max = thiz->max->data.number->value.intValue;
    step = thiz->step->data.number->value.intValue;

    // fix step value
    if (step == 0)
    {
        step = 1;
    }

    for (long v = min; v <= max; v += step)
    {
        if (v == value)
        {
            return true;
        }
    }

    TINY_LOG_ERROR("ValueRange_CheckIntegerValue FAILED, invalid value: %ld", value);

    return false;
}

static bool ValueRange_CheckFloatValue(ValueRange *thiz, double value)
{
    double v = 0.0;
    double max = 0.0;
    double step = 0.0;

    RETURN_VAL_IF_FAIL(thiz, false);

    TINY_LOG_DEBUG("ValueRange_CheckFloatValue: %f", value);

    v = thiz->min->data.number->value.floatValue;
    max = thiz->max->data.number->value.floatValue;
    step = thiz->step->data.number->value.floatValue;

    // fix step value
    if (step < 0.0001)
    {
        step = 0.0001;
    }

    while (v < max)
    {
        double difference = (v - value);
        if (difference > -0.0001 && difference < 0.0001)
        {
            return true;
        }

        v += step;
    }

    TINY_LOG_ERROR("ValueRange_CheckFloatValue FAILED, invalid value: %f", value);

    return false;
}

static bool ValueRange_CheckHexValue(ValueRange *thiz, JsonString *hex)
{
    char *stop = NULL;
    uint32_t min = 0;
    uint32_t max = 0;
    uint32_t step = 0;
    uint32_t value = 0;

    RETURN_VAL_IF_FAIL(thiz, false);
    RETURN_VAL_IF_FAIL(hex, false);

    TINY_LOG_DEBUG("ValueRange_CheckHexValue: %s", hex->value);

    if (thiz->min == NULL || thiz->max == NULL || thiz->step == NULL)
    {
        TINY_LOG_ERROR("check hex value error: min/max/step is null");
        return false;
    }

    if (thiz->min->type != JSON_STRING)
    {
        TINY_LOG_ERROR("check hex value error: ValueRange not hex");
        return false;
    }

    if (thiz->min->data.string->value == NULL || thiz->max->data.string->value == NULL || thiz->step->data.string->value == NULL)
    {
        TINY_LOG_ERROR("check hex value error: min/max/step.string is null");
        return false;
    }

    min = (uint32_t) strtol(thiz->min->data.string->value, &stop, 16);
    max = (uint32_t) strtol(thiz->max->data.string->value, &stop, 16);
    step = (uint32_t) strtol(thiz->step->data.string->value, &stop, 16);
    value = (uint32_t) strtol(hex->value, &stop, 16);

    TINY_LOG_DEBUG("check hex value: (%d, %d, %d), value = %d", (int) min, (int) max, (int) step, (int) value);

    for (uint32_t v = min; v <= max; v += step)
    {
        if (value == v)
        {
            return true;
        }
    }

    TINY_LOG_ERROR("ValueRange_CheckHexValue FAILED, invalid value: %s", hex->value);

    return false;
}

static bool ValueRange_CheckNumberValue(ValueRange *thiz, JsonNumber *number)
{
    RETURN_VAL_IF_FAIL(thiz, false);
    RETURN_VAL_IF_FAIL(number, false);

    if (thiz->min == NULL || thiz->max == NULL || thiz->step == NULL)
    {
        TINY_LOG_ERROR("check number value error, min/max/step is null");
        return false;
    }

    if (thiz->min->type != JSON_NUMBER)
    {
        TINY_LOG_ERROR("check number value error, min/max/step.type is not JsonNumber");
        return false;
    }

    // thiz.integer : value.float
    if (thiz->min->data.number->type == JSON_NUMBER_INTEGER && number->type == JSON_NUMBER_FLOAT)
    {
        TINY_LOG_ERROR("check number value error, value type invalid: %d", number->type);
        return false;
    }

    // thiz.integer: value.integer
    if (thiz->min->data.number->type == JSON_NUMBER_INTEGER)
    {
        return ValueRange_CheckIntegerValue(thiz, number->value.intValue);
    }

    // thiz.float: value.integer
    if (number->type == JSON_NUMBER_INTEGER)
    {
        return ValueRange_CheckFloatValue(thiz, (double) number->value.intValue);
    }

    // thiz.float: value.float
    return ValueRange_CheckFloatValue(thiz, number->value.floatValue);
}

bool ValueRange_CheckValue(ValueRange *thiz, JsonValue *value)
{
    RETURN_VAL_IF_FAIL(thiz, false);
    RETURN_VAL_IF_FAIL(value, false);

    TINY_LOG_DEBUG("ValueRange_CheckValue");

    switch (value->type)
    {
        case JSON_STRING:
            return ValueRange_CheckHexValue(thiz, value->data.string);

        case JSON_NUMBER:
            return ValueRange_CheckNumberValue(thiz, value->data.number);

        default:
            TINY_LOG_ERROR("ValueRange_CheckValue FAILED, value type error: %d", value->type);
            return false;
    }
}
