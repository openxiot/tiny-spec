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

#include <tinycore/json_number.h>
#include <tinycore/json_object.h>
#include <tinycore/json_string.h>
#include <tinycore/tiny_list.h>
#include <tinyspec/tinyspec.h>

static void test_xid3(void)
{
    XID3 xid;
    char text[IOT_XID_LENGTH];

    memset(&xid, 0, sizeof(xid));

    TINY_TEST_CHECK(XID3_FromString(&xid, "did-1.2.3") == TINY_RET_OK);
    TINY_TEST_CHECK(strcmp(xid.did, "did-1") == 0);
    TINY_TEST_CHECK(xid.siid == 2);
    TINY_TEST_CHECK(xid.iid == 3);

    XID3_ToString(text, &xid);
    TINY_TEST_CHECK(strcmp(text, "did-1.2.3") == 0);

    TINY_TEST_CHECK(XID3_FromString(&xid, "no-separators") == TINY_RET_E_PARAM_INVALID);
    TINY_TEST_CHECK(XID3_FromString(&xid, "did.1") == TINY_RET_E_PARAM_INVALID);
}

static void test_urn(void)
{
    Urn urn;

    TINY_TEST_CHECK(Urn_Construct(&urn) == TINY_RET_OK);
    TINY_TEST_CHECK(urn.ns == NULL);

    TINY_TEST_CHECK(Urn_Set(&urn, "homekit-spec", PROPERTY, "on", 0x30, NULL) == TINY_RET_OK);
    TINY_TEST_CHECK(strcmp(urn.ns, "homekit-spec") == 0);
    TINY_TEST_CHECK(urn.type == PROPERTY);
    TINY_TEST_CHECK(strcmp(urn.name, "on") == 0);
    TINY_TEST_CHECK(urn.value == 0x30);
    TINY_TEST_CHECK(urn.vendor == NULL);

    Urn_Dispose(&urn);
    TINY_TEST_CHECK(urn.ns == NULL);
    TINY_TEST_CHECK(urn.name == NULL);
    TINY_TEST_CHECK(urn.vendor == NULL);

    TINY_TEST_CHECK(Urn_Construct(&urn) == TINY_RET_OK);
    TINY_TEST_CHECK(Urn_SetString(&urn, "urn:homekit-spec:service:switch:00000014") == TINY_RET_OK);
    TINY_TEST_CHECK(urn.type == SERVICE);
    TINY_TEST_CHECK(strcmp(urn.name, "switch") == 0);
    TINY_TEST_CHECK(urn.value == 0x14);
    TINY_TEST_CHECK(Urn_SetString(&urn, "invalid") == TINY_RET_E_PARAM_INVALID);
    Urn_Dispose(&urn);
}

static void test_operations(void)
{
    PropertyOperation *property = NULL;
    ActionOperation *action = NULL;

    property = PropertyOperation_NewFrom("did-1", 2, 3);
    TINY_TEST_CHECK(property != NULL);
    if (property != NULL)
    {
        TINY_TEST_CHECK(strcmp(property->pid.did, "did-1") == 0);
        TINY_TEST_CHECK(property->pid.siid == 2);
        TINY_TEST_CHECK(property->pid.iid == 3);
        TINY_TEST_CHECK(property->value == NULL);
        TINY_TEST_CHECK(property->status == 0);
        PropertyOperation_Delete(property);
    }

    action = ActionOperation_New();
    TINY_TEST_CHECK(action != NULL);
    if (action != NULL)
    {
        TINY_TEST_CHECK(action->aid.siid == 0);
        TINY_TEST_CHECK(action->aid.iid == 0);
        TINY_TEST_CHECK(action->status == 0);
        TINY_TEST_CHECK(TinyList_IsEmpty(&action->in));
        TINY_TEST_CHECK(TinyList_IsEmpty(&action->out));
        ActionOperation_Delete(action);
    }
}

static void test_service_model(void)
{
    Service *service = NULL;
    Property *property = NULL;
    Action *action = NULL;

    service = Service_NewInstance(1, "homekit-spec", "switch", 0x14, NULL);
    TINY_TEST_CHECK(service != NULL);
    if (service == NULL)
    {
        return;
    }

    TINY_TEST_CHECK(service->iid == 1);
    TINY_TEST_CHECK(service->type.type == SERVICE);
    TINY_TEST_CHECK(strcmp(service->type.ns, "homekit-spec") == 0);
    TINY_TEST_CHECK(strcmp(service->type.name, "switch") == 0);
    TINY_TEST_CHECK(service->type.value == 0x14);

    property = Property_New(2);
    TINY_TEST_CHECK(property != NULL);
    if (property != NULL)
    {
        TINY_TEST_CHECK(TinyList_AddTail(&service->properties, property) != NULL);
        TINY_TEST_CHECK(Service_GetProperty(service, 2) == property);
    }

    TINY_TEST_CHECK(Service_GetProperty(service, 3) == NULL);
    TINY_TEST_CHECK(Service_GetAction(service, 2) == NULL);
    TINY_TEST_CHECK(Service_GetEvent(service, 2) == NULL);

    action = Action_New(4);
    TINY_TEST_CHECK(action != NULL);
    if (action != NULL)
    {
        TINY_TEST_CHECK(Action_InAdd(action, 5) == TINY_RET_OK);
        TINY_TEST_CHECK(Action_InAdd(action, 6) == TINY_RET_OK);
        TINY_TEST_CHECK(Action_OutAdd(action, 7) == TINY_RET_OK);
        TINY_TEST_CHECK(action->in.size == 2);
        TINY_TEST_CHECK(action->out.size == 1);

        TINY_TEST_CHECK(TinyList_AddTail(&service->actions, action) != NULL);
        TINY_TEST_CHECK(Service_GetAction(service, 4) == action);
    }

    Service_Delete(service);
}

static void test_product_model(void)
{
    Product *product = Product_New();
    Device *child = NULL;

    TINY_TEST_CHECK(product != NULL);
    if (product == NULL)
    {
        return;
    }

    TINY_TEST_CHECK(TinyList_IsEmpty(&product->children));
    TINY_TEST_CHECK(TinyList_IsEmpty(&product->device.services));
    TINY_TEST_CHECK(!Product_CheckHandler(product));

    product->onGet = NULL;
    product->onSet = NULL;
    product->onInvoke = NULL;

    child = Device_New();
    TINY_TEST_CHECK(child != NULL);
    if (child != NULL)
    {
        TINY_TEST_CHECK(TinyList_AddTail(&product->children, child) != NULL);
        TINY_TEST_CHECK(product->children.size == 1);
    }

    Product_Delete(product);
}

static void check_iq_method(Stanza *stanza, IQMethod method)
{
    JsonObject *object = StanzaCodec_Encode(stanza);
    TINY_TEST_CHECK(object != NULL);
    if (object == NULL)
    {
        return;
    }

    JsonObject *iq = JsonObject_GetObject(object, "iq");
    TINY_TEST_CHECK(iq != NULL);
    if (iq != NULL)
    {
        JsonString *method_string = JsonObject_GetString(iq, "method");
        TINY_TEST_CHECK(method_string != NULL);
        if (method_string != NULL)
        {
            TINY_TEST_CHECK(strcmp(method_string->value, IQMethod_ToString(method)) == 0);
        }
    }

    JsonObject_Delete(object);
}

static void test_stanza_factories(void)
{
    Stanza *stanza = NULL;
    JsonObject *object = NULL;

    stanza = QueryHello_New("1");
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(stanza->type == STANZA_IQ);
        TINY_TEST_CHECK(stanza->data.iq.type == IQ_TYPE_QUERY);
        TINY_TEST_CHECK(stanza->data.iq.content.query.method == IQ_METHOD_HELLO);
        TINY_TEST_CHECK(strcmp(stanza->data.iq.id, "1") == 0);
        check_iq_method(stanza, IQ_METHOD_HELLO);
        Stanza_Delete(stanza);
    }

    stanza = ResultHello_New("2");
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(stanza->data.iq.type == IQ_TYPE_RESULT);
        TINY_TEST_CHECK(stanza->data.iq.content.result.method == IQ_METHOD_HELLO);
        check_iq_method(stanza, IQ_METHOD_HELLO);
        Stanza_Delete(stanza);
    }

    stanza = Ping_New("3");
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(stanza->data.iq.content.query.method == IQ_METHOD_PING);
        check_iq_method(stanza, IQ_METHOD_PING);
        Stanza_Delete(stanza);
    }

    stanza = IQError_New("4", -3, "action not found");
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(stanza->data.iq.type == IQ_TYPE_ERROR);
        TINY_TEST_CHECK(stanza->data.iq.content.error.status == -3);
        TINY_TEST_CHECK(strcmp(stanza->data.iq.content.error.description, "action not found") == 0);

        object = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(object != NULL);
        if (object != NULL)
        {
            JsonObject *iq = JsonObject_GetObject(object, "iq");
            TINY_TEST_CHECK(iq != NULL);
            if (iq != NULL)
            {
                JsonObject *content = JsonObject_GetObject(iq, "content");
                TINY_TEST_CHECK(content != NULL);
                if (content != NULL)
                {
                    JsonNumber *status = JsonObject_GetNumber(content, "status");
                    TINY_TEST_CHECK(status != NULL);
                    if (status != NULL)
                    {
                        TINY_TEST_CHECK(status->value.intValue == -3);
                    }
                }
            }
            JsonObject_Delete(object);
        }

        Stanza_Delete(stanza);
    }
}

int main(void)
{
    TINY_TEST_BEGIN("model");

    test_xid3();
    test_urn();
    test_operations();
    test_service_model();
    test_product_model();
    test_stanza_factories();

    return TINY_TEST_FINISH();
}
