/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include "tiny_test.h"

#include <string.h>

#include <tinycore/json_array.h>
#include <tinycore/json_number.h>
#include <tinycore/json_object.h>
#include <tinycore/json_value.h>
#include <tinycore/tiny_list.h>
#include <tinyspec/tinyspec.h>

static const Format ALL_FORMATS[] = {
    FORMAT_UNDEFINED,
    FORMAT_BOOL,
    FORMAT_STRING,
    FORMAT_FLOAT,
    FORMAT_UINT8,
    FORMAT_UINT16,
    FORMAT_UINT32,
    FORMAT_INT8,
    FORMAT_INT16,
    FORMAT_INT32,
    FORMAT_INT64,
    FORMAT_HEX,
    FORMAT_TLV8,
    FORMAT_INT
};

static void test_format_strings(void)
{
    const uint32_t count = (uint32_t) (sizeof(ALL_FORMATS) / sizeof(ALL_FORMATS[0]));
    uint32_t i = 0;

    for (i = 0; i < count; ++i)
    {
        const char *string = Format_ToString(ALL_FORMATS[i]);

        TINY_TEST_CHECK(string != NULL);
        TINY_TEST_CHECK(Format_FromString(string) == ALL_FORMATS[i]);
    }

    TINY_TEST_CHECK(strcmp(Format_ToString(FORMAT_UNDEFINED), "undefined") == 0);
    TINY_TEST_CHECK(strcmp(Format_ToString(FORMAT_BOOL), "bool") == 0);
    TINY_TEST_CHECK(strcmp(Format_ToString(FORMAT_STRING), "string") == 0);
    TINY_TEST_CHECK(strcmp(Format_ToString(FORMAT_UINT16), "uint16") == 0);
    TINY_TEST_CHECK(strcmp(Format_ToString(FORMAT_INT64), "int64") == 0);
    TINY_TEST_CHECK(strcmp(Format_ToString(FORMAT_HEX), "hex") == 0);
    TINY_TEST_CHECK(strcmp(Format_ToString(FORMAT_TLV8), "tlv8") == 0);

    TINY_TEST_CHECK(Format_FromString("no-such-format") == FORMAT_UNDEFINED);
    TINY_TEST_CHECK(Format_FromString("") == FORMAT_UNDEFINED);
}

static void test_format_integer(void)
{
    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_UINT8, 0));
    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_UINT8, 255));
    TINY_TEST_CHECK(!Format_CheckInteger(FORMAT_UINT8, 256));
    TINY_TEST_CHECK(!Format_CheckInteger(FORMAT_UINT8, -1));

    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_UINT16, 65535));
    TINY_TEST_CHECK(!Format_CheckInteger(FORMAT_UINT16, 65536));

    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_UINT32, 2147483647L));
    TINY_TEST_CHECK(!Format_CheckInteger(FORMAT_UINT32, -1));

    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_INT8, -127));
    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_INT8, 127));
    TINY_TEST_CHECK(!Format_CheckInteger(FORMAT_INT8, 128));
    TINY_TEST_CHECK(!Format_CheckInteger(FORMAT_INT8, -128));

    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_INT16, -32767));
    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_INT16, 32767));
    TINY_TEST_CHECK(!Format_CheckInteger(FORMAT_INT16, 32768));

    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_INT32, 2147483647L));
    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_INT32, -2147483647L));

    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_INT64, 123456789L));
    TINY_TEST_CHECK(Format_CheckInteger(FORMAT_INT, 123456789L));

    TINY_TEST_CHECK(!Format_CheckInteger(FORMAT_BOOL, 1));
    TINY_TEST_CHECK(!Format_CheckInteger(FORMAT_STRING, 1));
    TINY_TEST_CHECK(!Format_CheckInteger(FORMAT_UNDEFINED, 1));
}

static void test_access(void)
{
    JsonArray *array = NULL;
    Access access = 0;

    TINY_TEST_CHECK(Access_FromJsonArray(NULL) == 0);
    TINY_TEST_CHECK(!Access_IsReadable(0));
    TINY_TEST_CHECK(!Access_IsWritable(0));
    TINY_TEST_CHECK(!Access_IsNotifiable(0));

    array = JsonArray_New();
    TINY_TEST_CHECK(array != NULL);
    if (array == NULL)
    {
        return;
    }

    TINY_TEST_CHECK(JsonArray_AddString(array, "read") == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_AddString(array, "write") == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_AddString(array, "notify") == TINY_RET_OK);
    access = Access_FromJsonArray(array);
    TINY_TEST_CHECK(access == ACCESS_RWN);
    TINY_TEST_CHECK(Access_IsReadable(access));
    TINY_TEST_CHECK(Access_IsWritable(access));
    TINY_TEST_CHECK(Access_IsNotifiable(access));
    JsonArray_Delete(array);

    array = JsonArray_New();
    TINY_TEST_CHECK(array != NULL);
    if (array == NULL)
    {
        return;
    }

    TINY_TEST_CHECK(JsonArray_AddString(array, "read") == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_AddString(array, "unknown") == TINY_RET_OK);
    access = Access_FromJsonArray(array);
    TINY_TEST_CHECK(access == ACCESS_READ);
    TINY_TEST_CHECK(Access_IsReadable(access));
    TINY_TEST_CHECK(!Access_IsWritable(access));
    TINY_TEST_CHECK(!Access_IsNotifiable(access));
    JsonArray_Delete(array);

    array = JsonArray_New();
    TINY_TEST_CHECK(array != NULL);
    if (array == NULL)
    {
        return;
    }

    TINY_TEST_CHECK(JsonArray_AddInteger(array, 1) == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_AddString(array, "write") == TINY_RET_OK);
    TINY_TEST_CHECK(Access_FromJsonArray(array) == 0);
    JsonArray_Delete(array);

    TINY_TEST_CHECK(Access_IsReadable(ACCESS_RW));
    TINY_TEST_CHECK(!Access_IsNotifiable(ACCESS_RW));
    TINY_TEST_CHECK(Access_IsNotifiable(ACCESS_WN));
    TINY_TEST_CHECK(!Access_IsReadable(ACCESS_WN));
}

static void test_urn_type(void)
{
    TINY_TEST_CHECK(UrnType_Retrieve("device") == DEVICE);
    TINY_TEST_CHECK(UrnType_Retrieve("service") == SERVICE);
    TINY_TEST_CHECK(UrnType_Retrieve("property") == PROPERTY);
    TINY_TEST_CHECK(UrnType_Retrieve("action") == ACTION);
    TINY_TEST_CHECK(UrnType_Retrieve("event") == EVENT);
    TINY_TEST_CHECK(UrnType_Retrieve("unknown") == UNDEFINED);

    TINY_TEST_CHECK(strcmp(UrnType_ToString(DEVICE), "device") == 0);
    TINY_TEST_CHECK(strcmp(UrnType_ToString(SERVICE), "service") == 0);
    TINY_TEST_CHECK(strcmp(UrnType_ToString(PROPERTY), "property") == 0);
    TINY_TEST_CHECK(strcmp(UrnType_ToString(ACTION), "action") == 0);
    TINY_TEST_CHECK(strcmp(UrnType_ToString(EVENT), "event") == 0);
    TINY_TEST_CHECK(strcmp(UrnType_ToString(UNDEFINED), "undefined") == 0);
}

static void test_value_list(void)
{
    ValueList *list = NULL;
    JsonValue *value = NULL;
    JsonArray *array = NULL;
    JsonObject *item = NULL;

    TINY_TEST_CHECK(ValueList_Put(NULL, NULL) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(!ValueList_CheckValue(NULL, NULL));

    /* integer values */
    list = ValueList_New();
    TINY_TEST_CHECK(list != NULL);
    if (list != NULL)
    {
        TINY_TEST_CHECK(TinyList_IsEmpty(&list->list));

        value = JsonValue_NewInteger(3);
        TINY_TEST_CHECK(value != NULL);
        TINY_TEST_CHECK(ValueList_Put(list, value) == TINY_RET_OK);
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(5);
        TINY_TEST_CHECK(value != NULL);
        TINY_TEST_CHECK(ValueList_Put(list, value) == TINY_RET_OK);
        JsonValue_Delete(value);

        TINY_TEST_CHECK(list->list.size == 2);
        TINY_TEST_CHECK(ValueList_Put(list, NULL) == TINY_RET_E_NULL_PTR);

        value = JsonValue_NewInteger(3);
        TINY_TEST_CHECK(ValueList_CheckValue(list, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(5);
        TINY_TEST_CHECK(ValueList_CheckValue(list, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(4);
        TINY_TEST_CHECK(!ValueList_CheckValue(list, value));
        JsonValue_Delete(value);

        value = JsonValue_NewBoolean(true);
        TINY_TEST_CHECK(!ValueList_CheckValue(list, value));
        JsonValue_Delete(value);

        TINY_TEST_CHECK(!ValueList_CheckValue(list, NULL));

        ValueList_Delete(list);
    }

    /* string values */
    list = ValueList_New();
    TINY_TEST_CHECK(list != NULL);
    if (list != NULL)
    {
        value = JsonValue_NewString("on");
        TINY_TEST_CHECK(value != NULL);
        TINY_TEST_CHECK(ValueList_Put(list, value) == TINY_RET_OK);
        JsonValue_Delete(value);

        value = JsonValue_NewString("off");
        TINY_TEST_CHECK(value != NULL);
        TINY_TEST_CHECK(ValueList_Put(list, value) == TINY_RET_OK);
        JsonValue_Delete(value);

        value = JsonValue_NewString("on");
        TINY_TEST_CHECK(ValueList_CheckValue(list, value));
        JsonValue_Delete(value);

        value = JsonValue_NewString("Off");
        TINY_TEST_CHECK(ValueList_CheckValue(list, value));
        JsonValue_Delete(value);

        value = JsonValue_NewString("auto");
        TINY_TEST_CHECK(!ValueList_CheckValue(list, value));
        JsonValue_Delete(value);

        ValueList_Delete(list);
    }

    /* a mixed list must not confuse numbers with strings */
    list = ValueList_New();
    TINY_TEST_CHECK(list != NULL);
    if (list != NULL)
    {
        value = JsonValue_NewInteger(3);
        TINY_TEST_CHECK(ValueList_Put(list, value) == TINY_RET_OK);
        JsonValue_Delete(value);

        value = JsonValue_NewString("on");
        TINY_TEST_CHECK(ValueList_Put(list, value) == TINY_RET_OK);
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(3);
        TINY_TEST_CHECK(ValueList_CheckValue(list, value));
        JsonValue_Delete(value);

        value = JsonValue_NewString("on");
        TINY_TEST_CHECK(ValueList_CheckValue(list, value));
        JsonValue_Delete(value);

        value = JsonValue_NewString("off");
        TINY_TEST_CHECK(!ValueList_CheckValue(list, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(4);
        TINY_TEST_CHECK(!ValueList_CheckValue(list, value));
        JsonValue_Delete(value);

        ValueList_Delete(list);
    }

    array = JsonArray_New();
    TINY_TEST_CHECK(array != NULL);
    if (array != NULL)
    {
        item = JsonObject_New();
        TINY_TEST_CHECK(item != NULL);
        if (item != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutInteger(item, "value", 7) == TINY_RET_OK);
            TINY_TEST_CHECK(JsonArray_AddObject(array, item) == TINY_RET_OK);
        }

        item = JsonObject_New();
        TINY_TEST_CHECK(item != NULL);
        if (item != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutInteger(item, "value", 9) == TINY_RET_OK);
            TINY_TEST_CHECK(JsonArray_AddObject(array, item) == TINY_RET_OK);
        }

        list = ValueList_NewFrom(array);
        TINY_TEST_CHECK(list != NULL);
        if (list != NULL)
        {
            TINY_TEST_CHECK(list->list.size == 2);

            value = JsonValue_NewInteger(7);
            TINY_TEST_CHECK(ValueList_CheckValue(list, value));
            JsonValue_Delete(value);

            value = JsonValue_NewInteger(9);
            TINY_TEST_CHECK(ValueList_CheckValue(list, value));
            JsonValue_Delete(value);

            value = JsonValue_NewInteger(8);
            TINY_TEST_CHECK(!ValueList_CheckValue(list, value));
            JsonValue_Delete(value);

            ValueList_Delete(list);
        }

        JsonArray_Delete(array);
    }

    array = JsonArray_New();
    TINY_TEST_CHECK(array != NULL);
    if (array != NULL)
    {
        TINY_TEST_CHECK(JsonArray_AddString(array, "not-an-object") == TINY_RET_OK);
        TINY_TEST_CHECK(ValueList_NewFrom(array) == NULL);
        JsonArray_Delete(array);
    }

    array = JsonArray_New();
    TINY_TEST_CHECK(array != NULL);
    if (array != NULL)
    {
        item = JsonObject_New();
        TINY_TEST_CHECK(item != NULL);
        if (item != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutInteger(item, "other", 1) == TINY_RET_OK);
            TINY_TEST_CHECK(JsonArray_AddObject(array, item) == TINY_RET_OK);
        }

        TINY_TEST_CHECK(ValueList_NewFrom(array) == NULL);
        JsonArray_Delete(array);
    }

    /* a NULL array yields an empty list */
    list = ValueList_NewFrom(NULL);
    TINY_TEST_CHECK(list != NULL);
    if (list != NULL)
    {
        TINY_TEST_CHECK(TinyList_IsEmpty(&list->list));
        ValueList_Delete(list);
    }
}

static void test_value_range(void)
{
    JsonValue *min = NULL;
    JsonValue *max = NULL;
    JsonValue *step = NULL;
    JsonValue *value = NULL;
    ValueRange *range = NULL;
    JsonArray *array = NULL;

    min = JsonValue_NewInteger(0);
    max = JsonValue_NewInteger(10);
    step = JsonValue_NewInteger(2);
    TINY_TEST_CHECK(min != NULL && max != NULL && step != NULL);

    range = ValueRange_New(min, max, step);
    TINY_TEST_CHECK(range != NULL);
    if (range != NULL)
    {
        TINY_TEST_CHECK(range->min != NULL);
        TINY_TEST_CHECK(range->max != NULL);
        TINY_TEST_CHECK(range->step != NULL);
        TINY_TEST_CHECK(range->min != min);

        value = JsonValue_NewInteger(0);
        TINY_TEST_CHECK(ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(4);
        TINY_TEST_CHECK(ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(10);
        TINY_TEST_CHECK(ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(3);
        TINY_TEST_CHECK(!ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(11);
        TINY_TEST_CHECK(!ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewFloat(4.0);
        TINY_TEST_CHECK(!ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewString("4");
        TINY_TEST_CHECK(!ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        TINY_TEST_CHECK(!ValueRange_CheckValue(range, NULL));

        ValueRange_Delete(range);
    }

    TINY_TEST_CHECK(ValueRange_New(NULL, max, step) == NULL);
    TINY_TEST_CHECK(ValueRange_New(min, NULL, step) == NULL);
    TINY_TEST_CHECK(ValueRange_New(min, max, NULL) == NULL);

    JsonValue_Delete(min);
    JsonValue_Delete(max);
    JsonValue_Delete(step);

    /* numeric range with mismatched value types is rejected */
    min = JsonValue_NewInteger(0);
    max = JsonValue_NewString("10");
    step = JsonValue_NewInteger(1);
    TINY_TEST_CHECK(ValueRange_New(min, max, step) == NULL);
    JsonValue_Delete(min);
    JsonValue_Delete(max);
    JsonValue_Delete(step);

    /* float range: only values reachable from min by step are accepted */
    min = JsonValue_NewFloat(0.0);
    max = JsonValue_NewFloat(1.0);
    step = JsonValue_NewFloat(0.25);
    range = ValueRange_New(min, max, step);
    TINY_TEST_CHECK(range != NULL);
    if (range != NULL)
    {
        value = JsonValue_NewFloat(0.5);
        TINY_TEST_CHECK(ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewFloat(0.75);
        TINY_TEST_CHECK(ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewFloat(0.3);
        TINY_TEST_CHECK(!ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(0);
        TINY_TEST_CHECK(ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        ValueRange_Delete(range);
    }
    JsonValue_Delete(min);
    JsonValue_Delete(max);
    JsonValue_Delete(step);

    /* hex range */
    min = JsonValue_NewString("01");
    max = JsonValue_NewString("05");
    step = JsonValue_NewString("01");
    range = ValueRange_New(min, max, step);
    TINY_TEST_CHECK(range != NULL);
    if (range != NULL)
    {
        value = JsonValue_NewString("03");
        TINY_TEST_CHECK(ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewString("05");
        TINY_TEST_CHECK(ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewString("07");
        TINY_TEST_CHECK(!ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(3);
        TINY_TEST_CHECK(!ValueRange_CheckValue(range, value));
        JsonValue_Delete(value);

        ValueRange_Delete(range);
    }
    JsonValue_Delete(min);
    JsonValue_Delete(max);
    JsonValue_Delete(step);

    /* NewFrom(array) */
    array = JsonArray_New();
    TINY_TEST_CHECK(array != NULL);
    if (array != NULL)
    {
        TINY_TEST_CHECK(JsonArray_AddInteger(array, 0) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonArray_AddInteger(array, 10) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonArray_AddInteger(array, 2) == TINY_RET_OK);

        range = ValueRange_NewFrom(array);
        TINY_TEST_CHECK(range != NULL);
        if (range != NULL)
        {
            value = JsonValue_NewInteger(6);
            TINY_TEST_CHECK(ValueRange_CheckValue(range, value));
            JsonValue_Delete(value);

            value = JsonValue_NewInteger(7);
            TINY_TEST_CHECK(!ValueRange_CheckValue(range, value));
            JsonValue_Delete(value);

            ValueRange_Delete(range);
        }

        TINY_TEST_CHECK(JsonArray_AddInteger(array, 1) == TINY_RET_OK);
        TINY_TEST_CHECK(ValueRange_NewFrom(array) == NULL);
        TINY_TEST_CHECK(JsonArray_GetSize(array) == 4U);

        JsonArray_Delete(array);
    }

    TINY_TEST_CHECK(ValueRange_NewFrom(NULL) == NULL);
}

static void test_property_value_types(void)
{
    Property *property = Property_NewInstance(2, "homekit-spec", "on", 0x30, NULL, FORMAT_BOOL, ACCESS_RWN, NONE);
    Property *plain = NULL;
    JsonValue *value = NULL;

    TINY_TEST_CHECK(property != NULL);
    if (property != NULL)
    {
        TINY_TEST_CHECK(property->iid == 2);
        TINY_TEST_CHECK(property->type.type == PROPERTY);
        TINY_TEST_CHECK(strcmp(property->type.ns, "homekit-spec") == 0);
        TINY_TEST_CHECK(strcmp(property->type.name, "on") == 0);
        TINY_TEST_CHECK(property->type.value == 0x30);
        TINY_TEST_CHECK(property->format == FORMAT_BOOL);
        TINY_TEST_CHECK(property->access == ACCESS_RWN);
        TINY_TEST_CHECK(property->unit == NONE);
        TINY_TEST_CHECK(property->value == NULL);
        TINY_TEST_CHECK(property->valueList == NULL);
        TINY_TEST_CHECK(property->valueRange == NULL);

        value = JsonValue_NewBoolean(true);
        TINY_TEST_CHECK(Property_CheckValue(property, value));
        JsonValue_Delete(value);

        value = JsonValue_NewBoolean(false);
        TINY_TEST_CHECK(Property_CheckValue(property, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(1);
        TINY_TEST_CHECK(Property_CheckValue(property, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(0);
        TINY_TEST_CHECK(Property_CheckValue(property, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(2);
        TINY_TEST_CHECK(!Property_CheckValue(property, value));
        JsonValue_Delete(value);

        value = JsonValue_NewFloat(1.0);
        TINY_TEST_CHECK(!Property_CheckValue(property, value));
        JsonValue_Delete(value);

        value = JsonValue_NewString("true");
        TINY_TEST_CHECK(!Property_CheckValue(property, value));
        JsonValue_Delete(value);

        TINY_TEST_CHECK(!Property_CheckValue(property, NULL));

        Property_Delete(property);
    }

    plain = Property_New(3);
    TINY_TEST_CHECK(plain != NULL);
    if (plain == NULL)
    {
        return;
    }

    plain->format = FORMAT_UINT8;
    value = JsonValue_NewInteger(255);
    TINY_TEST_CHECK(Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    value = JsonValue_NewInteger(256);
    TINY_TEST_CHECK(!Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    value = JsonValue_NewInteger(-1);
    TINY_TEST_CHECK(!Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    value = JsonValue_NewFloat(1.0);
    TINY_TEST_CHECK(!Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    plain->format = FORMAT_STRING;
    value = JsonValue_NewString("hello");
    TINY_TEST_CHECK(Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    value = JsonValue_NewInteger(1);
    TINY_TEST_CHECK(!Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    plain->format = FORMAT_FLOAT;
    value = JsonValue_NewInteger(1);
    TINY_TEST_CHECK(Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    value = JsonValue_NewFloat(1.5);
    TINY_TEST_CHECK(Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    value = JsonValue_NewString("1.5");
    TINY_TEST_CHECK(!Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    plain->format = FORMAT_HEX;
    value = JsonValue_NewString("ff");
    TINY_TEST_CHECK(Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    value = JsonValue_NewInteger(255);
    TINY_TEST_CHECK(!Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    plain->format = FORMAT_TLV8;
    value = JsonValue_NewString("0102");
    TINY_TEST_CHECK(Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    plain->format = FORMAT_INT32;
    value = JsonValue_NewInteger(100);
    TINY_TEST_CHECK(Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    value = JsonValue_NewFloat(100.0);
    TINY_TEST_CHECK(!Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    plain->format = FORMAT_INT64;
    value = JsonValue_NewInteger(100);
    TINY_TEST_CHECK(Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    plain->format = FORMAT_INT;
    value = JsonValue_NewInteger(100);
    TINY_TEST_CHECK(Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    plain->format = FORMAT_INT16;
    value = JsonValue_NewInteger(32768);
    TINY_TEST_CHECK(!Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    plain->format = FORMAT_UNDEFINED;
    value = JsonValue_NewInteger(0);
    TINY_TEST_CHECK(!Property_CheckValue(plain, value));
    JsonValue_Delete(value);

    /* value list constraint */
    plain->format = FORMAT_UINT8;
    plain->valueList = ValueList_New();
    TINY_TEST_CHECK(plain->valueList != NULL);
    if (plain->valueList != NULL)
    {
        value = JsonValue_NewInteger(1);
        TINY_TEST_CHECK(ValueList_Put(plain->valueList, value) == TINY_RET_OK);
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(2);
        TINY_TEST_CHECK(ValueList_Put(plain->valueList, value) == TINY_RET_OK);
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(2);
        TINY_TEST_CHECK(Property_CheckValue(plain, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(3);
        TINY_TEST_CHECK(!Property_CheckValue(plain, value));
        JsonValue_Delete(value);
    }

    /* value range constraint takes precedence over the value list */
    plain->format = FORMAT_FLOAT;
    plain->valueRange = ValueRange_New(JsonValue_NewInteger(0), JsonValue_NewInteger(10), JsonValue_NewInteger(2));
    TINY_TEST_CHECK(plain->valueRange != NULL);
    if (plain->valueRange != NULL)
    {
        value = JsonValue_NewInteger(4);
        TINY_TEST_CHECK(Property_CheckValue(plain, value));
        JsonValue_Delete(value);

        value = JsonValue_NewInteger(3);
        TINY_TEST_CHECK(!Property_CheckValue(plain, value));
        JsonValue_Delete(value);
    }

    Property_Delete(plain);
}

static void test_property_access(void)
{
    Property *read_write = Property_NewInstance(2, "homekit-spec", "on", 0x30, NULL, FORMAT_BOOL, ACCESS_RWN, NONE);
    Property *write_only = Property_NewInstance(3, "homekit-spec", "level", 0x31, NULL, FORMAT_UINT8, ACCESS_WRITE, NONE);
    Property *read_only = Property_NewInstance(4, "homekit-spec", "name", 0x32, NULL, FORMAT_STRING, ACCESS_READ, NONE);
    PropertyOperation *operation = NULL;
    JsonValue *value = NULL;

    TINY_TEST_CHECK(read_write != NULL);
    TINY_TEST_CHECK(write_only != NULL);
    TINY_TEST_CHECK(read_only != NULL);
    if (read_write == NULL || write_only == NULL || read_only == NULL)
    {
        Property_Delete(read_write);
        Property_Delete(write_only);
        Property_Delete(read_only);
        return;
    }

    operation = PropertyOperation_NewFrom("did-1", 1, 2);
    TINY_TEST_CHECK(operation != NULL);
    if (operation == NULL)
    {
        Property_Delete(read_write);
        Property_Delete(write_only);
        Property_Delete(read_only);
        return;
    }

    TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);

    /* read */
    Property_TryRead(read_write, operation);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);

    Property_TryRead(write_only, operation);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_PROPERTY_CANNOT_READ);

    /* write */
    operation->status = IOT_STATUS_COMPLETED;
    operation->value = JsonValue_NewBoolean(true);
    Property_TryWrite(read_write, operation);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);
    JsonValue_Delete(operation->value);
    operation->value = NULL;

    operation->status = IOT_STATUS_COMPLETED;
    operation->value = JsonValue_NewInteger(2);
    Property_TryWrite(read_write, operation);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_PROPERTY_VALUE_INVALID);

    operation->status = IOT_STATUS_COMPLETED;
    JsonValue_Delete(operation->value);
    operation->value = JsonValue_NewInteger(1);
    Property_TryWrite(read_only, operation);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_PROPERTY_CANNOT_WRITE);
    JsonValue_Delete(operation->value);
    operation->value = NULL;

    /* change */
    operation->status = IOT_STATUS_COMPLETED;
    operation->value = JsonValue_NewBoolean(false);
    Property_TryChange(read_write, operation, true);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);
    TINY_TEST_CHECK(read_write->value != NULL);
    TINY_TEST_CHECK(read_write->value != operation->value);
    if (read_write->value != NULL)
    {
        TINY_TEST_CHECK(read_write->value->type == JSON_BOOLEAN);
        TINY_TEST_CHECK(read_write->value->data.boolean->value == false);
    }

    operation->status = IOT_STATUS_COMPLETED;
    Property_TryChange(read_only, operation, true);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_PROPERTY_CANNOT_NOTIFY);

    operation->status = IOT_STATUS_COMPLETED;
    value = JsonValue_NewInteger(5);
    JsonValue_Delete(operation->value);
    operation->value = value;
    Property_TryChange(read_write, operation, true);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_PROPERTY_VALUE_INVALID);

    /* change without saving leaves the stored value untouched */
    operation->status = IOT_STATUS_COMPLETED;
    JsonValue_Delete(operation->value);
    operation->value = JsonValue_NewBoolean(true);
    Property_TryChange(read_write, operation, false);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);
    TINY_TEST_CHECK(read_write->value != NULL);
    if (read_write->value != NULL)
    {
        TINY_TEST_CHECK(read_write->value->data.boolean->value == false);
    }

    /* subscribe / unsubscribe */
    operation->status = IOT_STATUS_COMPLETED;
    Property_TrySubscribe(read_write, operation);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);

    Property_TryUnsubscribe(read_write, operation);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);

    operation->status = IOT_STATUS_COMPLETED;
    Property_TrySubscribe(read_only, operation);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_PROPERTY_CANNOT_NOTIFY);

    operation->status = IOT_STATUS_COMPLETED;
    Property_TryUnsubscribe(read_only, operation);
    TINY_TEST_CHECK(operation->status == IOT_STATUS_PROPERTY_CANNOT_NOTIFY);

    PropertyOperation_Delete(operation);
    Property_Delete(read_write);
    Property_Delete(write_only);
    Property_Delete(read_only);
}

static void test_argument_and_event(void)
{
    Argument *argument = NULL;
    Event *event = NULL;
    Event *named = NULL;
    Service *service = NULL;
    Property *property = NULL;
    EventOperation *operation = NULL;
    JsonArray *arguments = NULL;

    argument = Argument_New();
    TINY_TEST_CHECK(argument != NULL);
    if (argument != NULL)
    {
        TINY_TEST_CHECK(argument->iid == 0);
        Argument_Delete(argument);
    }

    argument = Argument_NewValue(7);
    TINY_TEST_CHECK(argument != NULL);
    if (argument != NULL)
    {
        TINY_TEST_CHECK(argument->iid == 7);
        Argument_Delete(argument);
    }

    Argument_Delete(NULL);

    event = Event_New(3);
    TINY_TEST_CHECK(event != NULL);
    if (event != NULL)
    {
        TINY_TEST_CHECK(event->iid == 3);
        TINY_TEST_CHECK(event->type.type == UNDEFINED);
        TINY_TEST_CHECK(TinyList_IsEmpty(&event->arguments));
        TINY_TEST_CHECK(event->service == NULL);
        Event_Delete(event);
    }

    named = Event_NewInstance(4, "homekit-spec", "stateless-programmable-switch-event", 0x71, NULL);
    TINY_TEST_CHECK(named != NULL);
    if (named != NULL)
    {
        TINY_TEST_CHECK(named->iid == 4);
        TINY_TEST_CHECK(named->type.type == EVENT);
        TINY_TEST_CHECK(strcmp(named->type.ns, "homekit-spec") == 0);
        TINY_TEST_CHECK(strcmp(named->type.name, "stateless-programmable-switch-event") == 0);
        TINY_TEST_CHECK(named->type.value == 0x71);
        Event_Delete(named);
    }

    /* produce with matching (empty) arguments */
    event = Event_New(5);
    operation = EventOperation_New();
    TINY_TEST_CHECK(event != NULL);
    TINY_TEST_CHECK(operation != NULL);
    if (event != NULL && operation != NULL)
    {
        arguments = JsonArray_New();
        TINY_TEST_CHECK(arguments != NULL);
        operation->arguments = arguments;

        TINY_TEST_CHECK(Event_ArgumentAdd(event, 2) == TINY_RET_OK);
        TINY_TEST_CHECK(event->arguments.size == 1);

        /* size mismatch */
        operation->status = IOT_STATUS_COMPLETED;
        Event_TryProduce(event, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_ACTION_IN_ERROR);

        /* no service attached */
        JsonArray_AddBoolean(operation->arguments, true);
        operation->status = IOT_STATUS_COMPLETED;
        Event_TryProduce(event, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_INTERNAL_ERROR);
    }
    if (operation != NULL)
    {
        EventOperation_Delete(operation);
    }
    if (event != NULL)
    {
        Event_Delete(event);
    }

    /* successful produce through a service */
    service = Service_New(1);
    property = Property_NewInstance(2, "homekit-spec", "on", 0x30, NULL, FORMAT_BOOL, ACCESS_RWN, NONE);
    event = Event_NewInstance(3, "homekit-spec", "event", 0x71, NULL);
    TINY_TEST_CHECK(service != NULL);
    TINY_TEST_CHECK(property != NULL);
    TINY_TEST_CHECK(event != NULL);

    if (service != NULL && property != NULL && event != NULL)
    {
        TINY_TEST_CHECK(TinyList_AddTail(&service->properties, property) != NULL);
        TINY_TEST_CHECK(TinyList_AddTail(&service->events, event) != NULL);

        event->service = service;
        TINY_TEST_CHECK(Event_ArgumentAdd(event, 2) == TINY_RET_OK);
        TINY_TEST_CHECK(event->arguments.size == 1);

        operation = EventOperation_New();
        TINY_TEST_CHECK(operation != NULL);
        if (operation != NULL)
        {
            operation->arguments = JsonArray_New();
            TINY_TEST_CHECK(operation->arguments != NULL);
            TINY_TEST_CHECK(JsonArray_AddBoolean(operation->arguments, true) == TINY_RET_OK);

            operation->status = IOT_STATUS_COMPLETED;
            Event_TryProduce(event, operation);
            TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);

            /* invalid argument value */
            JsonArray_Clean(operation->arguments);
            TINY_TEST_CHECK(JsonArray_AddInteger(operation->arguments, 5) == TINY_RET_OK);
            operation->status = IOT_STATUS_COMPLETED;
            Event_TryProduce(event, operation);
            TINY_TEST_CHECK(operation->status == IOT_STATUS_EVENT_ARGUMENTS_VALUE_ERROR);

            EventOperation_Delete(operation);
        }
    }

    Service_Delete(service);
}

static void test_device_and_service(void)
{
    Device *device = NULL;
    Service *service = NULL;
    Property *property = NULL;
    Action *action = NULL;
    Event *event = NULL;

    device = Device_New();
    TINY_TEST_CHECK(device != NULL);
    if (device == NULL)
    {
        return;
    }

    TINY_TEST_CHECK(device->index == 0);
    TINY_TEST_CHECK(TinyList_IsEmpty(&device->services));

    service = Service_NewInstance(1, "homekit-spec", "switch", 0x14, NULL);
    TINY_TEST_CHECK(service != NULL);
    if (service == NULL)
    {
        Device_Delete(device);
        return;
    }

    TINY_TEST_CHECK(service->iid == 1);
    TINY_TEST_CHECK(service->type.type == SERVICE);
    TINY_TEST_CHECK(strcmp(service->type.name, "switch") == 0);
    TINY_TEST_CHECK(TinyList_IsEmpty(&service->properties));
    TINY_TEST_CHECK(TinyList_IsEmpty(&service->actions));
    TINY_TEST_CHECK(TinyList_IsEmpty(&service->events));

    property = Property_New(2);
    action = Action_New(3);
    event = Event_New(4);
    TINY_TEST_CHECK(property != NULL);
    TINY_TEST_CHECK(action != NULL);
    TINY_TEST_CHECK(event != NULL);

    if (property != NULL && action != NULL && event != NULL)
    {
        TINY_TEST_CHECK(TinyList_AddTail(&service->properties, property) != NULL);
        TINY_TEST_CHECK(TinyList_AddTail(&service->actions, action) != NULL);
        TINY_TEST_CHECK(TinyList_AddTail(&service->events, event) != NULL);

        TINY_TEST_CHECK(service->properties.size == 1);
        TINY_TEST_CHECK(service->actions.size == 1);
        TINY_TEST_CHECK(service->events.size == 1);

        TINY_TEST_CHECK(Service_GetProperty(service, 2) == property);
        TINY_TEST_CHECK(Service_GetAction(service, 3) == action);
        TINY_TEST_CHECK(Service_GetEvent(service, 4) == event);

        TINY_TEST_CHECK(Service_GetProperty(service, 99) == NULL);
        TINY_TEST_CHECK(Service_GetAction(service, 99) == NULL);
        TINY_TEST_CHECK(Service_GetEvent(service, 99) == NULL);
    }

    TINY_TEST_CHECK(TinyList_AddTail(&device->services, service) != NULL);
    TINY_TEST_CHECK(device->services.size == 1);

    Device_Delete(device);
}

int main(void)
{
    TINY_TEST_BEGIN("typedef");

    test_format_strings();
    test_format_integer();
    test_access();
    test_urn_type();
    test_value_list();
    test_value_range();
    test_property_value_types();
    test_property_access();
    test_argument_and_event();
    test_device_and_service();

    return TINY_TEST_FINISH();
}
