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
#include <tinyspec/tinyspec.h>

static void test_type_strings(void)
{
    TINY_TEST_CHECK(strcmp(device_notice_type_to_string(DEVICE_NOTICE_PROPERTY_CHANGED), "property-changed") == 0);
    TINY_TEST_CHECK(strcmp(device_notice_type_to_string(DEVICE_NOTICE_EVENT_OCCURRED), "event-occurred") == 0);
    TINY_TEST_CHECK(strcmp(device_notice_type_to_string(DEVICE_NOTICE_UNDEFINED), "undefined") == 0);

    TINY_TEST_CHECK(device_notice_type_from_string("property-changed") == DEVICE_NOTICE_PROPERTY_CHANGED);
    TINY_TEST_CHECK(device_notice_type_from_string("event-occurred") == DEVICE_NOTICE_EVENT_OCCURRED);
    TINY_TEST_CHECK(device_notice_type_from_string("unknown") == DEVICE_NOTICE_UNDEFINED);
    TINY_TEST_CHECK(device_notice_type_from_string(NULL) == DEVICE_NOTICE_UNDEFINED);

    TINY_TEST_CHECK(strcmp(node_notice_type_to_string(NODE_NOTICE_NODE_FOUND), "node-found") == 0);
    TINY_TEST_CHECK(strcmp(node_notice_type_to_string(NODE_NOTICE_NODE_LOST), "node-lost") == 0);
    TINY_TEST_CHECK(strcmp(node_notice_type_to_string(NODE_NOTICE_UNDEFINED), "undefined") == 0);

    TINY_TEST_CHECK(node_notice_type_from_string("node-found") == NODE_NOTICE_NODE_FOUND);
    TINY_TEST_CHECK(node_notice_type_from_string("node-lost") == NODE_NOTICE_NODE_LOST);
    TINY_TEST_CHECK(node_notice_type_from_string("unknown") == NODE_NOTICE_UNDEFINED);
    TINY_TEST_CHECK(node_notice_type_from_string(NULL) == NODE_NOTICE_UNDEFINED);

    TINY_TEST_CHECK(strcmp(message_topic_to_string(MESSAGE_TOPIC_DEVICE), "urn:xiot:device") == 0);
    TINY_TEST_CHECK(strcmp(message_topic_to_string(MESSAGE_TOPIC_NODE), "urn:xiot:node") == 0);
    TINY_TEST_CHECK(strcmp(message_topic_to_string(MESSAGE_TOPIC_UNDEFINED), "undefined") == 0);

    TINY_TEST_CHECK(message_topic_from_string("urn:xiot:device") == MESSAGE_TOPIC_DEVICE);
    TINY_TEST_CHECK(message_topic_from_string("urn:xiot:node") == MESSAGE_TOPIC_NODE);
    TINY_TEST_CHECK(message_topic_from_string("urn:xiot:unknown") == MESSAGE_TOPIC_UNDEFINED);
    TINY_TEST_CHECK(message_topic_from_string(NULL) == MESSAGE_TOPIC_UNDEFINED);
}

static void test_property_changed(void)
{
    DevicePropertyChanged changed;
    JsonObject *payload = NULL;
    JsonValue *value = NULL;

    TINY_TEST_CHECK(DevicePropertyChanged_Construct(NULL) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(DevicePropertyChanged_Construct(&changed) == TINY_RET_OK);
    TINY_TEST_CHECK(changed.pid.did[0] == '\0');
    TINY_TEST_CHECK(changed.value == NULL);
    DevicePropertyChanged_Dispose(&changed);

    TINY_TEST_CHECK(DevicePropertyChanged_Construct(&changed) == TINY_RET_OK);
    strncpy(changed.pid.did, "did-1", IOT_DEVICE_ID_LENGTH);
    changed.pid.siid = 2;
    changed.pid.iid = 3;
    changed.value = JsonValue_NewInteger(5);

    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(DevicePropertyChangedCodec_Encode(payload, &changed) == TINY_RET_OK);
        TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(payload, "pid"), "did-1.2.3") == 0);

        JsonNumber *number = JsonObject_GetNumber(payload, "value");
        TINY_TEST_CHECK(number != NULL);
        if (number != NULL)
        {
            TINY_TEST_CHECK(number->value.intValue == 5);
        }

        /* the model value is not consumed by the encoder */
        TINY_TEST_CHECK(changed.value != NULL);

        TINY_TEST_CHECK(DevicePropertyChangedCodec_Encode(NULL, &changed) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(DevicePropertyChangedCodec_Encode(payload, NULL) == TINY_RET_E_NULL_PTR);

        JsonObject_Delete(payload);
    }

    DevicePropertyChanged_Dispose(&changed);
    TINY_TEST_CHECK(changed.value == NULL);

    /* decoding */
    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(DevicePropertyChanged_Construct(&changed) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutString(payload, "pid", "did-9.1.8") == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutString(payload, "value", "on") == TINY_RET_OK);

        TINY_TEST_CHECK(DevicePropertyChangedCodec_Decode(&changed, payload) == TINY_RET_OK);
        TINY_TEST_CHECK(strcmp(changed.pid.did, "did-9") == 0);
        TINY_TEST_CHECK(changed.pid.siid == 1);
        TINY_TEST_CHECK(changed.pid.iid == 8);
        TINY_TEST_CHECK(changed.value != NULL);
        if (changed.value != NULL)
        {
            TINY_TEST_CHECK(changed.value->type == JSON_STRING);
            TINY_TEST_CHECK(strcmp(changed.value->data.string->value, "on") == 0);
        }
        DevicePropertyChanged_Dispose(&changed);

        TINY_TEST_CHECK(changed.value == NULL);

        /* missing fields */
        TINY_TEST_CHECK(DevicePropertyChanged_Construct(&changed) == TINY_RET_OK);
        TINY_TEST_CHECK(DevicePropertyChangedCodec_Decode(&changed, payload) == TINY_RET_OK);
        DevicePropertyChanged_Dispose(&changed);

        TINY_TEST_CHECK(DevicePropertyChangedCodec_Decode(NULL, payload) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(DevicePropertyChangedCodec_Decode(&changed, NULL) == TINY_RET_E_NULL_PTR);
        JsonObject_Delete(payload);
    }

    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(DevicePropertyChanged_Construct(&changed) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutString(payload, "pid", "did-9.1.8") == TINY_RET_OK);
        TINY_TEST_CHECK(DevicePropertyChangedCodec_Decode(&changed, payload) == TINY_RET_E_PARAM_INVALID);
        DevicePropertyChanged_Dispose(&changed);
        JsonObject_Delete(payload);
    }

    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(DevicePropertyChanged_Construct(&changed) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutInteger(payload, "value", 1) == TINY_RET_OK);
        TINY_TEST_CHECK(DevicePropertyChangedCodec_Decode(&changed, payload) == TINY_RET_E_PARAM_INVALID);
        DevicePropertyChanged_Dispose(&changed);
        JsonObject_Delete(payload);
    }

    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(DevicePropertyChanged_Construct(&changed) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutString(payload, "pid", "no-separators") == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutInteger(payload, "value", 1) == TINY_RET_OK);
        TINY_TEST_CHECK(DevicePropertyChangedCodec_Decode(&changed, payload) == TINY_RET_E_PARAM_INVALID);
        DevicePropertyChanged_Dispose(&changed);
        JsonObject_Delete(payload);
    }

    /* the encoder rejects a notice without a value */
    TINY_TEST_CHECK(DevicePropertyChanged_Construct(&changed) == TINY_RET_OK);
    payload = JsonObject_New();
    if (payload != NULL)
    {
        TINY_TEST_CHECK(RET_FAILED(DevicePropertyChangedCodec_Encode(payload, &changed)));
        JsonObject_Delete(payload);
    }
    DevicePropertyChanged_Dispose(&changed);

    value = JsonValue_NewInteger(1);
    JsonValue_Delete(value);
}

static void test_event_occurred(void)
{
    DeviceEventOccurred occurred;
    JsonObject *payload = NULL;
    JsonArray *arguments = NULL;

    TINY_TEST_CHECK(DeviceEventOccurred_Construct(NULL) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(DeviceEventOccurred_Construct(&occurred) == TINY_RET_OK);
    TINY_TEST_CHECK(occurred.eid.did[0] == '\0');
    TINY_TEST_CHECK(occurred.argument == NULL);

    strncpy(occurred.eid.did, "did-1", IOT_DEVICE_ID_LENGTH);
    occurred.eid.siid = 2;
    occurred.eid.iid = 7;

    arguments = JsonArray_New();
    TINY_TEST_CHECK(arguments != NULL);
    if (arguments != NULL)
    {
        TINY_TEST_CHECK(JsonArray_AddInteger(arguments, 1) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonArray_AddString(arguments, "a") == TINY_RET_OK);
        occurred.argument = arguments;

        payload = JsonObject_New();
        TINY_TEST_CHECK(payload != NULL);
        if (payload != NULL)
        {
            TINY_TEST_CHECK(DeviceEventOccurredCodec_Encode(payload, &occurred) == TINY_RET_OK);
            TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(payload, "eid"), "did-1.2.7") == 0);

            JsonArray *encoded = JsonObject_GetArray(payload, "arguments");
            TINY_TEST_CHECK(encoded != NULL);
            if (encoded != NULL)
            {
                TINY_TEST_CHECK(encoded->values.size == 2);
            }

            TINY_TEST_CHECK(DeviceEventOccurredCodec_Encode(NULL, &occurred) == TINY_RET_E_NULL_PTR);
            TINY_TEST_CHECK(DeviceEventOccurredCodec_Encode(payload, NULL) == TINY_RET_E_NULL_PTR);

            JsonObject_Delete(payload);
        }
    }

    /* encode without arguments only emits the eid */
    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        DeviceEventOccurred bare;

        TINY_TEST_CHECK(DeviceEventOccurred_Construct(&bare) == TINY_RET_OK);
        strncpy(bare.eid.did, "did-1", IOT_DEVICE_ID_LENGTH);
        bare.eid.siid = 2;
        bare.eid.iid = 7;

        TINY_TEST_CHECK(DeviceEventOccurredCodec_Encode(payload, &bare) == TINY_RET_OK);
        TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(payload, "eid"), "did-1.2.7") == 0);
        TINY_TEST_CHECK(!JsonObject_ContainsKey(payload, "arguments"));

        DeviceEventOccurred_Dispose(&bare);
        JsonObject_Delete(payload);
    }

    DeviceEventOccurred_Dispose(&occurred);
    TINY_TEST_CHECK(occurred.argument == NULL);

    /* decoding */
    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        DeviceEventOccurred decoded;

        TINY_TEST_CHECK(DeviceEventOccurred_Construct(&decoded) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutString(payload, "eid", "did-3.4.5") == TINY_RET_OK);

        arguments = JsonArray_New();
        TINY_TEST_CHECK(arguments != NULL);
        if (arguments != NULL)
        {
            JsonArray_AddInteger(arguments, 9);
            TINY_TEST_CHECK(JsonObject_PutArray(payload, "arguments", arguments) == TINY_RET_OK);
        }

        TINY_TEST_CHECK(DeviceEventOccurredCodec_Decode(&decoded, payload) == TINY_RET_OK);
        TINY_TEST_CHECK(strcmp(decoded.eid.did, "did-3") == 0);
        TINY_TEST_CHECK(decoded.eid.siid == 4);
        TINY_TEST_CHECK(decoded.eid.iid == 5);
        TINY_TEST_CHECK(decoded.argument != NULL);
        if (decoded.argument != NULL)
        {
            TINY_TEST_CHECK(decoded.argument->values.size == 1);
        }
        DeviceEventOccurred_Dispose(&decoded);

        TINY_TEST_CHECK(DeviceEventOccurredCodec_Decode(NULL, payload) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(DeviceEventOccurredCodec_Decode(&decoded, NULL) == TINY_RET_E_NULL_PTR);
        JsonObject_Delete(payload);
    }

    /* decode without arguments */
    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        DeviceEventOccurred decoded;

        TINY_TEST_CHECK(DeviceEventOccurred_Construct(&decoded) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutString(payload, "eid", "did-3.4.5") == TINY_RET_OK);
        TINY_TEST_CHECK(DeviceEventOccurredCodec_Decode(&decoded, payload) == TINY_RET_OK);
        TINY_TEST_CHECK(decoded.argument == NULL);
        DeviceEventOccurred_Dispose(&decoded);
        JsonObject_Delete(payload);
    }

    /* decode errors */
    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        DeviceEventOccurred decoded;

        TINY_TEST_CHECK(DeviceEventOccurred_Construct(&decoded) == TINY_RET_OK);
        TINY_TEST_CHECK(DeviceEventOccurredCodec_Decode(&decoded, payload) == TINY_RET_E_PARAM_INVALID);
        TINY_TEST_CHECK(JsonObject_PutString(payload, "eid", "no-separators") == TINY_RET_OK);
        TINY_TEST_CHECK(DeviceEventOccurredCodec_Decode(&decoded, payload) == TINY_RET_E_PARAM_INVALID);
        DeviceEventOccurred_Dispose(&decoded);
        JsonObject_Delete(payload);
    }
}

static void test_node_info(void)
{
    Node node;
    JsonObject *payload = NULL;

    TINY_TEST_CHECK(Node_Construct(NULL) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(Node_Construct(&node) == TINY_RET_OK);
    TINY_TEST_CHECK(node.serialNumber[0] == '\0');
    TINY_TEST_CHECK(node.productId == 0);
    TINY_TEST_CHECK(node.productVersion == 0);

    strncpy(node.serialNumber, "SN-001", NODE_SERIAL_NUMBER_LENGTH);
    node.productId = 300;
    node.productVersion = 2;

    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(NodeInfoCodec_Encode(payload, &node) == TINY_RET_OK);
        TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(payload, "serialNumber"), "SN-001") == 0);

        JsonNumber *productId = JsonObject_GetNumber(payload, "productId");
        TINY_TEST_CHECK(productId != NULL);
        if (productId != NULL)
        {
            TINY_TEST_CHECK(productId->value.intValue == 300);
        }

        TINY_TEST_CHECK(NodeInfoCodec_Encode(NULL, &node) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(NodeInfoCodec_Encode(payload, NULL) == TINY_RET_E_NULL_PTR);
        JsonObject_Delete(payload);
    }

    /* decoding */
    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        Node decoded;

        TINY_TEST_CHECK(Node_Construct(&decoded) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutString(payload, "serialNumber", "SN-002") == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutInteger(payload, "productId", 301) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutInteger(payload, "productVersion", 3) == TINY_RET_OK);

        TINY_TEST_CHECK(NodeInfoCodec_Decode(&decoded, payload) == TINY_RET_OK);
        TINY_TEST_CHECK(strcmp(decoded.serialNumber, "SN-002") == 0);
        TINY_TEST_CHECK(decoded.productId == 301);
        TINY_TEST_CHECK(decoded.productVersion == 3);

        TINY_TEST_CHECK(NodeInfoCodec_Decode(NULL, payload) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(NodeInfoCodec_Decode(&decoded, NULL) == TINY_RET_E_NULL_PTR);
        JsonObject_Delete(payload);
    }

    /* missing serialNumber */
    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(JsonObject_PutInteger(payload, "productId", 301) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutInteger(payload, "productVersion", 3) == TINY_RET_OK);
        TINY_TEST_CHECK(NodeInfoCodec_Decode(&node, payload) == TINY_RET_E_PARAM_INVALID);
        JsonObject_Delete(payload);
    }

    /* missing productVersion */
    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(JsonObject_PutString(payload, "serialNumber", "SN-002") == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutInteger(payload, "productId", 301) == TINY_RET_OK);
        TINY_TEST_CHECK(NodeInfoCodec_Decode(&node, payload) == TINY_RET_E_PARAM_INVALID);
        JsonObject_Delete(payload);
    }

    /* productId must be an integer */
    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(JsonObject_PutString(payload, "serialNumber", "SN-002") == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutFloat(payload, "productId", 3.5) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutInteger(payload, "productVersion", 3) == TINY_RET_OK);
        TINY_TEST_CHECK(NodeInfoCodec_Decode(&node, payload) == TINY_RET_E_PARAM_INVALID);
        JsonObject_Delete(payload);
    }

    /* productVersion must be an integer */
    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(JsonObject_PutString(payload, "serialNumber", "SN-002") == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutInteger(payload, "productId", 301) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonObject_PutFloat(payload, "productVersion", 1.5) == TINY_RET_OK);
        TINY_TEST_CHECK(NodeInfoCodec_Decode(&node, payload) == TINY_RET_E_PARAM_INVALID);
        JsonObject_Delete(payload);
    }

    Node *copy = Node_New();
    TINY_TEST_CHECK(copy != NULL);
    if (copy != NULL)
    {
        TINY_TEST_CHECK(Node_Copy(copy, &node) == TINY_RET_OK);
        TINY_TEST_CHECK(strcmp(copy->serialNumber, node.serialNumber) == 0);
        TINY_TEST_CHECK(copy->productId == node.productId);
        TINY_TEST_CHECK(Node_Copy(copy, copy) == TINY_RET_E_PARAM_INVALID);
        Node_Delete(copy);
    }

    Node_Dispose(&node);
}

static void test_notice_codecs(void)
{
    DeviceNotice device;
    NodeNotice nodeNotice;
    JsonObject *payload = NULL;
    Node *node = NULL;

    /* unsupported device notice types */
    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(DeviceNoticeCodec_Encode(payload, DEVICE_NOTICE_UNDEFINED, &device) == TINY_RET_E_PARAM_INVALID);
        TINY_TEST_CHECK(DeviceNoticeCodec_Encode(payload, 99, &device) == TINY_RET_E_PARAM_INVALID);
        TINY_TEST_CHECK(DeviceNoticeCodec_Decode(&device, DEVICE_NOTICE_UNDEFINED, payload) == TINY_RET_E_PARAM_INVALID);
        TINY_TEST_CHECK(DeviceNoticeCodec_Decode(&device, 99, payload) == TINY_RET_E_PARAM_INVALID);
        TINY_TEST_CHECK(DeviceNoticeCodec_Encode(NULL, DEVICE_NOTICE_PROPERTY_CHANGED, &device) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(DeviceNoticeCodec_Encode(payload, DEVICE_NOTICE_PROPERTY_CHANGED, NULL) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(DeviceNoticeCodec_Decode(NULL, DEVICE_NOTICE_PROPERTY_CHANGED, payload) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(DeviceNoticeCodec_Decode(&device, DEVICE_NOTICE_PROPERTY_CHANGED, NULL) == TINY_RET_E_NULL_PTR);
        JsonObject_Delete(payload);
    }

    /* device notice: property changed */
    memset(&device, 0, sizeof(device));
    device.type = DEVICE_NOTICE_PROPERTY_CHANGED;
    TINY_TEST_CHECK(DevicePropertyChanged_Construct(&device.payload.propertyChanged) == TINY_RET_OK);
    strncpy(device.payload.propertyChanged.pid.did, "did-1", IOT_DEVICE_ID_LENGTH);
    device.payload.propertyChanged.pid.siid = 2;
    device.payload.propertyChanged.pid.iid = 3;
    device.payload.propertyChanged.value = JsonValue_NewBoolean(true);

    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(DeviceNoticeCodec_Encode(payload, DEVICE_NOTICE_PROPERTY_CHANGED, &device) == TINY_RET_OK);
        TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(payload, "pid"), "did-1.2.3") == 0);
        TINY_TEST_CHECK(JsonObject_ContainsKey(payload, "value"));
        JsonObject_Delete(payload);
    }
    DevicePropertyChanged_Dispose(&device.payload.propertyChanged);

    /* device notice: event occurred */
    memset(&device, 0, sizeof(device));
    device.type = DEVICE_NOTICE_EVENT_OCCURRED;
    TINY_TEST_CHECK(DeviceEventOccurred_Construct(&device.payload.eventOccurred) == TINY_RET_OK);
    strncpy(device.payload.eventOccurred.eid.did, "did-1", IOT_DEVICE_ID_LENGTH);
    device.payload.eventOccurred.eid.siid = 2;
    device.payload.eventOccurred.eid.iid = 4;

    payload = JsonObject_New();
    TINY_TEST_CHECK(payload != NULL);
    if (payload != NULL)
    {
        TINY_TEST_CHECK(DeviceNoticeCodec_Encode(payload, DEVICE_NOTICE_EVENT_OCCURRED, &device) == TINY_RET_OK);
        TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(payload, "eid"), "did-1.2.4") == 0);

        DeviceNotice decoded;

        memset(&decoded, 0, sizeof(decoded));
        TINY_TEST_CHECK(DeviceNoticeCodec_Decode(&decoded, DEVICE_NOTICE_EVENT_OCCURRED, payload) == TINY_RET_OK);
        TINY_TEST_CHECK(decoded.type == DEVICE_NOTICE_EVENT_OCCURRED ||
                        decoded.payload.eventOccurred.eid.iid == 4);
        TINY_TEST_CHECK(strcmp(decoded.payload.eventOccurred.eid.did, "did-1") == 0);
        TINY_TEST_CHECK(decoded.payload.eventOccurred.eid.siid == 2);
        TINY_TEST_CHECK(decoded.payload.eventOccurred.eid.iid == 4);
        DeviceEventOccurred_Dispose(&decoded.payload.eventOccurred);

        JsonObject_Delete(payload);
    }
    DeviceEventOccurred_Dispose(&device.payload.eventOccurred);

    /* node notices */
    payload = NULL;

    node = Node_New();
    TINY_TEST_CHECK(node != NULL);
    if (node != NULL)
    {
        strncpy(node->serialNumber, "SN-003", NODE_SERIAL_NUMBER_LENGTH);
        node->productId = 400;
        node->productVersion = 1;

        memset(&nodeNotice, 0, sizeof(nodeNotice));
        nodeNotice.type = NODE_NOTICE_NODE_FOUND;
        TINY_TEST_CHECK(Node_Copy(&nodeNotice.payload.node, node) == TINY_RET_OK);

        TINY_TEST_CHECK(NodeNoticeCodec_Encode(NULL, NODE_NOTICE_NODE_FOUND, &nodeNotice) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(NodeNoticeCodec_Encode(NULL, NODE_NOTICE_UNDEFINED, &nodeNotice) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(NodeNoticeCodec_Decode(NULL, NODE_NOTICE_NODE_FOUND, payload) == TINY_RET_E_NULL_PTR);
        TINY_TEST_CHECK(NodeNoticeCodec_Decode(&nodeNotice, 99, NULL) == TINY_RET_E_NULL_PTR);

        payload = JsonObject_New();
        TINY_TEST_CHECK(payload != NULL);
        if (payload != NULL)
        {
            TINY_TEST_CHECK(NodeNoticeCodec_Encode(payload, NODE_NOTICE_NODE_LOST, &nodeNotice) == TINY_RET_OK);
            TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(payload, "serialNumber"), "SN-003") == 0);

            NodeNotice decoded;

            memset(&decoded, 0, sizeof(decoded));
            TINY_TEST_CHECK(NodeNoticeCodec_Decode(&decoded, NODE_NOTICE_NODE_FOUND, payload) == TINY_RET_OK);
            TINY_TEST_CHECK(strcmp(decoded.payload.node.serialNumber, "SN-003") == 0);
            TINY_TEST_CHECK(decoded.payload.node.productId == 400);
            TINY_TEST_CHECK(decoded.payload.node.productVersion == 1);

            JsonObject_Delete(payload);
        }

        Node_Delete(node);
    }
}

static void test_message_stanzas(void)
{
    Stanza *stanza = NULL;
    Stanza *decoded = NULL;
    JsonObject *document = NULL;

    /* device message: property changed */
    stanza = Stanza_New(STANZA_MESSAGE);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza == NULL)
    {
        return;
    }

    TINY_TEST_CHECK(stanza->type == STANZA_MESSAGE);

    strncpy(stanza->data.message.id, "m-1", MESSAGE_ID_LENGTH);
    stanza->data.message.topic = MESSAGE_TOPIC_DEVICE;
    stanza->data.message.type = DEVICE_NOTICE_PROPERTY_CHANGED;
    {
        DeviceNotice *notice = &stanza->data.message.payload.data.device;

        memset(notice, 0, sizeof(*notice));
        notice->type = DEVICE_NOTICE_PROPERTY_CHANGED;
        TINY_TEST_CHECK(DevicePropertyChanged_Construct(&notice->payload.propertyChanged) == TINY_RET_OK);
        strncpy(notice->payload.propertyChanged.pid.did, "did-1", IOT_DEVICE_ID_LENGTH);
        notice->payload.propertyChanged.pid.siid = 2;
        notice->payload.propertyChanged.pid.iid = 3;
        notice->payload.propertyChanged.value = JsonValue_NewInteger(11);
    }

    document = StanzaCodec_Encode(stanza);
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        JsonObject *object = JsonObject_GetObject(document, "message");
        TINY_TEST_CHECK(object != NULL);
        if (object != NULL)
        {
            TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(object, "id"), "m-1") == 0);
            TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(object, "topic"), "urn:xiot:device") == 0);
            TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(object, "type"), "property-changed") == 0);

            JsonObject *payload = JsonObject_GetObject(object, "payload");
            TINY_TEST_CHECK(payload != NULL);
            if (payload != NULL)
            {
                TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(payload, "pid"), "did-1.2.3") == 0);
            }
        }

        decoded = StanzaCodec_Decode(document);
        TINY_TEST_CHECK(decoded != NULL);
        if (decoded != NULL)
        {
            Message *message = &decoded->data.message;

            TINY_TEST_CHECK(decoded->type == STANZA_MESSAGE);
            TINY_TEST_CHECK(strcmp(message->id, "m-1") == 0);
            TINY_TEST_CHECK(message->topic == MESSAGE_TOPIC_DEVICE);
            TINY_TEST_CHECK(message->type == DEVICE_NOTICE_PROPERTY_CHANGED);
            TINY_TEST_CHECK(strcmp(message->payload.data.device.payload.propertyChanged.pid.did, "did-1") == 0);
            TINY_TEST_CHECK(message->payload.data.device.payload.propertyChanged.pid.iid == 3);

            JsonValue *value = message->payload.data.device.payload.propertyChanged.value;
            TINY_TEST_CHECK(value != NULL);
            if (value != NULL)
            {
                TINY_TEST_CHECK(value->type == JSON_NUMBER);
                TINY_TEST_CHECK(value->data.number->value.intValue == 11);
            }

            Stanza_Delete(decoded);
        }

        JsonObject_Delete(document);
    }

    Stanza_Delete(stanza);

    /* device message: event occurred, without arguments */
    stanza = Stanza_New(STANZA_MESSAGE);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza == NULL)
    {
        return;
    }

    strncpy(stanza->data.message.id, "m-2", MESSAGE_ID_LENGTH);
    stanza->data.message.topic = MESSAGE_TOPIC_DEVICE;
    stanza->data.message.type = DEVICE_NOTICE_EVENT_OCCURRED;
    {
        DeviceEventOccurred *occurred = &stanza->data.message.payload.data.device.payload.eventOccurred;

        memset(&stanza->data.message.payload, 0, sizeof(stanza->data.message.payload));
        TINY_TEST_CHECK(DeviceEventOccurred_Construct(occurred) == TINY_RET_OK);
        strncpy(occurred->eid.did, "did-1", IOT_DEVICE_ID_LENGTH);
        occurred->eid.siid = 2;
        occurred->eid.iid = 9;
    }

    document = StanzaCodec_Encode(stanza);
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        decoded = StanzaCodec_Decode(document);
        TINY_TEST_CHECK(decoded != NULL);
        if (decoded != NULL)
        {
            TINY_TEST_CHECK(decoded->data.message.type == DEVICE_NOTICE_EVENT_OCCURRED);
            TINY_TEST_CHECK(strcmp(decoded->data.message.payload.data.device.payload.eventOccurred.eid.did, "did-1") == 0);
            TINY_TEST_CHECK(decoded->data.message.payload.data.device.payload.eventOccurred.eid.iid == 9);
            Stanza_Delete(decoded);
        }
        JsonObject_Delete(document);
    }
    Stanza_Delete(stanza);

    /* node message, node lost */
    stanza = Stanza_New(STANZA_MESSAGE);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza == NULL)
    {
        return;
    }

    strncpy(stanza->data.message.id, "m-3", MESSAGE_ID_LENGTH);
    stanza->data.message.topic = MESSAGE_TOPIC_NODE;
    stanza->data.message.type = NODE_NOTICE_NODE_LOST;
    strncpy(stanza->data.message.payload.data.node.payload.node.serialNumber, "SN-006", NODE_SERIAL_NUMBER_LENGTH);
    stanza->data.message.payload.data.node.payload.node.productId = 500;
    stanza->data.message.payload.data.node.payload.node.productVersion = 4;

    document = StanzaCodec_Encode(stanza);
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        JsonObject *object = JsonObject_GetObject(document, "message");
        TINY_TEST_CHECK(object != NULL);
        if (object != NULL)
        {
            TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(object, "type"), "node-lost") == 0);
        }

        decoded = StanzaCodec_Decode(document);
        TINY_TEST_CHECK(decoded != NULL);
        if (decoded != NULL)
        {
            TINY_TEST_CHECK(decoded->type == STANZA_MESSAGE);
            TINY_TEST_CHECK(decoded->data.message.topic == MESSAGE_TOPIC_NODE);
            TINY_TEST_CHECK(decoded->data.message.type == NODE_NOTICE_NODE_LOST);
            TINY_TEST_CHECK(strcmp(decoded->data.message.payload.data.node.payload.node.serialNumber, "SN-006") == 0);
            TINY_TEST_CHECK(decoded->data.message.payload.data.node.payload.node.productId == 500);
            TINY_TEST_CHECK(decoded->data.message.payload.data.node.payload.node.productVersion == 4);
            Stanza_Delete(decoded);
        }
        JsonObject_Delete(document);
    }
    Stanza_Delete(stanza);

    /* an undefined topic can not be encoded */
    stanza = Stanza_New(STANZA_MESSAGE);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        strncpy(stanza->data.message.id, "m-4", MESSAGE_ID_LENGTH);
        stanza->data.message.topic = MESSAGE_TOPIC_UNDEFINED;
        TINY_TEST_CHECK(StanzaCodec_Encode(stanza) == NULL);
        Stanza_Delete(stanza);
    }

    /* decoding rejects incomplete message documents */
    document = JsonObject_New();
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        JsonObject *message = JsonObject_New();
        TINY_TEST_CHECK(message != NULL);
        if (message != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutString(message, "id", "m-5") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutString(message, "topic", "urn:xiot:device") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutString(message, "type", "property-changed") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutObject(document, "message", message) == TINY_RET_OK);
        }

        TINY_TEST_CHECK(StanzaCodec_Decode(document) == NULL);
        JsonObject_Delete(document);
    }

    document = JsonObject_New();
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        JsonObject *message = JsonObject_New();
        TINY_TEST_CHECK(message != NULL);
        if (message != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutString(message, "id", "m-6") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutString(message, "topic", "urn:xiot:unknown") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutString(message, "type", "property-changed") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutObject(message, "payload", JsonObject_New()) == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutObject(document, "message", message) == TINY_RET_OK);
        }

        TINY_TEST_CHECK(StanzaCodec_Decode(document) == NULL);
        JsonObject_Delete(document);
    }

    TINY_TEST_CHECK(StanzaCodec_Decode(NULL) == NULL);
}

int main(void)
{
    TINY_TEST_BEGIN("notice");

    test_type_strings();
    test_property_changed();
    test_event_occurred();
    test_node_info();
    test_notice_codecs();
    test_message_stanzas();

    return TINY_TEST_FINISH();
}
