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
#include <tinycore/json_string.h>
#include <tinycore/json_value.h>
#include <tinyspec/json_util.h>

static void test_null_arguments(void)
{
    JsonArray array;

    TINY_TEST_CHECK(JsonArray_Construct(&array) == TINY_RET_OK);

    TINY_TEST_CHECK(JsonArray_CopyInto(NULL, &array) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(JsonArray_CopyInto(&array, NULL) == TINY_RET_E_NULL_PTR);

    JsonArray_Dispose(&array);
}

static void test_copy_empty(void)
{
    JsonArray dst;
    JsonArray src;

    TINY_TEST_CHECK(JsonArray_Construct(&dst) == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_Construct(&src) == TINY_RET_OK);

    TINY_TEST_CHECK(JsonArray_CopyInto(&dst, &src) == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_GetSize(&dst) == 0);
    TINY_TEST_CHECK(JsonArray_IsEmpty(&dst));

    JsonArray_Dispose(&dst);
    JsonArray_Dispose(&src);
}

static void test_copy_values(void)
{
    JsonArray dst;
    JsonArray src;

    TINY_TEST_CHECK(JsonArray_Construct(&dst) == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_Construct(&src) == TINY_RET_OK);

    TINY_TEST_CHECK(JsonArray_AddString(&src, "hello") == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_AddInteger(&src, 42) == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_AddBoolean(&src, true) == TINY_RET_OK);

    TINY_TEST_CHECK(JsonArray_CopyInto(&dst, &src) == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_GetSize(&dst) == 3);
    TINY_TEST_CHECK(JsonArray_Equals(&dst, &src));

    JsonString *string = JsonArray_GetString(&dst, 0);
    TINY_TEST_CHECK(string != NULL);
    if (string != NULL)
    {
        TINY_TEST_CHECK(strcmp(string->value, "hello") == 0);
    }

    JsonNumber *number = JsonArray_GetNumber(&dst, 1);
    TINY_TEST_CHECK(number != NULL);
    if (number != NULL)
    {
        TINY_TEST_CHECK(number->value.intValue == 42);
    }

    JsonBoolean *boolean = JsonArray_GetBoolean(&dst, 2);
    TINY_TEST_CHECK(boolean != NULL);
    if (boolean != NULL)
    {
        TINY_TEST_CHECK(boolean->value);
    }

    JsonArray_Dispose(&dst);
    JsonArray_Dispose(&src);
}

static void test_copy_replaces_existing(void)
{
    JsonArray dst;
    JsonArray src;

    TINY_TEST_CHECK(JsonArray_Construct(&dst) == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_Construct(&src) == TINY_RET_OK);

    TINY_TEST_CHECK(JsonArray_AddString(&dst, "old") == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_AddInteger(&src, 1) == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_AddInteger(&src, 2) == TINY_RET_OK);

    TINY_TEST_CHECK(JsonArray_CopyInto(&dst, &src) == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_GetSize(&dst) == 2);
    TINY_TEST_CHECK(JsonArray_Equals(&dst, &src));

    /* copying a source onto itself must not free the copied data */
    TINY_TEST_CHECK(JsonArray_CopyInto(&dst, &dst) == TINY_RET_OK);
    TINY_TEST_CHECK(JsonArray_GetSize(&dst) == 2);

    JsonArray_Dispose(&dst);
    JsonArray_Dispose(&src);
}

int main(void)
{
    TINY_TEST_BEGIN("json_util");

    test_null_arguments();
    test_copy_empty();
    test_copy_values();
    test_copy_replaces_existing();

    return TINY_TEST_FINISH();
}
