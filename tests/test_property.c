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

#include <tinyspec/typedef/device/property.h>
#include <tinyspec/typedef/status/iot_status.h>
#include <tinycore/json_object.h>
#include <tinycore/json_value.h>

static void test_format_combination(void)
{
    TINY_TEST_CHECK(Format_FromString("combination") == FORMAT_COMBINATION);
    TINY_TEST_CHECK(STR_EQUAL(Format_ToString(FORMAT_COMBINATION), "combination"));
}

static void test_property_unit_string(void)
{
    Property *p = Property_NewInstance(1, "xiot-spec", "temperature", 0x02, NULL, FORMAT_FLOAT, ACCESS_READ, NULL);

    TINY_TEST_CHECK(p != NULL);
    if (p != NULL)
    {
        TINY_TEST_CHECK(p->unit == NULL);                     /* NONE → 无单位 */
        Property_SetUnit(p, "celsius");
        TINY_TEST_CHECK(p->unit != NULL);
        TINY_TEST_CHECK(STR_EQUAL(p->unit, "celsius"));
        Property_SetUnit(p, "my-custom-unit");
        TINY_TEST_CHECK(STR_EQUAL(p->unit, "my-custom-unit"));
        Property_SetUnit(p, NULL);
        TINY_TEST_CHECK(p->unit == NULL);

        Property_Delete(p);
    }

    /* 枚举参数 → 标准字符串（BC） */
    p = Property_NewInstance(2, "xiot-spec", "on", 0x06, NULL, FORMAT_UINT8, ACCESS_RWN, "celsius");
    TINY_TEST_CHECK(p != NULL);
    if (p != NULL)
    {
        TINY_TEST_CHECK(p->unit != NULL);
        TINY_TEST_CHECK(STR_EQUAL(p->unit, "celsius"));
        Property_Delete(p);
    }
}

static void test_property_max_length(void)
{
    Property *p = Property_NewInstance(1, "xiot-spec", "name", 0x01, NULL, FORMAT_STRING, ACCESS_READ, NULL);
    JsonValue *v = NULL;

    TINY_TEST_CHECK(p != NULL);
    if (p != NULL)
    {
        p->maxLength = 5;

        v = JsonValue_NewString("abcde");
        TINY_TEST_CHECK(Property_CheckValue(p, v));
        JsonValue_Delete(v);

        v = JsonValue_NewString("abcdef");
        TINY_TEST_CHECK(!Property_CheckValue(p, v));
        JsonValue_Delete(v);

        Property_Delete(p);
    }
}

static void test_property_combination(void)
{
    Property *hsv = NULL;
    Property *hue = NULL;
    Property *sat = NULL;
    JsonObject *object = NULL;
    JsonValue *value = NULL;

    hsv = Property_NewInstance(4, "xiot-spec", "hsv", 0x40, NULL, FORMAT_COMBINATION, ACCESS_RWN, NULL);
    hue = Property_NewInstance(1, "xiot-spec", "hue", 0x41, NULL, FORMAT_UINT8, ACCESS_RWN, NULL);
    sat = Property_NewInstance(2, "xiot-spec", "saturation", 0x42, NULL, FORMAT_BOOL, ACCESS_READ, NULL);

    TINY_TEST_CHECK(hsv != NULL && hue != NULL && sat != NULL);
    if (hsv != NULL && hue != NULL && sat != NULL)
    {
        hue->valueRange = ValueRange_New(JsonValue_NewInteger(0), JsonValue_NewInteger(100), JsonValue_NewInteger(1));

        TINY_TEST_CHECK(Property_MemberAdd(hsv, hue) == TINY_RET_OK);
        TINY_TEST_CHECK(Property_MemberAdd(hsv, sat) == TINY_RET_OK);
        TINY_TEST_CHECK(Property_GetMemberCount(hsv) == 2U);

        /* 完整合法对象 */
        object = JsonObject_New();
        JsonObject_PutInteger(object, "1", 50);
        JsonObject_PutBoolean(object, "2", true);
        value = JsonValue_NewObject(object);
        TINY_TEST_CHECK(Property_CheckValue(hsv, value));
        JsonValue_Delete(value);

        /* 缺成员 → 失败 */
        object = JsonObject_New();
        JsonObject_PutInteger(object, "1", 50);
        value = JsonValue_NewObject(object);
        TINY_TEST_CHECK(!Property_CheckValue(hsv, value));
        JsonValue_Delete(value);

        /* 成员值越界 → 失败（hue range [0,100]） */
        object = JsonObject_New();
        JsonObject_PutInteger(object, "1", 101);
        JsonObject_PutBoolean(object, "2", true);
        value = JsonValue_NewObject(object);
        TINY_TEST_CHECK(!Property_CheckValue(hsv, value));
        JsonValue_Delete(value);
    }

    if (hsv != NULL) Property_Delete(hsv);
    if (hue != NULL) Property_Delete(hue);
    if (sat != NULL) Property_Delete(sat);
}

int main(void)
{
    TINY_TEST_BEGIN("property");

    test_format_combination();
    test_property_unit_string();
    test_property_max_length();
    test_property_combination();

    return TINY_TEST_FINISH();
}