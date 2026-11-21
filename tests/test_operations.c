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

static void operations_add_integer(PropertyOperations *operations,
                                   const char *did,
                                   uint16_t siid,
                                   uint16_t iid,
                                   int32_t value)
{
    PropertyOperation *operation = PropertyOperation_NewFrom(did, siid, iid);

    if (operation == NULL)
    {
        return;
    }

    operation->value = JsonValue_NewInteger(value);
    if (operation->value == NULL || TinyList_AddTail(&operations->properties, operation) == NULL)
    {
        PropertyOperation_Delete(operation);
    }
}

static void test_property_operation(void)
{
    PropertyOperation operation;
    PropertyOperation *first = NULL;
    PropertyOperation *second = NULL;
    PropertyOperation *copy = NULL;
    JsonValue *value = NULL;
    char text[IOT_XID_LENGTH];

    TINY_TEST_CHECK(PropertyOperation_Construct(NULL) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(PropertyOperation_Construct(&operation) == TINY_RET_OK);
    TINY_TEST_CHECK(operation.pid.did[0] == '\0');
    TINY_TEST_CHECK(operation.pid.siid == 0);
    TINY_TEST_CHECK(operation.pid.iid == 0);
    TINY_TEST_CHECK(operation.value == NULL);
    TINY_TEST_CHECK(operation.status == 0);
    TINY_TEST_CHECK(operation.description[0] == '\0');
    PropertyOperation_Dispose(&operation);
    PropertyOperation_Delete(NULL);

    first = PropertyOperation_NewFrom("did-1", 2, 3);
    TINY_TEST_CHECK(first != NULL);
    if (first != NULL)
    {
        TINY_TEST_CHECK(strcmp(first->pid.did, "did-1") == 0);
        TINY_TEST_CHECK(first->pid.siid == 2);
        TINY_TEST_CHECK(first->pid.iid == 3);
        TINY_TEST_CHECK(first->value == NULL);

        PropertyId_ToString(text, &first->pid);
        TINY_TEST_CHECK(strcmp(text, "did-1.2.3") == 0);

        PropertyOperation_Delete(first);
    }

    TINY_TEST_CHECK(PropertyOperation_NewValue("did-1", 1, 1, NULL) == NULL);

    value = JsonValue_NewInteger(42);
    TINY_TEST_CHECK(value != NULL);
    first = PropertyOperation_NewValue("did-2", 1, 4, value);
    TINY_TEST_CHECK(first != NULL);
    if (first != NULL)
    {
        TINY_TEST_CHECK(first->value != NULL);
        TINY_TEST_CHECK(first->value != value);
        TINY_TEST_CHECK(JsonValue_Equals(first->value, value));

        copy = PropertyOperation_CopyFrom(first);
        TINY_TEST_CHECK(copy != NULL);
        if (copy != NULL)
        {
            TINY_TEST_CHECK(copy != first);
            TINY_TEST_CHECK(copy->value != NULL);
            TINY_TEST_CHECK(copy->value != first->value);
            TINY_TEST_CHECK(JsonValue_Equals(copy->value, first->value));
            TINY_TEST_CHECK(strcmp(copy->pid.did, "did-2") == 0);
            TINY_TEST_CHECK(copy->pid.siid == 1);
            TINY_TEST_CHECK(copy->pid.iid == 4);
            TINY_TEST_CHECK(copy->status == 0);
            PropertyOperation_Delete(copy);
        }

        /* copying over an operation that already holds a value replaces it */
        second = PropertyOperation_NewFrom("did-3", 5, 6);
        TINY_TEST_CHECK(second != NULL);
        if (second != NULL)
        {
            second->value = JsonValue_NewInteger(7);
            second->status = -425;
            strncpy(second->description, "old", IOT_DESCRIPTION_LENGTH);

            TINY_TEST_CHECK(PropertyOperation_Copy(second, first) == TINY_RET_OK);
            TINY_TEST_CHECK(strcmp(second->pid.did, "did-2") == 0);
            TINY_TEST_CHECK(second->pid.siid == 1);
            TINY_TEST_CHECK(second->pid.iid == 4);
            TINY_TEST_CHECK(second->value != NULL);
            TINY_TEST_CHECK(JsonValue_Equals(second->value, first->value));

            /* self copy is a no-op */
            TINY_TEST_CHECK(PropertyOperation_Copy(second, second) == TINY_RET_OK);
            TINY_TEST_CHECK(second->value != NULL);

            /* copying a value-less source clears the destination value */
            JsonValue_Delete(first->value);
            first->value = NULL;
            TINY_TEST_CHECK(PropertyOperation_Copy(second, first) == TINY_RET_OK);
            TINY_TEST_CHECK(second->value == NULL);

            PropertyOperation_Delete(second);
        }

        PropertyOperation_Delete(first);
    }
    JsonValue_Delete(value);
}

static void test_property_operations(void)
{
    PropertyOperations operations;
    PropertyOperations *src = NULL;
    PropertyOperations *dst = NULL;

    TINY_TEST_CHECK(PropertyOperations_Construct(NULL) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(PropertyOperations_Construct(&operations) == TINY_RET_OK);
    TINY_TEST_CHECK(TinyList_IsEmpty(&operations.properties));
    PropertyOperations_Dispose(&operations);
    PropertyOperations_Delete(NULL);

    src = PropertyOperations_New();
    dst = PropertyOperations_New();
    TINY_TEST_CHECK(src != NULL);
    TINY_TEST_CHECK(dst != NULL);
    if (src == NULL || dst == NULL)
    {
        PropertyOperations_Delete(src);
        PropertyOperations_Delete(dst);
        return;
    }

    TINY_TEST_CHECK(TinyList_IsEmpty(&src->properties));

    operations_add_integer(src, "did-1", 2, 3, 42);
    operations_add_integer(src, "did-1", 2, 4, 7);
    TINY_TEST_CHECK(src->properties.size == 2);

    TINY_TEST_CHECK(PropertyOperations_Copy(dst, src) == TINY_RET_OK);
    TINY_TEST_CHECK(dst->properties.size == 2);
    if (dst->properties.size == 2)
    {
        PropertyOperation *first = (PropertyOperation *) TinyList_GetAt(&dst->properties, 0);
        PropertyOperation *second = (PropertyOperation *) TinyList_GetAt(&dst->properties, 1);

        TINY_TEST_CHECK(first->pid.iid == 3);
        TINY_TEST_CHECK(first->value != NULL);
        TINY_TEST_CHECK(second->pid.iid == 4);
        TINY_TEST_CHECK(second->value != NULL);

        if (first->value != NULL && second->value != NULL)
        {
            TINY_TEST_CHECK(first->value->data.number->value.intValue == 42);
            TINY_TEST_CHECK(second->value->data.number->value.intValue == 7);
        }
    }

    /* self copy leaves the list untouched */
    TINY_TEST_CHECK(PropertyOperations_Copy(dst, dst) == TINY_RET_OK);
    TINY_TEST_CHECK(dst->properties.size == 2);

    TINY_TEST_CHECK(PropertyOperations_Copy(NULL, src) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(PropertyOperations_Copy(dst, NULL) == TINY_RET_E_NULL_PTR);

    PropertyOperations_Delete(dst);
    PropertyOperations_Delete(src);
}

static void test_action_operation(void)
{
    ActionOperation operation;
    ActionOperation *src = NULL;
    ActionOperation *dst = NULL;

    TINY_TEST_CHECK(ActionOperation_Construct(NULL) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(ActionOperation_Construct(&operation) == TINY_RET_OK);
    TINY_TEST_CHECK(operation.aid.did[0] == '\0');
    TINY_TEST_CHECK(operation.status == 0);
    TINY_TEST_CHECK(TinyList_IsEmpty(&operation.in));
    TINY_TEST_CHECK(TinyList_IsEmpty(&operation.out));
    ActionOperation_Dispose(&operation);
    ActionOperation_Delete(NULL);

    src = ActionOperation_New();
    dst = ActionOperation_New();
    TINY_TEST_CHECK(src != NULL);
    TINY_TEST_CHECK(dst != NULL);
    if (src == NULL || dst == NULL)
    {
        ActionOperation_Delete(src);
        ActionOperation_Delete(dst);
        return;
    }

    strncpy(src->aid.did, "did-1", IOT_DEVICE_ID_LENGTH);
    src->aid.siid = 2;
    src->aid.iid = 3;
    src->status = IOT_STATUS_ACTION_IN_ERROR;

    {
        ArgumentOperation *argument = ActionOperation_EnsureIn(src, 1);

        TINY_TEST_CHECK(argument != NULL);
        if (argument != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddInteger(&argument->values, 1) == TINY_RET_OK);
        }
    }

    {
        ArgumentOperation *argument = ActionOperation_EnsureIn(src, 2);

        TINY_TEST_CHECK(argument != NULL);
        if (argument != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddInteger(&argument->values, 2) == TINY_RET_OK);
        }
    }

    {
        ArgumentOperation *argument = ActionOperation_EnsureOut(src, 3);

        TINY_TEST_CHECK(argument != NULL);
        if (argument != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddBoolean(&argument->values, true) == TINY_RET_OK);
        }
    }

    src->argumentsCompact = true;

    TINY_TEST_CHECK(ActionOperation_Copy(src, src) == TINY_RET_E_PARAM_INVALID);
    TINY_TEST_CHECK(ActionOperation_Copy(NULL, src) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(ActionOperation_Copy(dst, NULL) == TINY_RET_E_NULL_PTR);

    TINY_TEST_CHECK(ActionOperation_Copy(dst, src) == TINY_RET_OK);
    TINY_TEST_CHECK(strcmp(dst->aid.did, "did-1") == 0);
    TINY_TEST_CHECK(dst->aid.siid == 2);
    TINY_TEST_CHECK(dst->aid.iid == 3);
    TINY_TEST_CHECK(dst->status == IOT_STATUS_ACTION_IN_ERROR);
    TINY_TEST_CHECK(dst->argumentsCompact == true);
    TINY_TEST_CHECK(dst->in.size == 2);
    TINY_TEST_CHECK(dst->out.size == 1);
    TINY_TEST_CHECK(ActionOperation_GetIn(dst, 1) != NULL);
    TINY_TEST_CHECK(ActionOperation_GetOut(dst, 3) != NULL);

    /* copying again replaces the existing argument lists */
    TINY_TEST_CHECK(ActionOperation_Copy(dst, src) == TINY_RET_OK);
    TINY_TEST_CHECK(dst->in.size == 2);
    TINY_TEST_CHECK(dst->out.size == 1);

    ActionOperation_Delete(dst);
    ActionOperation_Delete(src);
}

static void test_event_operation(void)
{
    EventOperation operation;
    EventOperation *src = NULL;
    EventOperation *dst = NULL;
    EventOperation *copy = NULL;
    EventOperation *empty = NULL;

    TINY_TEST_CHECK(EventOperation_Construct(NULL) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(EventOperation_Construct(&operation) == TINY_RET_OK);
    TINY_TEST_CHECK(operation.eid.did[0] == '\0');
    TINY_TEST_CHECK(operation.status == 0);
    TINY_TEST_CHECK(TinyList_IsEmpty(&operation.arguments));
    EventOperation_Dispose(&operation);
    EventOperation_Delete(NULL);

    src = EventOperation_New();
    dst = EventOperation_New();
    empty = EventOperation_New();
    TINY_TEST_CHECK(src != NULL);
    TINY_TEST_CHECK(dst != NULL);
    TINY_TEST_CHECK(empty != NULL);
    if (src == NULL || dst == NULL || empty == NULL)
    {
        EventOperation_Delete(src);
        EventOperation_Delete(dst);
        EventOperation_Delete(empty);
        return;
    }

    strncpy(src->eid.did, "did-1", IOT_DEVICE_ID_LENGTH);
    src->eid.siid = 2;
    src->eid.iid = 5;
    {
        ArgumentOperation *argument = EventOperation_EnsureArgument(src, 1);

        TINY_TEST_CHECK(argument != NULL);
        if (argument != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddInteger(&argument->values, 7) == TINY_RET_OK);
        }
    }

    {
        ArgumentOperation *argument = EventOperation_EnsureArgument(src, 2);

        TINY_TEST_CHECK(argument != NULL);
        if (argument != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddString(&argument->values, "on") == TINY_RET_OK);
        }
    }

    src->argumentsCompact = true;

    copy = EventOperation_CopyFrom(src);
    TINY_TEST_CHECK(copy != NULL);
    if (copy != NULL)
    {
        TINY_TEST_CHECK(strcmp(copy->eid.did, "did-1") == 0);
        TINY_TEST_CHECK(copy->eid.siid == 2);
        TINY_TEST_CHECK(copy->eid.iid == 5);
        TINY_TEST_CHECK(copy->argumentsCompact == true);
        TINY_TEST_CHECK(copy->arguments.size == 2);
        TINY_TEST_CHECK(EventOperation_GetArgument(copy, 1) != NULL);
        TINY_TEST_CHECK(EventOperation_GetArgument(copy, 2) != NULL);
        EventOperation_Delete(copy);
    }

    copy = EventOperation_CopyFrom(empty);
    TINY_TEST_CHECK(copy != NULL);
    if (copy != NULL)
    {
        TINY_TEST_CHECK(TinyList_IsEmpty(&copy->arguments));
        EventOperation_Delete(copy);
    }

    TINY_TEST_CHECK(EventOperation_Copy(src, src) == TINY_RET_E_PARAM_INVALID);
    TINY_TEST_CHECK(EventOperation_Copy(NULL, src) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(EventOperation_Copy(dst, NULL) == TINY_RET_E_NULL_PTR);

    TINY_TEST_CHECK(EventOperation_Copy(dst, src) == TINY_RET_OK);
    TINY_TEST_CHECK(strcmp(dst->eid.did, "did-1") == 0);
    TINY_TEST_CHECK(dst->eid.siid == 2);
    TINY_TEST_CHECK(dst->eid.iid == 5);
    TINY_TEST_CHECK(dst->arguments.size == 2);

    EventOperation_Delete(dst);
    EventOperation_Delete(empty);
    EventOperation_Delete(src);
}

static JsonArray *new_number_array(int32_t first, int32_t second)
{
    JsonArray *array = JsonArray_New();

    if (array != NULL)
    {
        JsonArray_AddInteger(array, first);
        JsonArray_AddInteger(array, second);
    }

    return array;
}

static void test_set_codec(void)
{
    PropertyOperations *operations = PropertyOperations_New();
    PropertyOperations decoded;
    JsonObject *content = NULL;
    JsonArray *array = NULL;

    TINY_TEST_CHECK(operations != NULL);
    if (operations == NULL)
    {
        return;
    }

    operations_add_integer(operations, "did-1", 2, 3, 42);
    operations_add_integer(operations, "did-1", 2, 4, 7);

    content = JsonObject_New();
    TINY_TEST_CHECK(content != NULL);
    if (content == NULL)
    {
        PropertyOperations_Delete(operations);
        return;
    }

    TINY_TEST_CHECK(!PropertyOperationsCodec_Set_Query_Encode(NULL, content));
    TINY_TEST_CHECK(!PropertyOperationsCodec_Set_Query_Encode(operations, NULL));
    TINY_TEST_CHECK(PropertyOperationsCodec_Set_Query_Decode(NULL, &decoded) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(PropertyOperationsCodec_Set_Query_Decode(content, NULL) == TINY_RET_E_NULL_PTR);

    TINY_TEST_CHECK(PropertyOperationsCodec_Set_Query_Encode(operations, content));
    array = JsonObject_GetArray(content, "properties");
    TINY_TEST_CHECK(array != NULL);
    if (array != NULL)
    {
        JsonValue *item = (JsonValue *) TinyList_GetAt(&array->values, 0);
        JsonObject *object = item != NULL && item->type == JSON_OBJECT ? item->data.object : NULL;

        TINY_TEST_CHECK(array->values.size == 2);
        TINY_TEST_CHECK(object != NULL);
        if (object != NULL)
        {
            JsonString *pid = JsonObject_GetString(object, "pid");
            JsonNumber *value = JsonObject_GetNumber(object, "value");

            TINY_TEST_CHECK(pid != NULL);
            if (pid != NULL)
            {
                TINY_TEST_CHECK(strcmp(pid->value, "did-1.2.3") == 0);
            }

            TINY_TEST_CHECK(value != NULL);
            if (value != NULL)
            {
                TINY_TEST_CHECK(value->type == JSON_NUMBER_INTEGER);
                TINY_TEST_CHECK(value->value.intValue == 42);
            }
        }
    }

    TINY_TEST_CHECK(PropertyOperationsCodec_Set_Query_Decode(content, &decoded) == TINY_RET_OK);
    TINY_TEST_CHECK(decoded.properties.size == 2);
    if (decoded.properties.size == 2)
    {
        PropertyOperation *first = (PropertyOperation *) TinyList_GetAt(&decoded.properties, 0);
        PropertyOperation *second = (PropertyOperation *) TinyList_GetAt(&decoded.properties, 1);

        TINY_TEST_CHECK(strcmp(first->pid.did, "did-1") == 0);
        TINY_TEST_CHECK(first->pid.siid == 2);
        TINY_TEST_CHECK(first->pid.iid == 3);
        TINY_TEST_CHECK(first->value != NULL);
        TINY_TEST_CHECK(second->pid.iid == 4);
        TINY_TEST_CHECK(second->value != NULL);
    }
    PropertyOperations_Dispose(&decoded);
    JsonObject_Delete(content);

    /* content without "properties" */
    content = JsonObject_New();
    TINY_TEST_CHECK(content != NULL);
    if (content != NULL)
    {
        TINY_TEST_CHECK(PropertyOperationsCodec_Set_Query_Decode(content, &decoded) == TINY_RET_E_PARAM_INVALID);
        JsonObject_Delete(content);
    }

    /* items that are not objects */
    content = JsonObject_New();
    if (content != NULL)
    {
        array = JsonArray_New();
        TINY_TEST_CHECK(array != NULL);
        if (array != NULL)
        {
            JsonArray_AddInteger(array, 1);
            TINY_TEST_CHECK(JsonObject_PutArray(content, "properties", array) == TINY_RET_OK);
        }
        TINY_TEST_CHECK(PropertyOperationsCodec_Set_Query_Decode(content, &decoded) == TINY_RET_E_PARAM_INVALID);
        PropertyOperations_Dispose(&decoded);
        JsonObject_Delete(content);
    }

    /* item without value */
    content = JsonObject_New();
    if (content != NULL)
    {
        JsonObject *item = JsonObject_New();
        array = JsonArray_New();
        TINY_TEST_CHECK(item != NULL);
        TINY_TEST_CHECK(array != NULL);
        if (item != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutString(item, "pid", "did-1.2.3") == TINY_RET_OK);
        }
        if (array != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddObject(array, item) == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutArray(content, "properties", array) == TINY_RET_OK);
        }
        TINY_TEST_CHECK(PropertyOperationsCodec_Set_Query_Decode(content, &decoded) == TINY_RET_E_PARAM_INVALID);
        PropertyOperations_Dispose(&decoded);
        JsonObject_Delete(content);
    }

    /* invalid pid is reported on the operation, not in the return code */
    content = JsonObject_New();
    if (content != NULL)
    {
        JsonObject *item = JsonObject_New();
        array = JsonArray_New();
        TINY_TEST_CHECK(item != NULL);
        TINY_TEST_CHECK(array != NULL);
        if (item != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutString(item, "pid", "no-separators") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutInteger(item, "value", 1) == TINY_RET_OK);
        }
        if (array != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddObject(array, item) == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutArray(content, "properties", array) == TINY_RET_OK);
        }
        TINY_TEST_CHECK(RET_FAILED(PropertyOperationsCodec_Set_Query_Decode(content, &decoded)));
        TINY_TEST_CHECK(decoded.properties.size == 1);
        if (decoded.properties.size == 1)
        {
            PropertyOperation *operation = (PropertyOperation *) TinyList_GetAt(&decoded.properties, 0);
            TINY_TEST_CHECK(operation->status == -100);
            TINY_TEST_CHECK(operation->value == NULL);
        }
        PropertyOperations_Dispose(&decoded);
        JsonObject_Delete(content);
    }

    /* results: status 0 has no description, failures do */
    {
        PropertyOperations *results = PropertyOperations_New();
        PropertyOperation *failed = PropertyOperation_NewFrom("did-1", 2, 4);

        TINY_TEST_CHECK(results != NULL);
        TINY_TEST_CHECK(failed != NULL);
        if (results != NULL && failed != NULL)
        {
            PropertyOperation *done = (PropertyOperation *) TinyList_GetAt(&operations->properties, 0);

            failed->status = IOT_STATUS_PROPERTY_VALUE_INVALID;
            strncpy(failed->description, "invalid value", IOT_DESCRIPTION_LENGTH);

            TINY_TEST_CHECK(TinyList_AddTail(&results->properties, done) != NULL);
            TINY_TEST_CHECK(TinyList_AddTail(&results->properties, failed) != NULL);
            /* the operations belong to the results list now */
            TinyList_Remove(&operations->properties, done);

            content = JsonObject_New();
            TINY_TEST_CHECK(content != NULL);
            if (content != NULL)
            {
                TINY_TEST_CHECK(PropertyOperationsCodec_Set_Result_Encode(results, content));
                array = JsonObject_GetArray(content, "properties");
                TINY_TEST_CHECK(array != NULL);
                if (array != NULL)
                {
                    JsonValue *item = (JsonValue *) TinyList_GetAt(&array->values, 1);
                    JsonObject *object = item != NULL && item->type == JSON_OBJECT ? item->data.object : NULL;

                    TINY_TEST_CHECK(array->values.size == 2);
                    TINY_TEST_CHECK(object != NULL);
                    if (object != NULL)
                    {
                        TINY_TEST_CHECK(!JsonObject_ContainsKey(object, "description") ||
                                        JsonObject_GetString(object, "description") != NULL);
                        TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(object, "description"), "invalid value") == 0);
                    }
                }

                TINY_TEST_CHECK(PropertyOperationsCodec_Set_Result_Decode(content, &decoded) == TINY_RET_OK);
                TINY_TEST_CHECK(decoded.properties.size == 2);
                if (decoded.properties.size == 2)
                {
                    PropertyOperation *first = (PropertyOperation *) TinyList_GetAt(&decoded.properties, 0);
                    PropertyOperation *second = (PropertyOperation *) TinyList_GetAt(&decoded.properties, 1);

                    TINY_TEST_CHECK(first->status == 0);
                    TINY_TEST_CHECK(second->status == IOT_STATUS_PROPERTY_VALUE_INVALID);
                    TINY_TEST_CHECK(strcmp(second->description, "invalid value") == 0);
                }
                PropertyOperations_Dispose(&decoded);
                JsonObject_Delete(content);
            }

            PropertyOperations_Delete(results);
        }
        else
        {
            PropertyOperation_Delete(failed);
            PropertyOperations_Delete(results);
        }
    }

    PropertyOperations_Delete(operations);
}

static void test_get_codec(void)
{
    PropertyOperations *operations = PropertyOperations_New();
    PropertyOperations decoded;
    JsonObject *content = NULL;
    JsonArray *array = NULL;

    TINY_TEST_CHECK(operations != NULL);
    if (operations == NULL)
    {
        return;
    }

    operations_add_integer(operations, "did-1", 2, 3, 42);
    operations_add_integer(operations, "did-1", 2, 4, 7);

    /* query: an array of pid strings */
    content = JsonObject_New();
    TINY_TEST_CHECK(content != NULL);
    if (content != NULL)
    {
        TINY_TEST_CHECK(PropertyOperationsCodec_Get_Query_Encode(operations, content));
        array = JsonObject_GetArray(content, "properties");
        TINY_TEST_CHECK(array != NULL);
        if (array != NULL)
        {
            JsonValue *item = (JsonValue *) TinyList_GetAt(&array->values, 1);

            TINY_TEST_CHECK(array->values.size == 2);
            TINY_TEST_CHECK(item != NULL && item->type == JSON_STRING);
            if (item != NULL && item->type == JSON_STRING)
            {
                TINY_TEST_CHECK(strcmp(item->data.string->value, "did-1.2.4") == 0);
            }
        }

        TINY_TEST_CHECK(PropertyOperationsCodec_Get_Query_Decode(content, &decoded) == TINY_RET_OK);
        TINY_TEST_CHECK(decoded.properties.size == 2);
        if (decoded.properties.size == 2)
        {
            PropertyOperation *first = (PropertyOperation *) TinyList_GetAt(&decoded.properties, 0);

            TINY_TEST_CHECK(strcmp(first->pid.did, "did-1") == 0);
            TINY_TEST_CHECK(first->pid.iid == 3);
            TINY_TEST_CHECK(first->value == NULL);
        }
        PropertyOperations_Dispose(&decoded);
        JsonObject_Delete(content);
    }

    /* query with an invalid pid string */
    content = JsonObject_New();
    if (content != NULL)
    {
        array = JsonArray_New();
        TINY_TEST_CHECK(array != NULL);
        if (array != NULL)
        {
            JsonArray_AddString(array, "no-separators");
            TINY_TEST_CHECK(JsonObject_PutArray(content, "properties", array) == TINY_RET_OK);
        }
        TINY_TEST_CHECK(RET_FAILED(PropertyOperationsCodec_Get_Query_Decode(content, &decoded)));
        TINY_TEST_CHECK(decoded.properties.size == 1);
        if (decoded.properties.size == 1)
        {
            PropertyOperation *operation = (PropertyOperation *) TinyList_GetAt(&decoded.properties, 0);
            TINY_TEST_CHECK(operation->status == IOT_STATUS_PID_INVALID);
        }
        PropertyOperations_Dispose(&decoded);
        JsonObject_Delete(content);
    }

    /* query with a non string item */
    content = JsonObject_New();
    if (content != NULL)
    {
        array = JsonArray_New();
        TINY_TEST_CHECK(array != NULL);
        if (array != NULL)
        {
            JsonArray_AddInteger(array, 1);
            TINY_TEST_CHECK(JsonObject_PutArray(content, "properties", array) == TINY_RET_OK);
        }
        TINY_TEST_CHECK(PropertyOperationsCodec_Get_Query_Decode(content, &decoded) == TINY_RET_E_PARAM_INVALID);
        PropertyOperations_Dispose(&decoded);
        JsonObject_Delete(content);
    }

    /* result: values when the status is 0, status + description otherwise */
    {
        PropertyOperations *results = PropertyOperations_New();
        PropertyOperation *missing = PropertyOperation_NewFrom("did-1", 2, 4);
        PropertyOperation *without_value = PropertyOperation_NewFrom("did-1", 2, 5);

        TINY_TEST_CHECK(results != NULL);
        TINY_TEST_CHECK(missing != NULL);
        TINY_TEST_CHECK(without_value != NULL);
        if (results != NULL && missing != NULL && without_value != NULL)
        {
            PropertyOperation *with_value = (PropertyOperation *) TinyList_GetAt(&operations->properties, 0);

            missing->status = IOT_STATUS_PROPERTY_NOT_FOUND;
            strncpy(missing->description, "property not found", IOT_DESCRIPTION_LENGTH);

            TINY_TEST_CHECK(TinyList_AddTail(&results->properties, with_value) != NULL);
            TINY_TEST_CHECK(TinyList_AddTail(&results->properties, without_value) != NULL);
            TINY_TEST_CHECK(TinyList_AddTail(&results->properties, missing) != NULL);
            TinyList_Remove(&operations->properties, with_value);

            content = JsonObject_New();
            TINY_TEST_CHECK(content != NULL);
            if (content != NULL)
            {
                TINY_TEST_CHECK(PropertyOperationsCodec_Get_Result_Encode(results, content));
                array = JsonObject_GetArray(content, "properties");
                TINY_TEST_CHECK(array != NULL);
                if (array != NULL)
                {
                    /* the value-less property is skipped */
                    TINY_TEST_CHECK(array->values.size == 2);

                    if (array->values.size == 2)
                    {
                        JsonValue *item = (JsonValue *) TinyList_GetAt(&array->values, 0);
                        JsonObject *object = item != NULL && item->type == JSON_OBJECT ? item->data.object : NULL;

                        TINY_TEST_CHECK(object != NULL);
                        if (object != NULL)
                        {
                            JsonNumber *value = JsonObject_GetNumber(object, "value");
                            TINY_TEST_CHECK(value != NULL);
                            if (value != NULL)
                            {
                                TINY_TEST_CHECK(value->value.intValue == 42);
                            }
                            TINY_TEST_CHECK(!JsonObject_ContainsKey(object, "status"));
                        }

                        item = (JsonValue *) TinyList_GetAt(&array->values, 1);
                        object = item != NULL && item->type == JSON_OBJECT ? item->data.object : NULL;
                        TINY_TEST_CHECK(object != NULL);
                        if (object != NULL)
                        {
                            JsonNumber *status = JsonObject_GetNumber(object, "status");
                            TINY_TEST_CHECK(status != NULL);
                            if (status != NULL)
                            {
                                TINY_TEST_CHECK(status->value.intValue == IOT_STATUS_PROPERTY_NOT_FOUND);
                            }
                            TINY_TEST_CHECK(!JsonObject_ContainsKey(object, "value"));
                            TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(object, "description"), "property not found") == 0);
                        }
                    }
                }

                TINY_TEST_CHECK(PropertyOperationsCodec_Get_Result_Decode(content, &decoded) == TINY_RET_OK);
                TINY_TEST_CHECK(decoded.properties.size == 2);
                if (decoded.properties.size == 2)
                {
                    PropertyOperation *first = (PropertyOperation *) TinyList_GetAt(&decoded.properties, 0);
                    PropertyOperation *second = (PropertyOperation *) TinyList_GetAt(&decoded.properties, 1);

                    TINY_TEST_CHECK(first->status == 0);
                    TINY_TEST_CHECK(first->value != NULL);
                    TINY_TEST_CHECK(second->status == IOT_STATUS_PROPERTY_NOT_FOUND);
                    TINY_TEST_CHECK(second->value == NULL);
                    TINY_TEST_CHECK(strcmp(second->description, "property not found") == 0);
                }
                PropertyOperations_Dispose(&decoded);
                JsonObject_Delete(content);
            }

            PropertyOperations_Delete(results);
        }
        else
        {
            PropertyOperation_Delete(missing);
            PropertyOperation_Delete(without_value);
            PropertyOperations_Delete(results);
        }
    }

    /* result with status 0 but without a value */
    content = JsonObject_New();
    if (content != NULL)
    {
        JsonObject *item = JsonObject_New();
        array = JsonArray_New();
        TINY_TEST_CHECK(item != NULL);
        TINY_TEST_CHECK(array != NULL);
        if (item != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutString(item, "pid", "did-1.2.3") == TINY_RET_OK);
        }
        if (array != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddObject(array, item) == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutArray(content, "properties", array) == TINY_RET_OK);
        }
        TINY_TEST_CHECK(PropertyOperationsCodec_Get_Result_Decode(content, &decoded) == TINY_RET_E_PARAM_INVALID);
        PropertyOperations_Dispose(&decoded);
        JsonObject_Delete(content);
    }

    PropertyOperations_Delete(operations);
}

static void test_action_codec(void)
{
    ActionOperation *action = ActionOperation_New();
    ActionOperation *decoded = ActionOperation_New();
    JsonObject *content = NULL;
    JsonArray *array = NULL;

    TINY_TEST_CHECK(action != NULL);
    TINY_TEST_CHECK(decoded != NULL);
    if (action == NULL || decoded == NULL)
    {
        ActionOperation_Delete(action);
        ActionOperation_Delete(decoded);
        return;
    }

    strncpy(action->aid.did, "did-9", IOT_DEVICE_ID_LENGTH);
    action->aid.siid = 3;
    action->aid.iid = 1;

    /* query encoding: aid only when there is no input */
    content = JsonObject_New();
    TINY_TEST_CHECK(content != NULL);
    if (content != NULL)
    {
        TINY_TEST_CHECK(ActionOperationCodec_Invoke_Query_Encode(action, content));
        TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(content, "aid"), "did-9.3.1") == 0);
        TINY_TEST_CHECK(!JsonObject_ContainsKey(content, "in"));
        TINY_TEST_CHECK(!ActionOperationCodec_Invoke_Query_Encode(NULL, content));
        TINY_TEST_CHECK(!ActionOperationCodec_Invoke_Query_Encode(action, NULL));
        JsonObject_Delete(content);
    }

    /* query encoding with one input argument */
    {
        ArgumentOperation *in = ActionOperation_EnsureIn(action, 2);

        TINY_TEST_CHECK(in != NULL);
        if (in != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddInteger(&in->values, 7) == TINY_RET_OK);
        }
    }

    content = JsonObject_New();
    if (content != NULL)
    {
        TINY_TEST_CHECK(ActionOperationCodec_Invoke_Query_Encode(action, content));
        array = JsonObject_GetArray(content, "in");
        TINY_TEST_CHECK(array != NULL);
        if (array != NULL)
        {
            TINY_TEST_CHECK(array->values.size == 1);
        }
        JsonObject_Delete(content);
    }

    /* query decoding reads aid and the optional in array */
    content = JsonObject_New();
    array = new_number_array(5, 6);
    TINY_TEST_CHECK(content != NULL);
    TINY_TEST_CHECK(array != NULL);
    if (content != NULL && array != NULL)
    {
        TINY_TEST_CHECK(JsonObject_PutString(content, "aid", "did-9.3.1") == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutArray(content, "in", array) == TINY_RET_OK);

        TINY_TEST_CHECK(ActionOperationCodec_Invoke_Query_Decode(content, decoded) == TINY_RET_OK);
        TINY_TEST_CHECK(strcmp(decoded->aid.did, "did-9") == 0);
        TINY_TEST_CHECK(decoded->aid.siid == 3);
        TINY_TEST_CHECK(decoded->aid.iid == 1);
        TINY_TEST_CHECK(decoded->in.size == 2);
        TINY_TEST_CHECK(ActionOperation_GetIn(decoded, 5) != NULL);
        TINY_TEST_CHECK(ActionOperation_GetIn(decoded, 6) != NULL);

        TINY_TEST_CHECK(ActionOperationCodec_Invoke_Query_Decode(NULL, decoded) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(ActionOperationCodec_Invoke_Query_Decode(content, NULL) == TINY_RET_E_NULL_PTR);
        JsonObject_Delete(content);
    }

    /* query without aid */
    content = JsonObject_New();
    if (content != NULL)
    {
        TINY_TEST_CHECK(ActionOperationCodec_Invoke_Query_Decode(content, decoded) == TINY_RET_E_PARAM_INVALID);
        JsonObject_Delete(content);
    }

    /* query with an invalid aid */
    content = JsonObject_New();
    if (content != NULL)
    {
        TINY_TEST_CHECK(JsonObject_PutString(content, "aid", "no-separators") == TINY_RET_OK);
        TINY_TEST_CHECK(RET_FAILED(ActionOperationCodec_Invoke_Query_Decode(content, decoded)));
        TINY_TEST_CHECK(decoded->status == -100);
        JsonObject_Delete(content);
    }

    /* result encoding reports the output on success */
    {
        ArgumentOperation *out1 = ActionOperation_EnsureOut(action, 1);
        ArgumentOperation *out2 = ActionOperation_EnsureOut(action, 2);

        TINY_TEST_CHECK(out1 != NULL);
        TINY_TEST_CHECK(out2 != NULL);
        if (out1 != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddInteger(&out1->values, 1) == TINY_RET_OK);
        }
        if (out2 != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddInteger(&out2->values, 2) == TINY_RET_OK);
        }
    }

    content = JsonObject_New();
    TINY_TEST_CHECK(content != NULL);
    if (content != NULL)
    {
        TINY_TEST_CHECK(ActionOperationCodec_Invoke_Result_Encode(action, content));
        TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(content, "aid"), "did-9.3.1") == 0);

        array = JsonObject_GetArray(content, "out");
        TINY_TEST_CHECK(array != NULL);
        if (array != NULL)
        {
            TINY_TEST_CHECK(array->values.size == 2);
        }
        JsonObject_Delete(content);
    }

    /* a failure status is encoded with status and without out */
    action->status = IOT_STATUS_ACTION_IN_ERROR;
    TinyList_RemoveAll(&action->out);

    content = JsonObject_New();
    if (content != NULL)
    {
        TINY_TEST_CHECK(ActionOperationCodec_Invoke_Result_Encode(action, content));
        TINY_TEST_CHECK(JsonObject_ContainsKey(content, "aid"));
        TINY_TEST_CHECK(JsonObject_ContainsKey(content, "status"));
        TINY_TEST_CHECK(!JsonObject_ContainsKey(content, "out"));
        JsonObject_Delete(content);
    }

    /* result decoding: completed with out arguments */
    content = JsonObject_New();
    array = JsonArray_New();
    if (content != NULL && array != NULL)
    {
        JsonObject *entry = JsonObject_New();
        JsonArray *values = JsonArray_New();

        if (entry != NULL && values != NULL)
        {
            TINY_TEST_CHECK(JsonArray_AddInteger(values, 42) == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutInteger(entry, "piid", 1) == TINY_RET_OK);
            if (RET_FAILED(JsonObject_PutArray(entry, "values", values)))
            {
                JsonArray_Delete(values);
                JsonObject_Delete(entry);
                JsonObject_Delete(content);
                JsonArray_Delete(array);
                ActionOperation_Delete(decoded);
                ActionOperation_Delete(action);
                return;
            }

            values = NULL;

            if (RET_FAILED(JsonArray_AddObject(array, entry)))
            {
                JsonObject_Delete(entry);
                JsonObject_Delete(content);
                JsonArray_Delete(array);
                ActionOperation_Delete(decoded);
                ActionOperation_Delete(action);
                return;
            }

            entry = NULL;
        }
        else
        {
            JsonObject_Delete(entry);
            JsonArray_Delete(values);
        }

        TINY_TEST_CHECK(JsonObject_PutString(content, "aid", "did-9.3.1") == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutInteger(content, "status", IOT_STATUS_COMPLETED) == TINY_RET_OK);
        if (RET_FAILED(JsonObject_PutArray(content, "out", array)))
        {
            JsonArray_Delete(array);
            JsonObject_Delete(content);
            ActionOperation_Delete(decoded);
            ActionOperation_Delete(action);
            return;
        }

        array = NULL;

        TINY_TEST_CHECK(ActionOperationCodec_Invoke_Result_Decode(content, decoded) == TINY_RET_OK);
        TINY_TEST_CHECK(decoded->status == IOT_STATUS_COMPLETED);

        {
            ArgumentOperation *out = ActionOperation_GetOut(decoded, 1);

            TINY_TEST_CHECK(out != NULL);
            if (out != NULL)
            {
                TINY_TEST_CHECK(JsonArray_GetSize(&out->values) == 1);
                if (JsonArray_GetSize(&out->values) == 1)
                {
                    JsonValue *value = JsonArray_GetValue(&out->values, 0);

                    TINY_TEST_CHECK(value != NULL);
                    if (value != NULL)
                    {
                        TINY_TEST_CHECK(value->type == JSON_NUMBER);
                        TINY_TEST_CHECK(value->data.number->value.intValue == 42);
                    }
                }
            }
        }
    }
    else
    {
        JsonObject_Delete(content);
        content = NULL;
        JsonArray_Delete(array);
        array = NULL;
    }

    JsonObject_Delete(content);

    ActionOperation_Delete(decoded);
    ActionOperation_Delete(action);
}

int main(void)
{
    TINY_TEST_BEGIN("operations");

    test_property_operation();
    test_property_operations();
    test_action_operation();
    test_event_operation();
    test_set_codec();
    test_get_codec();
    test_action_codec();

    return TINY_TEST_FINISH();
}
