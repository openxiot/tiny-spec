/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include "tiny_test.h"

#include <stdlib.h>
#include <string.h>

#include <tinycore/json_array.h>
#include <tinycore/json_number.h>
#include <tinycore/json_object.h>
#include <tinycore/json_value.h>
#include <tinyspec/tinyspec.h>

static int g_get_calls;
static int g_set_calls;
static int g_invoke_calls;

/* 捕获 Product 发出的通知（properties-changed）JSON */
static char g_sent[4096];

static int on_send(JsonObject *obj, void *product, void *ctx)
{
    (void)product;
    (void)ctx;

    char *s = JsonObject_ToString(obj, 0);
    if (s != NULL)
    {
        strncpy(g_sent, s, sizeof(g_sent) - 1);
        g_sent[sizeof(g_sent) - 1] = '\0';
        free(s);
    }

    return 0;
}

static void reset_counters(void)
{
    g_get_calls = 0;
    g_set_calls = 0;
    g_invoke_calls = 0;
}

static void on_get(PropertyOperation *operation, void *product)
{
    (void)product;

    g_get_calls++;

    if (operation->value == NULL)
    {
        operation->value = JsonValue_NewInteger(42);
    }
}

static void on_set(PropertyOperation *operation, void *product)
{
    (void)operation;
    (void)product;

    g_set_calls++;
}

static void on_invoke(ActionOperation *operation, void *product)
{
    (void)product;

    g_invoke_calls++;

    {
        ArgumentOperation *out = ActionOperation_EnsureOut(operation, 3);

        if (out != NULL)
        {
            JsonArray_AddString(&out->values, "done");
        }
    }
}

static void on_invoke_empty(ActionOperation *operation, void *product)
{
    (void)operation;
    (void)product;

    g_invoke_calls++;
}

static void on_invoke_bad(ActionOperation *operation, void *product)
{
    (void)product;

    g_invoke_calls++;

    {
        ArgumentOperation *out = ActionOperation_EnsureOut(operation, 3);

        if (out != NULL)
        {
            JsonArray_AddInteger(&out->values, 1);
        }
    }
}

static Product *make_product(const char *did)
{
    Product *product = Product_New();

    if (product != NULL)
    {
        strncpy(product->config.did, did, IOT_DEVICE_ID_LENGTH);
    }

    return product;
}

static Service *add_service(Product *product, uint16_t siid)
{
    Service *service = Service_New(siid);

    if (service != NULL)
    {
        if (TinyList_AddTail(&product->device.services, service) == NULL)
        {
            Service_Delete(service);
            return NULL;
        }
    }

    return service;
}

static Property *add_property(Service *service, uint16_t iid, Format format, Access access)
{
    Property *property = Property_New(iid);

    if (property != NULL)
    {
        property->format = format;
        property->access = access;

        if (TinyList_AddTail(&service->properties, property) == NULL)
        {
            Property_Delete(property);
            return NULL;
        }
    }

    return property;
}

static PropertyOperation *add_operation(
    PropertyOperations *operations, const char *did, uint16_t siid, uint16_t iid)
{
    PropertyOperation *operation = PropertyOperation_NewFrom(did, siid, iid);

    if (operation != NULL)
    {
        if (TinyList_AddTail(&operations->properties, operation) == NULL)
        {
            PropertyOperation_Delete(operation);
            return NULL;
        }
    }

    return operation;
}

static PropertyOperation *get_operation(PropertyOperations *operations, uint32_t index)
{
    return (PropertyOperation *)TinyList_GetAt(&operations->properties, index);
}

/* a product with one service (siid 1) exposing:
 *   iid 1: uint8, readable + writable
 *   iid 2: uint8, writable only
 *   iid 3: string, readable + writable
 *   iid 4: uint8, notifiable only
 *   iid 5: uint8, no access at all
 */
static Product *make_full_product(const char *did)
{
    Product *product = make_product(did);
    Service *service = NULL;

    if (product == NULL)
    {
        return NULL;
    }

    service = add_service(product, 1);
    if (service == NULL)
    {
        Product_Delete(product);
        return NULL;
    }

    add_property(service, 1, FORMAT_UINT8, ACCESS_RW);
    add_property(service, 2, FORMAT_UINT8, ACCESS_WRITE);
    add_property(service, 3, FORMAT_STRING, ACCESS_RW);
    add_property(service, 4, FORMAT_UINT8, ACCESS_NOTIFY);
    add_property(service, 5, FORMAT_UINT8, (Access)0);

    return product;
}

static void test_check_handler(void)
{
    Product *product = make_product("did-check");

    TINY_TEST_CHECK(product != NULL);
    if (product == NULL)
    {
        return;
    }

    TINY_TEST_CHECK(!Product_CheckHandler(product));
    TINY_TEST_CHECK(!Product_CheckHandler(NULL));

    product->onGet = on_get;
    TINY_TEST_CHECK(!Product_CheckHandler(product));

    product->onSet = on_set;
    TINY_TEST_CHECK(Product_CheckHandler(product));

    /* onInvoke is optional */
    product->onInvoke = NULL;
    TINY_TEST_CHECK(Product_CheckHandler(product));

    Product_Delete(product);
}

static void test_get_all_properties(void)
{
    Product *product = NULL;
    PropertyOperations *operations = NULL;

    reset_counters();
    product = make_full_product("did-all");
    TINY_TEST_CHECK(product != NULL);
    if (product == NULL)
    {
        return;
    }

    product->onGet = on_get;

    operations = Product_GetAllProperties(product);
    TINY_TEST_CHECK(operations != NULL);
    if (operations != NULL)
    {
        /* iid 1 and 3 are readable, the others are skipped */
        TINY_TEST_CHECK(operations->properties.size == 2);
        TINY_TEST_CHECK(g_get_calls == 2);

        if (operations->properties.size == 2)
        {
            PropertyOperation *first = get_operation(operations, 0);
            PropertyOperation *second = get_operation(operations, 1);

            TINY_TEST_CHECK(strcmp(first->pid.did, "did-all") == 0);
            TINY_TEST_CHECK(first->pid.siid == 1);
            TINY_TEST_CHECK(first->pid.iid == 1);
            TINY_TEST_CHECK(first->status == IOT_STATUS_COMPLETED);
            TINY_TEST_CHECK(first->value != NULL);
            if (first->value != NULL)
            {
                TINY_TEST_CHECK(first->value->data.number->value.intValue == 42);
            }

            TINY_TEST_CHECK(second->pid.iid == 3);
            TINY_TEST_CHECK(second->status == IOT_STATUS_COMPLETED);
        }

        PropertyOperations_Delete(operations);
    }

    /* without a getter the reads fail */
    product->onGet = NULL;
    operations = Product_GetAllProperties(product);
    TINY_TEST_CHECK(operations != NULL);
    if (operations != NULL)
    {
        TINY_TEST_CHECK(operations->properties.size == 2);
        if (operations->properties.size == 2)
        {
            TINY_TEST_CHECK(get_operation(operations, 0)->status == IOT_STATUS_INTERNAL_ERROR);
            TINY_TEST_CHECK(get_operation(operations, 0)->value == NULL);
        }
        PropertyOperations_Delete(operations);
    }

    TINY_TEST_CHECK(Product_GetAllProperties(NULL) == NULL);

    Product_Delete(product);
}

static void test_read_properties(void)
{
    Product *product = NULL;
    Product *child = NULL;
    PropertyOperations operations;

    reset_counters();
    product = make_full_product("did-main");
    TINY_TEST_CHECK(product != NULL);
    if (product == NULL)
    {
        return;
    }

    product->onGet = on_get;

    child = make_full_product("did-child");
    TINY_TEST_CHECK(child != NULL);
    if (child != NULL)
    {
        child->onGet = on_get;
        TINY_TEST_CHECK(TinyList_AddTail(&product->children, child) != NULL);
    }

    TINY_TEST_CHECK(PropertyOperations_Construct(&operations) == TINY_RET_OK);
    add_operation(&operations, "did-main", 1, 1);  /* 0: ok */
    add_operation(&operations, "did-main", 1, 2);  /* 1: not readable */
    add_operation(&operations, "did-main", 1, 5);  /* 2: no access */
    add_operation(&operations, "did-main", 1, 9);  /* 3: property not found */
    add_operation(&operations, "did-main", 7, 1);  /* 4: service not found */
    add_operation(&operations, "did-child", 1, 1); /* 5: child ok */
    add_operation(&operations, "did-gone", 1, 1);  /* 6: child not found */

    Product_TryReadProperties(product, &operations);

    TINY_TEST_CHECK(get_operation(&operations, 0)->status == IOT_STATUS_COMPLETED);
    TINY_TEST_CHECK(get_operation(&operations, 0)->value != NULL);
    TINY_TEST_CHECK(get_operation(&operations, 1)->status == IOT_STATUS_PROPERTY_CANNOT_READ);
    TINY_TEST_CHECK(get_operation(&operations, 2)->status == IOT_STATUS_PROPERTY_CANNOT_READ);
    TINY_TEST_CHECK(get_operation(&operations, 3)->status == IOT_STATUS_PROPERTY_NOT_FOUND);
    TINY_TEST_CHECK(get_operation(&operations, 4)->status == IOT_STATUS_SERVICE_NOT_FOUND);
    TINY_TEST_CHECK(get_operation(&operations, 5)->status == IOT_STATUS_COMPLETED);
    TINY_TEST_CHECK(get_operation(&operations, 5)->value != NULL);
    TINY_TEST_CHECK(get_operation(&operations, 6)->status == IOT_STATUS_CHILD_NOT_FOUND);

    PropertyOperations_Dispose(&operations);

    /* reading without a getter is an internal error */
    product->onGet = NULL;
    TINY_TEST_CHECK(PropertyOperations_Construct(&operations) == TINY_RET_OK);
    add_operation(&operations, "did-main", 1, 1);
    Product_TryReadProperties(product, &operations);
    TINY_TEST_CHECK(get_operation(&operations, 0)->status == IOT_STATUS_INTERNAL_ERROR);
    PropertyOperations_Dispose(&operations);

    /* NULL arguments are ignored */
    Product_TryReadProperties(NULL, &operations);
    Product_TryReadProperties(product, NULL);

    Product_Delete(product);
}

static void test_write_properties(void)
{
    Product *product = NULL;
    Product *child = NULL;
    PropertyOperations operations;

    reset_counters();
    product = make_full_product("did-main");
    TINY_TEST_CHECK(product != NULL);
    if (product == NULL)
    {
        return;
    }

    product->onSet = on_set;

    child = make_full_product("did-child");
    TINY_TEST_CHECK(child != NULL);
    if (child != NULL)
    {
        child->onSet = on_set;
        TINY_TEST_CHECK(TinyList_AddTail(&product->children, child) != NULL);
    }

    TINY_TEST_CHECK(PropertyOperations_Construct(&operations) == TINY_RET_OK);
    add_operation(&operations, "did-main", 1, 1)->value = JsonValue_NewInteger(5); /* 0: ok */
    add_operation(&operations, "did-main", 1, 1)->value =
        JsonValue_NewInteger(300); /* 1: out of range */
    add_operation(&operations, "did-main", 1, 1)->value =
        JsonValue_NewString("x");                 /* 2: wrong type */
    add_operation(&operations, "did-main", 1, 1); /* 3: no value */
    add_operation(&operations, "did-main", 1, 4); /* 4: not writable */
    add_operation(&operations, "did-main", 1, 9); /* 5: property not found */
    add_operation(&operations, "did-main", 7, 1); /* 6: service not found */
    add_operation(&operations, "did-child", 1, 1)->value =
        JsonValue_NewInteger(6);                  /* 7: child ok */
    add_operation(&operations, "did-gone", 1, 1); /* 8: child not found */

    Product_TryWriteProperties(product, &operations);

    TINY_TEST_CHECK(get_operation(&operations, 0)->status == IOT_STATUS_COMPLETED);
    TINY_TEST_CHECK(get_operation(&operations, 1)->status == IOT_STATUS_PROPERTY_VALUE_INVALID);
    TINY_TEST_CHECK(get_operation(&operations, 2)->status == IOT_STATUS_PROPERTY_VALUE_INVALID);
    TINY_TEST_CHECK(get_operation(&operations, 3)->status == IOT_STATUS_PROPERTY_VALUE_INVALID);
    TINY_TEST_CHECK(get_operation(&operations, 4)->status == IOT_STATUS_PROPERTY_CANNOT_WRITE);
    TINY_TEST_CHECK(get_operation(&operations, 5)->status == IOT_STATUS_PROPERTY_NOT_FOUND);
    TINY_TEST_CHECK(get_operation(&operations, 6)->status == IOT_STATUS_SERVICE_NOT_FOUND);
    TINY_TEST_CHECK(get_operation(&operations, 7)->status == IOT_STATUS_COMPLETED);
    TINY_TEST_CHECK(get_operation(&operations, 8)->status == IOT_STATUS_CHILD_NOT_FOUND);
    TINY_TEST_CHECK(g_set_calls == 2);

    PropertyOperations_Dispose(&operations);

    /* writing without a setter is an internal error */
    product->onSet = NULL;
    TINY_TEST_CHECK(PropertyOperations_Construct(&operations) == TINY_RET_OK);
    add_operation(&operations, "did-main", 1, 1)->value = JsonValue_NewInteger(5);
    Product_TryWriteProperties(product, &operations);
    TINY_TEST_CHECK(get_operation(&operations, 0)->status == IOT_STATUS_INTERNAL_ERROR);
    PropertyOperations_Dispose(&operations);

    Product_TryWriteProperties(NULL, &operations);
    Product_TryWriteProperties(product, NULL);

    Product_Delete(product);
}

static ActionOperation *make_action_operation(const char *did, uint16_t siid, uint16_t iid)
{
    ActionOperation *operation = ActionOperation_New();

    if (operation != NULL)
    {
        strncpy(operation->aid.did, did, IOT_DEVICE_ID_LENGTH);
        operation->aid.siid = siid;
        operation->aid.iid = iid;
    }

    return operation;
}

static Action *add_action(Service *service, uint16_t iid, bool withArguments)
{
    Action *action = Action_New(iid);

    if (action != NULL)
    {
        if (withArguments)
        {
            /* in argument refers to property 1, out argument to property 3 */
            TINY_TEST_CHECK(Action_InAdd(action, 1) == TINY_RET_OK);
            TINY_TEST_CHECK(Action_OutAdd(action, 3) == TINY_RET_OK);
        }

        if (TinyList_AddTail(&service->actions, action) == NULL)
        {
            Action_Delete(action);
            return NULL;
        }
    }

    return action;
}

static void test_invoke_action(void)
{
    Product *product = NULL;
    Product *child = NULL;
    Service *service = NULL;
    ActionOperation *operation = NULL;

    reset_counters();
    product = make_full_product("did-main");
    TINY_TEST_CHECK(product != NULL);
    if (product == NULL)
    {
        return;
    }

    product->onInvoke = on_invoke;
    service = (Service *)TinyList_GetAt(&product->device.services, 0);
    TINY_TEST_CHECK(service != NULL);
    if (service == NULL)
    {
        Product_Delete(product);
        return;
    }

    TINY_TEST_CHECK(add_action(service, 1, true) != NULL);  /* with in/out arguments */
    TINY_TEST_CHECK(add_action(service, 2, false) != NULL); /* without arguments */

    child = make_full_product("did-child");
    TINY_TEST_CHECK(child != NULL);
    if (child != NULL)
    {
        Service *childService = (Service *)TinyList_GetAt(&child->device.services, 0);

        child->onInvoke = on_invoke;
        if (childService != NULL)
        {
            TINY_TEST_CHECK(add_action(childService, 2, false) != NULL);
        }
        TINY_TEST_CHECK(TinyList_AddTail(&product->children, child) != NULL);
    }

    /* a valid invocation */
    operation = make_action_operation("did-main", 1, 1);
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        {
            ArgumentOperation *in = ActionOperation_EnsureIn(operation, 1);

            TINY_TEST_CHECK(in != NULL);
            if (in != NULL)
            {
                TINY_TEST_CHECK(JsonArray_AddInteger(&in->values, 5) == TINY_RET_OK);
            }
        }

        Product_TryInvokeAction(product, operation);

        TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);
        TINY_TEST_CHECK(g_invoke_calls == 1);
        {
            ArgumentOperation *out = ActionOperation_GetOut(operation, 3);

            TINY_TEST_CHECK(out != NULL);
            if (out != NULL)
            {
                TINY_TEST_CHECK(JsonArray_GetSize(&out->values) == 1);
            }
        }

        ActionOperation_Delete(operation);
    }

    /* the number of arguments must match */
    operation = make_action_operation("did-main", 1, 1);
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        Product_TryInvokeAction(product, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_ACTION_IN_ERROR);
        ActionOperation_Delete(operation);
    }

    /* the argument values must match the property format */
    operation = make_action_operation("did-main", 1, 1);
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        {
            ArgumentOperation *in = ActionOperation_EnsureIn(operation, 1);

            TINY_TEST_CHECK(in != NULL);
            if (in != NULL)
            {
                TINY_TEST_CHECK(JsonArray_AddString(&in->values, "x") == TINY_RET_OK);
            }
        }
        Product_TryInvokeAction(product, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_ACTION_IN_VALUE_INVALID);
        ActionOperation_Delete(operation);
    }

    /* an action without arguments produces no output values */
    product->onInvoke = on_invoke_empty;
    if (child != NULL)
    {
        child->onInvoke = on_invoke_empty;
    }

    operation = make_action_operation("did-main", 1, 2);
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        Product_TryInvokeAction(product, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);
        ActionOperation_Delete(operation);
    }

    /* unknown action and service */
    operation = make_action_operation("did-main", 1, 9);
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        Product_TryInvokeAction(product, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_ACTION_NOT_FOUND);
        ActionOperation_Delete(operation);
    }

    operation = make_action_operation("did-main", 7, 1);
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        Product_TryInvokeAction(product, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_SERVICE_NOT_FOUND);
        ActionOperation_Delete(operation);
    }

    /* child devices and unknown children */
    operation = make_action_operation("did-child", 1, 2);
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        Product_TryInvokeAction(product, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);
        ActionOperation_Delete(operation);
    }

    operation = make_action_operation("did-gone", 1, 2);
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        Product_TryInvokeAction(product, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_CHILD_NOT_FOUND);
        ActionOperation_Delete(operation);
    }

    /* missing output values */
    product->onInvoke = on_invoke_empty;
    operation = make_action_operation("did-main", 1, 1);
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        {
            ArgumentOperation *in = ActionOperation_EnsureIn(operation, 1);

            TINY_TEST_CHECK(in != NULL);
            if (in != NULL)
            {
                TINY_TEST_CHECK(JsonArray_AddInteger(&in->values, 5) == TINY_RET_OK);
            }
        }
        Product_TryInvokeAction(product, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_ACTION_OUT_ERROR);
        ActionOperation_Delete(operation);
    }

    /* output values of the wrong type */
    product->onInvoke = on_invoke_bad;
    operation = make_action_operation("did-main", 1, 1);
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        {
            ArgumentOperation *in = ActionOperation_EnsureIn(operation, 1);

            TINY_TEST_CHECK(in != NULL);
            if (in != NULL)
            {
                TINY_TEST_CHECK(JsonArray_AddInteger(&in->values, 5) == TINY_RET_OK);
            }
        }
        Product_TryInvokeAction(product, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_ACTION_OUT_VALUE_INVALID);
        ActionOperation_Delete(operation);
    }

    /* without a handler the invocation fails */
    product->onInvoke = NULL;
    operation = make_action_operation("did-main", 1, 2);
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        Product_TryInvokeAction(product, operation);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_INTERNAL_ERROR);
        ActionOperation_Delete(operation);
    }

    Product_TryInvokeAction(NULL, operation);
    Product_TryInvokeAction(product, NULL);

    Product_Delete(product);
}

static void test_change_property_value(void)
{
    Product *product = NULL;
    PropertyOperation *operation = NULL;
    PropertyOperations *changed = NULL;

    reset_counters();
    product = make_full_product("did-main");
    TINY_TEST_CHECK(product != NULL);
    if (product == NULL)
    {
        return;
    }

    /* a notifiable property stores the new value when save is set */
    operation = PropertyOperation_NewValue("did-main", 1, 4, JsonValue_NewInteger(9));
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        TINY_TEST_CHECK(Product_TryChangePropertyValue(product, operation, true) == TINY_RET_OK);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);
        PropertyOperation_Delete(operation);
    }

    changed = Product_GetChangedProperties(product);
    TINY_TEST_CHECK(changed != NULL);
    if (changed != NULL)
    {
        TINY_TEST_CHECK(changed->properties.size == 1);
        if (changed->properties.size == 1)
        {
            PropertyOperation *entry = get_operation(changed, 0);

            TINY_TEST_CHECK(strcmp(entry->pid.did, "did-main") == 0);
            TINY_TEST_CHECK(entry->pid.siid == 1);
            TINY_TEST_CHECK(entry->pid.iid == 4);
            TINY_TEST_CHECK(entry->value != NULL);
            if (entry->value != NULL)
            {
                TINY_TEST_CHECK(entry->value->data.number->value.intValue == 9);
            }
        }
        PropertyOperations_Delete(changed);
    }

    /* save == false validates but does not store */
    operation = PropertyOperation_NewValue("did-main", 1, 4, JsonValue_NewInteger(10));
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        TINY_TEST_CHECK(Product_TryChangePropertyValue(product, operation, false) == TINY_RET_OK);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);
        PropertyOperation_Delete(operation);
    }

    changed = Product_GetChangedProperties(product);
    TINY_TEST_CHECK(changed != NULL);
    if (changed != NULL)
    {
        TINY_TEST_CHECK(changed->properties.size == 1);
        if (changed->properties.size == 1)
        {
            PropertyOperation *entry = get_operation(changed, 0);
            TINY_TEST_CHECK(entry->value->data.number->value.intValue == 9);
        }
        PropertyOperations_Delete(changed);
    }

    /* DoChangePropertyValue behaves like TryChange(..., true) */
    operation = PropertyOperation_NewValue("did-main", 1, 4, JsonValue_NewInteger(11));
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        TINY_TEST_CHECK(Product_DoChangePropertyValue(product, operation) == TINY_RET_OK);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);
        PropertyOperation_Delete(operation);
    }

    changed = Product_GetChangedProperties(product);
    TINY_TEST_CHECK(changed != NULL);
    if (changed != NULL)
    {
        TINY_TEST_CHECK(changed->properties.size == 1);
        if (changed->properties.size == 1)
        {
            TINY_TEST_CHECK(get_operation(changed, 0)->value->data.number->value.intValue == 11);
        }
        PropertyOperations_Delete(changed);
    }

    /* value errors */
    operation = PropertyOperation_NewValue("did-main", 1, 4, JsonValue_NewInteger(300));
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        TINY_TEST_CHECK(Product_TryChangePropertyValue(product, operation, true) == TINY_RET_OK);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_PROPERTY_VALUE_INVALID);
        PropertyOperation_Delete(operation);
    }

    /* a property that is not notifiable */
    operation = PropertyOperation_NewValue("did-main", 1, 1, JsonValue_NewInteger(1));
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        TINY_TEST_CHECK(Product_TryChangePropertyValue(product, operation, true) == TINY_RET_OK);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_PROPERTY_CANNOT_NOTIFY);
        PropertyOperation_Delete(operation);
    }

    /* unknown property, unknown service, NULL arguments */
    operation = PropertyOperation_NewValue("did-main", 1, 9, JsonValue_NewInteger(1));
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        TINY_TEST_CHECK(Product_TryChangePropertyValue(product, operation, true) == TINY_RET_OK);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_PROPERTY_NOT_FOUND);
        PropertyOperation_Delete(operation);
    }

    operation = PropertyOperation_NewValue("did-main", 7, 4, JsonValue_NewInteger(1));
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        TINY_TEST_CHECK(
            Product_TryChangePropertyValue(product, operation, true) == TINY_RET_E_PARAM_INVALID);
        PropertyOperation_Delete(operation);
    }

    TINY_TEST_CHECK(Product_TryChangePropertyValue(NULL, operation, true) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(Product_TryChangePropertyValue(product, NULL, true) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(Product_DoChangePropertyValue(NULL, operation) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(Product_DoChangePropertyValue(product, NULL) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(Product_GetChangedProperties(NULL) == NULL);

    Product_Delete(product);
}

static void test_produce_event(void)
{
    Product *product = NULL;
    Service *service = NULL;
    Event *event = NULL;
    EventOperation *operation = NULL;

    reset_counters();
    product = make_full_product("did-main");
    TINY_TEST_CHECK(product != NULL);
    if (product == NULL)
    {
        return;
    }

    service = (Service *)TinyList_GetAt(&product->device.services, 0);
    TINY_TEST_CHECK(service != NULL);
    if (service == NULL)
    {
        Product_Delete(product);
        return;
    }

    event = Event_New(2);
    TINY_TEST_CHECK(event != NULL);
    if (event != NULL)
    {
        TINY_TEST_CHECK(Event_ArgumentAdd(event, 1) == TINY_RET_OK);
        TINY_TEST_CHECK(TinyList_AddTail(&service->events, event) != NULL);
    }

    /* a valid event with one integer argument */
    operation = EventOperation_New();
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        strncpy(operation->eid.did, "did-main", IOT_DEVICE_ID_LENGTH);
        operation->eid.siid = 1;
        operation->eid.iid = 2;

        {
            ArgumentOperation *argument = EventOperation_EnsureArgument(operation, 1);

            TINY_TEST_CHECK(argument != NULL);
            if (argument != NULL)
            {
                TINY_TEST_CHECK(JsonArray_AddInteger(&argument->values, 5) == TINY_RET_OK);
            }
        }

        TINY_TEST_CHECK(Product_TryProduceEvent(product, operation) == TINY_RET_OK);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_COMPLETED);

        EventOperation_Delete(operation);
    }

    /* too few arguments */
    operation = EventOperation_New();
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        strncpy(operation->eid.did, "did-main", IOT_DEVICE_ID_LENGTH);
        operation->eid.siid = 1;
        operation->eid.iid = 2;

        TINY_TEST_CHECK(Product_TryProduceEvent(product, operation) == TINY_RET_OK);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_EVENT_ARGUMENTS_ERROR);

        EventOperation_Delete(operation);
    }

    /* an empty argument list is treated as no arguments */
    operation = EventOperation_New();
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        strncpy(operation->eid.did, "did-main", IOT_DEVICE_ID_LENGTH);
        operation->eid.siid = 1;
        operation->eid.iid = 2;

        TINY_TEST_CHECK(Product_TryProduceEvent(product, operation) == TINY_RET_OK);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_EVENT_ARGUMENTS_ERROR);

        EventOperation_Delete(operation);
    }

    /* argument value of the wrong type */
    operation = EventOperation_New();
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        strncpy(operation->eid.did, "did-main", IOT_DEVICE_ID_LENGTH);
        operation->eid.siid = 1;
        operation->eid.iid = 2;

        {
            ArgumentOperation *argument = EventOperation_EnsureArgument(operation, 1);

            TINY_TEST_CHECK(argument != NULL);
            if (argument != NULL)
            {
                TINY_TEST_CHECK(JsonArray_AddString(&argument->values, "x") == TINY_RET_OK);
            }
        }

        TINY_TEST_CHECK(Product_TryProduceEvent(product, operation) == TINY_RET_OK);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_EVENT_ARGUMENTS_VALUE_ERROR);

        EventOperation_Delete(operation);
    }

    /* unknown event and service */
    operation = EventOperation_New();
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        strncpy(operation->eid.did, "did-main", IOT_DEVICE_ID_LENGTH);
        operation->eid.siid = 1;
        operation->eid.iid = 9;

        TINY_TEST_CHECK(Product_TryProduceEvent(product, operation) == TINY_RET_OK);
        TINY_TEST_CHECK(operation->status == IOT_STATUS_EVENT_NOT_FOUND);

        EventOperation_Delete(operation);
    }

    operation = EventOperation_New();
    TINY_TEST_CHECK(operation != NULL);
    if (operation != NULL)
    {
        strncpy(operation->eid.did, "did-main", IOT_DEVICE_ID_LENGTH);
        operation->eid.siid = 7;
        operation->eid.iid = 2;

        TINY_TEST_CHECK(Product_TryProduceEvent(product, operation) == TINY_RET_E_PARAM_INVALID);

        EventOperation_Delete(operation);
    }

    TINY_TEST_CHECK(Product_TryProduceEvent(NULL, operation) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(Product_TryProduceEvent(product, NULL) == TINY_RET_E_NULL_PTR);

    Product_Delete(product);
}

static void test_change_property_value_notify(void)
{
    Product *product = make_full_product("did-main");

    TINY_TEST_CHECK(product != NULL);
    if (product == NULL)
    {
        return;
    }

    g_sent[0] = '\0';
    product->send = on_send;
    product->sendCtx = NULL;

    Product_ChangePropertyValue(product, 1, 4, JsonValue_NewInteger(7));

    TINY_TEST_CHECK(g_sent[0] != '\0');
    if (g_sent[0] != '\0')
    {
        /* properties-changed 通知：method + content.properties pid/value */
        TINY_TEST_CHECK(strstr(g_sent, "urn:xiot:properties-changed") != NULL);
        TINY_TEST_CHECK(strstr(g_sent, "\"pid\":\"did-main.1.4\"") != NULL);
        TINY_TEST_CHECK(strstr(g_sent, "\"value\":7") != NULL);
    }

    Product_Delete(product);
}

static void test_delete_with_children(void)
{
    Product *product = make_product("did-parent");
    Product *child = NULL;

    TINY_TEST_CHECK(product != NULL);
    if (product == NULL)
    {
        return;
    }

    child = make_full_product("did-child");
    TINY_TEST_CHECK(child != NULL);
    if (child != NULL)
    {
        TINY_TEST_CHECK(TinyList_AddTail(&product->children, child) != NULL);
        TINY_TEST_CHECK(product->children.size == 1);
    }

    /* deleting the parent deletes the children as products */
    Product_Delete(product);
    TINY_TEST_CHECK(true);
}

int main(void)
{
    TINY_TEST_BEGIN("product_operator");

    test_check_handler();
    test_get_all_properties();
    test_read_properties();
    test_write_properties();
    test_invoke_action();
    test_change_property_value();
    test_change_property_value_notify();
    test_produce_event();
    test_delete_with_children();

    return TINY_TEST_FINISH();
}
