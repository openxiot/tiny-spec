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
#include <tinycore/json_boolean.h>
#include <tinycore/json_number.h>
#include <tinycore/json_object.h>
#include <tinycore/json_value.h>
#include <tinyspec/tinyspec.h>

static void test_method_strings(void)
{
    static const struct
    {
        IQMethod method;
        const char *string;
    } mappings[] =
            {
                    {IQ_METHOD_HELLO, "urn:xiot:hello"},
                    {IQ_METHOD_BYE, "urn:xiot:bye-bye"},
                    {IQ_METHOD_PING, "urn:xiot:ping"},
                    {IQ_METHOD_GET_PROPERTIES, "urn:xiot:get-properties"},
                    {IQ_METHOD_SET_PROPERTIES, "urn:xiot:set-properties"},
                    {IQ_METHOD_GET_PROPERTY, "urn:xiot:get-property"},
                    {IQ_METHOD_SET_PROPERTY, "urn:xiot:set-property"},
                    {IQ_METHOD_INVOKE_ACTION, "urn:xiot:invoke-action"},
                    {IQ_METHOD_GET_ACCESS_KEY, "urn:xiot:get-access-key"},
                    {IQ_METHOD_SET_ACCESS_KEY, "urn:xiot:set-access-key"},
                    {IQ_METHOD_INITIALIZE, "urn:xiot:initialize"},
                    {IQ_METHOD_VERIFY_START, "urn:xiot:verify-start"},
                    {IQ_METHOD_VERIFY_FINISH, "urn:xiot:verify-finish"}
            };

    for (size_t i = 0; i < DIM(mappings); ++i)
    {
        TINY_TEST_CHECK(strcmp(IQMethod_ToString(mappings[i].method), mappings[i].string) == 0);
        TINY_TEST_CHECK(IQMethod_FromString(mappings[i].string) == mappings[i].method);
    }

    TINY_TEST_CHECK(IQMethod_ToString(IQ_METHOD_UNDEFINED) == NULL);
    TINY_TEST_CHECK(IQMethod_FromString("urn:xiot:unknown") == IQ_METHOD_UNDEFINED);
}

static void test_iq_lifecycle(void)
{
    IQ iq;

    memset(&iq, 0, sizeof(iq));

    TINY_TEST_CHECK(IQ_InitializeQuery(NULL, "id", IQ_METHOD_HELLO) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(IQ_InitializeQuery(&iq, NULL, IQ_METHOD_HELLO) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(IQ_InitializeResult(NULL, "id", IQ_METHOD_HELLO) == TINY_RET_E_NULL_PTR);
    TINY_TEST_CHECK(IQ_InitializeResult(&iq, NULL, IQ_METHOD_HELLO) == TINY_RET_E_NULL_PTR);

    TINY_TEST_CHECK(IQ_InitializeQuery(&iq, "id-1", IQ_METHOD_HELLO) == TINY_RET_OK);
    TINY_TEST_CHECK(iq.type == IQ_TYPE_QUERY);
    TINY_TEST_CHECK(strcmp(iq.id, "id-1") == 0);
    TINY_TEST_CHECK(iq.content.query.method == IQ_METHOD_HELLO);
    IQ_Dispose(&iq);

    TINY_TEST_CHECK(IQ_InitializeResult(&iq, "id-2", IQ_METHOD_HELLO) == TINY_RET_OK);
    TINY_TEST_CHECK(iq.type == IQ_TYPE_RESULT);
    TINY_TEST_CHECK(iq.content.result.method == IQ_METHOD_HELLO);
    IQ_Dispose(&iq);

    /* every method supported by the query side */
    {
        static const IQMethod queries[] =
                {
                        IQ_METHOD_HELLO, IQ_METHOD_BYE, IQ_METHOD_PING,
                        IQ_METHOD_GET_PROPERTIES, IQ_METHOD_SET_PROPERTIES,
                        IQ_METHOD_GET_PROPERTY, IQ_METHOD_SET_PROPERTY,
                        IQ_METHOD_INVOKE_ACTION,
                        IQ_METHOD_GET_ACCESS_KEY, IQ_METHOD_SET_ACCESS_KEY,
                        IQ_METHOD_INITIALIZE, IQ_METHOD_VERIFY_START, IQ_METHOD_VERIFY_FINISH
                };

        for (size_t i = 0; i < DIM(queries); ++i)
        {
            memset(&iq, 0, sizeof(iq));
            TINY_TEST_CHECK(IQ_InitializeQuery(&iq, "id", queries[i]) == TINY_RET_OK);
            IQ_Dispose(&iq);
        }
    }

    /* every method supported by the result side */
    {
        static const IQMethod results[] =
                {
                        IQ_METHOD_HELLO, IQ_METHOD_BYE, IQ_METHOD_PING,
                        IQ_METHOD_GET_PROPERTIES, IQ_METHOD_SET_PROPERTIES,
                        IQ_METHOD_GET_PROPERTY, IQ_METHOD_SET_PROPERTY,
                        IQ_METHOD_INVOKE_ACTION,
                        IQ_METHOD_GET_ACCESS_KEY, IQ_METHOD_SET_ACCESS_KEY,
                        IQ_METHOD_INITIALIZE, IQ_METHOD_VERIFY_START, IQ_METHOD_VERIFY_FINISH
                };

        for (size_t i = 0; i < DIM(results); ++i)
        {
            memset(&iq, 0, sizeof(iq));
            TINY_TEST_CHECK(IQ_InitializeResult(&iq, "id", results[i]) == TINY_RET_OK);
            IQ_Dispose(&iq);
        }
    }

    /* unsupported method */
    memset(&iq, 0, sizeof(iq));
    TINY_TEST_CHECK(IQ_InitializeQuery(&iq, "id", IQ_METHOD_UNDEFINED) == TINY_RET_E_PARAM_INVALID);
    memset(&iq, 0, sizeof(iq));
    TINY_TEST_CHECK(IQ_InitializeResult(&iq, "id", IQ_METHOD_UNDEFINED) == TINY_RET_E_PARAM_INVALID);

    /* disposing a stanza that was never initialized is a no-op */
    memset(&iq, 0, sizeof(iq));
    IQ_Dispose(&iq);
    TINY_TEST_CHECK(iq.type == IQ_TYPE_UNDEFINED);
}

static void test_basic_factories(void)
{
    Stanza *stanza = NULL;
    JsonObject *document = NULL;

    struct
    {
        Stanza *(*factory)(const char *);
        IQType type;
        IQMethod method;
    } cases[] =
            {
                    {QueryHello_New, IQ_TYPE_QUERY, IQ_METHOD_HELLO},
                    {ResultHello_New, IQ_TYPE_RESULT, IQ_METHOD_HELLO},
                    {QueryBye_New, IQ_TYPE_QUERY, IQ_METHOD_BYE},
                    {ResultBye_New, IQ_TYPE_RESULT, IQ_METHOD_BYE},
                    {Ping_New, IQ_TYPE_QUERY, IQ_METHOD_PING},
                    {Pong_New, IQ_TYPE_RESULT, IQ_METHOD_PING}
            };

    for (size_t i = 0; i < DIM(cases); ++i)
    {
        stanza = cases[i].factory("id-basic");
        TINY_TEST_CHECK(stanza != NULL);
        if (stanza == NULL)
        {
            continue;
        }

        TINY_TEST_CHECK(stanza->type == STANZA_IQ);
        TINY_TEST_CHECK(stanza->data.iq.type == cases[i].type);
        TINY_TEST_CHECK(strcmp(stanza->data.iq.id, "id-basic") == 0);
        if (cases[i].type == IQ_TYPE_QUERY)
        {
            TINY_TEST_CHECK(stanza->data.iq.content.query.method == cases[i].method);
        }
        else
        {
            TINY_TEST_CHECK(stanza->data.iq.content.result.method == cases[i].method);
        }

        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            JsonObject *iq = JsonObject_GetObject(document, "iq");
            TINY_TEST_CHECK(iq != NULL);
            if (iq != NULL)
            {
                TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(iq, "id"), "id-basic") == 0);
                TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(iq, "type"),
                                       cases[i].type == IQ_TYPE_QUERY ? "query" : "result") == 0);
                TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(iq, "method"),
                                       IQMethod_ToString(cases[i].method)) == 0);
            }

            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                TINY_TEST_CHECK(decoded->type == STANZA_IQ);
                TINY_TEST_CHECK(decoded->data.iq.type == cases[i].type);
                TINY_TEST_CHECK(strcmp(decoded->data.iq.id, "id-basic") == 0);
                if (cases[i].type == IQ_TYPE_QUERY)
                {
                    TINY_TEST_CHECK(decoded->data.iq.content.query.method == cases[i].method);
                }
                else
                {
                    TINY_TEST_CHECK(decoded->data.iq.content.result.method == cases[i].method);
                }
                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }

    /* a NULL id is rejected */
    TINY_TEST_CHECK(QueryHello_New(NULL) == NULL);
    TINY_TEST_CHECK(ResultHello_New(NULL) == NULL);
    TINY_TEST_CHECK(Ping_New(NULL) == NULL);
    TINY_TEST_CHECK(Pong_New(NULL) == NULL);
}

static void test_error_stanza(void)
{
    Stanza *stanza = IQError_New("id-error", IOT_STATUS_PROPERTY_NOT_FOUND, "property not found");

    TINY_TEST_CHECK(stanza != NULL);
    if (stanza == NULL)
    {
        return;
    }

    TINY_TEST_CHECK(stanza->type == STANZA_IQ);
    TINY_TEST_CHECK(stanza->data.iq.type == IQ_TYPE_ERROR);
    TINY_TEST_CHECK(strcmp(stanza->data.iq.id, "id-error") == 0);
    TINY_TEST_CHECK(stanza->data.iq.content.error.status == IOT_STATUS_PROPERTY_NOT_FOUND);
    TINY_TEST_CHECK(strcmp(stanza->data.iq.content.error.description, "property not found") == 0);

    JsonObject *document = StanzaCodec_Encode(stanza);
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        JsonObject *iq = JsonObject_GetObject(document, "iq");
        TINY_TEST_CHECK(iq != NULL);
        if (iq != NULL)
        {
            TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(iq, "type"), "error") == 0);

            JsonObject *content = JsonObject_GetObject(iq, "content");
            TINY_TEST_CHECK(content != NULL);
            if (content != NULL)
            {
                JsonNumber *status = JsonObject_GetNumber(content, "status");
                TINY_TEST_CHECK(status != NULL);
                if (status != NULL)
                {
                    TINY_TEST_CHECK(status->type == JSON_NUMBER_INTEGER);
                    TINY_TEST_CHECK(status->value.intValue == IOT_STATUS_PROPERTY_NOT_FOUND);
                }
                TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(content, "description"), "property not found") == 0);
            }
        }

        Stanza *decoded = StanzaCodec_Decode(document);
        TINY_TEST_CHECK(decoded != NULL);
        if (decoded != NULL)
        {
            TINY_TEST_CHECK(decoded->data.iq.type == IQ_TYPE_ERROR);
            TINY_TEST_CHECK(strcmp(decoded->data.iq.id, "id-error") == 0);
            TINY_TEST_CHECK(decoded->data.iq.content.error.status == IOT_STATUS_PROPERTY_NOT_FOUND);
            TINY_TEST_CHECK(strcmp(decoded->data.iq.content.error.description, "property not found") == 0);
            Stanza_Delete(decoded);
        }

        JsonObject_Delete(document);
    }

    Stanza_Delete(stanza);
}

static void test_key_stanzas(void)
{
    Stanza *stanza = NULL;
    JsonObject *document = NULL;

    /* query: set-access-key */
    stanza = QuerySetAccessKey_New("id-key", "abcdef");
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(stanza->data.iq.type == IQ_TYPE_QUERY);
        TINY_TEST_CHECK(stanza->data.iq.content.query.method == IQ_METHOD_SET_ACCESS_KEY);
        TINY_TEST_CHECK(strcmp(stanza->data.iq.content.query.content.setAccessKey.key, "abcdef") == 0);

        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            JsonObject *iq = JsonObject_GetObject(document, "iq");
            TINY_TEST_CHECK(iq != NULL);
            if (iq != NULL)
            {
                JsonObject *content = JsonObject_GetObject(iq, "content");
                TINY_TEST_CHECK(content != NULL);
                if (content != NULL)
                {
                    TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(content, "key"), "abcdef") == 0);
                }
            }

            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                TINY_TEST_CHECK(decoded->data.iq.content.query.method == IQ_METHOD_SET_ACCESS_KEY);
                TINY_TEST_CHECK(strcmp(decoded->data.iq.content.query.content.setAccessKey.key, "abcdef") == 0);
                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }

    /* query: get-access-key has no content */
    stanza = QueryGetAccessKey_New("id-key-2");
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            JsonObject *iq = JsonObject_GetObject(document, "iq");
            if (iq != NULL)
            {
                TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(iq, "method"), "urn:xiot:get-access-key") == 0);
                TINY_TEST_CHECK(!JsonObject_ContainsKey(iq, "content"));
            }

            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                TINY_TEST_CHECK(decoded->data.iq.content.query.method == IQ_METHOD_GET_ACCESS_KEY);
                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }

    /* result: get-access-key */
    stanza = Stanza_New(STANZA_IQ);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(IQ_InitializeResult(&stanza->data.iq, "id-key-3", IQ_METHOD_GET_ACCESS_KEY) == TINY_RET_OK);
        strncpy(stanza->data.iq.content.result.content.getAccessKey.key, "key-value", STANZA_ACCESS_KEY_LEN);

        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                TINY_TEST_CHECK(decoded->data.iq.type == IQ_TYPE_RESULT);
                TINY_TEST_CHECK(decoded->data.iq.content.result.method == IQ_METHOD_GET_ACCESS_KEY);
                TINY_TEST_CHECK(strcmp(decoded->data.iq.content.result.content.getAccessKey.key, "key-value") == 0);
                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }
}

static void test_verify_stanzas(void)
{
    Stanza *stanza = NULL;
    JsonObject *document = NULL;

    /* initialize */
    stanza = QueryInitialize_New("id-init", "1.0", "auth-token");
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(stanza->data.iq.content.query.method == IQ_METHOD_INITIALIZE);
        TINY_TEST_CHECK(strcmp(stanza->data.iq.content.query.content.initialize.version, "1.0") == 0);
        TINY_TEST_CHECK(strcmp(stanza->data.iq.content.query.content.initialize.authentication, "auth-token") == 0);

        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                TINY_TEST_CHECK(decoded->data.iq.content.query.method == IQ_METHOD_INITIALIZE);
                TINY_TEST_CHECK(strcmp(decoded->data.iq.content.query.content.initialize.version, "1.0") == 0);
                TINY_TEST_CHECK(strcmp(decoded->data.iq.content.query.content.initialize.authentication, "auth-token") == 0);
                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }

    /* verify-start query */
    stanza = QueryVerifyStart_New("id-vs", "public-key-data");
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            JsonObject *iq = JsonObject_GetObject(document, "iq");
            if (iq != NULL)
            {
                JsonObject *content = JsonObject_GetObject(iq, "content");
                if (content != NULL)
                {
                    TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(content, "public-key"), "public-key-data") == 0);
                }
            }

            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                TINY_TEST_CHECK(strcmp(decoded->data.iq.content.query.content.verifyStart.publicKey, "public-key-data") == 0);
                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }

    /* verify-start result */
    stanza = Stanza_New(STANZA_IQ);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(IQ_InitializeResult(&stanza->data.iq, "id-vs-r", IQ_METHOD_VERIFY_START) == TINY_RET_OK);
        strncpy(stanza->data.iq.content.result.content.verifyStart.publicKey, "pk", STANZA_PUBLIC_KEY_LENGTH);
        strncpy(stanza->data.iq.content.result.content.verifyStart.signature, "sig", STANZA_SIGNATURE_LENGTH);

        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                TINY_TEST_CHECK(strcmp(decoded->data.iq.content.result.content.verifyStart.publicKey, "pk") == 0);
                TINY_TEST_CHECK(strcmp(decoded->data.iq.content.result.content.verifyStart.signature, "sig") == 0);
                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }

    /* verify-finish */
    stanza = QueryVerifyFinish_New("id-vf", "did-100", "device-type", "the-signature", 1);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        QueryVerifyFinish *finish = &stanza->data.iq.content.query.content.verifyFinish;
        TINY_TEST_CHECK(strcmp(finish->did, "did-100") == 0);
        TINY_TEST_CHECK(strcmp(finish->type, "device-type") == 0);
        TINY_TEST_CHECK(strcmp(finish->signature, "the-signature") == 0);
        TINY_TEST_CHECK(finish->codec == 1);

        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                QueryVerifyFinish *value = &decoded->data.iq.content.query.content.verifyFinish;
                TINY_TEST_CHECK(strcmp(value->did, "did-100") == 0);
                TINY_TEST_CHECK(strcmp(value->type, "device-type") == 0);
                TINY_TEST_CHECK(strcmp(value->signature, "the-signature") == 0);
                TINY_TEST_CHECK(value->codec == 1);
                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }
}

static void test_control_queries(void)
{
    Stanza *stanza = NULL;
    JsonObject *document = NULL;

    /* get-properties query carrying two property ids */
    stanza = Stanza_New(STANZA_IQ);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza == NULL)
    {
        return;
    }

    TINY_TEST_CHECK(IQ_InitializeQuery(&stanza->data.iq, "id-gp", IQ_METHOD_GET_PROPERTIES) == TINY_RET_OK);
    {
        PropertyOperations *operations = &stanza->data.iq.content.query.content.getProperties.properties;

        PropertyOperation *first = PropertyOperation_NewFrom("did-1", 2, 1);
        TINY_TEST_CHECK(first != NULL);
        if (first != NULL)
        {
            TINY_TEST_CHECK(TinyList_AddTail(&operations->properties, first) != NULL);
        }

        PropertyOperation *second = PropertyOperation_NewFrom("did-1", 2, 2);
        TINY_TEST_CHECK(second != NULL);
        if (second != NULL)
        {
            TINY_TEST_CHECK(TinyList_AddTail(&operations->properties, second) != NULL);
        }

        TINY_TEST_CHECK(operations->properties.size == 2);
    }

    document = StanzaCodec_Encode(stanza);
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        Stanza *decoded = StanzaCodec_Decode(document);
        TINY_TEST_CHECK(decoded != NULL);
        if (decoded != NULL)
        {
            TINY_TEST_CHECK(decoded->data.iq.content.query.method == IQ_METHOD_GET_PROPERTIES);
            TINY_TEST_CHECK(decoded->data.iq.content.query.content.getProperties.properties.properties.size == 2);
            Stanza_Delete(decoded);
        }

        JsonObject_Delete(document);
    }

    Stanza_Delete(stanza);

    /* get-property query */
    stanza = Stanza_New(STANZA_IQ);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(IQ_InitializeQuery(&stanza->data.iq, "id-gp2", IQ_METHOD_GET_PROPERTY) == TINY_RET_OK);
        strncpy(stanza->data.iq.content.query.content.getProperty.property.pid.did, "did-2", IOT_DEVICE_ID_LENGTH);
        stanza->data.iq.content.query.content.getProperty.property.pid.siid = 3;
        stanza->data.iq.content.query.content.getProperty.property.pid.iid = 4;

        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                PropertyId *pid = &decoded->data.iq.content.query.content.getProperty.property.pid;
                TINY_TEST_CHECK(strcmp(pid->did, "did-2") == 0);
                TINY_TEST_CHECK(pid->siid == 3);
                TINY_TEST_CHECK(pid->iid == 4);
                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }

    /* set-property query */
    stanza = Stanza_New(STANZA_IQ);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(IQ_InitializeQuery(&stanza->data.iq, "id-sp", IQ_METHOD_SET_PROPERTY) == TINY_RET_OK);
        {
            PropertyOperation *operation = &stanza->data.iq.content.query.content.setProperty.property;

            strncpy(operation->pid.did, "did-3", IOT_DEVICE_ID_LENGTH);
            operation->pid.siid = 1;
            operation->pid.iid = 2;
            operation->value = JsonValue_NewString("hello");
        }

        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                PropertyOperation *operation = &decoded->data.iq.content.query.content.setProperty.property;
                TINY_TEST_CHECK(operation->pid.iid == 2);
                TINY_TEST_CHECK(operation->value != NULL);
                if (operation->value != NULL)
                {
                    TINY_TEST_CHECK(operation->value->type == JSON_STRING);
                    TINY_TEST_CHECK(strcmp(operation->value->data.string->value, "hello") == 0);
                }
                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }
}

static void test_control_results(void)
{
    Stanza *stanza = NULL;
    JsonObject *document = NULL;
    PropertyOperations operations;

    TINY_TEST_CHECK(PropertyOperations_Construct(&operations) == TINY_RET_OK);
    {
        PropertyOperation *first = PropertyOperation_NewValue("did-1", 2, 1, JsonValue_NewInteger(7));
        TINY_TEST_CHECK(first != NULL);
        if (first != NULL)
        {
            TINY_TEST_CHECK(TinyList_AddTail(&operations.properties, first) != NULL);
        }

        PropertyOperation *second = PropertyOperation_NewFrom("did-1", 2, 2);
        TINY_TEST_CHECK(second != NULL);
        if (second != NULL)
        {
            second->status = IOT_STATUS_PROPERTY_CANNOT_READ;
            strncpy(second->description, "read denied", IOT_DESCRIPTION_LENGTH);
            TINY_TEST_CHECK(TinyList_AddTail(&operations.properties, second) != NULL);
        }
    }

    /* get-properties result */
    stanza = ResultGetProperties_New("id-gpr", &operations);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(stanza->data.iq.type == IQ_TYPE_RESULT);
        TINY_TEST_CHECK(stanza->data.iq.content.result.method == IQ_METHOD_GET_PROPERTIES);
        TINY_TEST_CHECK(stanza->data.iq.content.result.content.getProperties.properties.properties.size == 2);

        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                PropertyOperations *result = &decoded->data.iq.content.result.content.getProperties.properties;

                TINY_TEST_CHECK(result->properties.size == 2);
                if (result->properties.size == 2)
                {
                    PropertyOperation *first = (PropertyOperation *) TinyList_GetAt(&result->properties, 0);
                    PropertyOperation *second = (PropertyOperation *) TinyList_GetAt(&result->properties, 1);

                    TINY_TEST_CHECK(first->status == IOT_STATUS_COMPLETED);
                    TINY_TEST_CHECK(first->value != NULL);
                    if (first->value != NULL)
                    {
                        TINY_TEST_CHECK(first->value->data.number->value.intValue == 7);
                    }
                    TINY_TEST_CHECK(second->status == IOT_STATUS_PROPERTY_CANNOT_READ);
                    TINY_TEST_CHECK(strcmp(second->description, "read denied") == 0);
                }

                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }

    /* set-properties result */
    stanza = ResultSetProperties_New("id-spr", &operations);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(stanza->data.iq.content.result.method == IQ_METHOD_SET_PROPERTIES);

        document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        if (document != NULL)
        {
            Stanza *decoded = StanzaCodec_Decode(document);
            TINY_TEST_CHECK(decoded != NULL);
            if (decoded != NULL)
            {
                TINY_TEST_CHECK(decoded->data.iq.content.result.content.setProperties.properties.properties.size == 2);
                Stanza_Delete(decoded);
            }

            JsonObject_Delete(document);
        }

        Stanza_Delete(stanza);
    }

    TINY_TEST_CHECK(ResultGetProperties_New("id", NULL) == NULL);
    TINY_TEST_CHECK(ResultSetProperties_New("id", NULL) == NULL);

    /* get-property result */
    {
        PropertyOperation operation;

        TINY_TEST_CHECK(PropertyOperation_Construct(&operation) == TINY_RET_OK);
        strncpy(operation.pid.did, "did-4", IOT_DEVICE_ID_LENGTH);
        operation.pid.siid = 5;
        operation.pid.iid = 6;
        operation.value = JsonValue_NewBoolean(false);

        stanza = ResultGetProperty_New("id-gp-r", &operation);
        TINY_TEST_CHECK(stanza != NULL);
        if (stanza != NULL)
        {
            document = StanzaCodec_Encode(stanza);
            TINY_TEST_CHECK(document != NULL);
            if (document != NULL)
            {
                Stanza *decoded = StanzaCodec_Decode(document);
                TINY_TEST_CHECK(decoded != NULL);
                if (decoded != NULL)
                {
                    PropertyOperation *value = &decoded->data.iq.content.result.content.getProperty.property;
                    TINY_TEST_CHECK(strcmp(value->pid.did, "did-4") == 0);
                    TINY_TEST_CHECK(value->status == IOT_STATUS_COMPLETED);
                    TINY_TEST_CHECK(value->value != NULL);
                    if (value->value != NULL)
                    {
                        TINY_TEST_CHECK(value->value->type == JSON_BOOLEAN);
                        TINY_TEST_CHECK(JsonBoolean_GetValue(value->value->data.boolean) == false);
                    }
                    Stanza_Delete(decoded);
                }

                JsonObject_Delete(document);
            }

            Stanza_Delete(stanza);
        }

        PropertyOperation_Dispose(&operation);
    }

    /* get-property result carrying an error status */
    {
        PropertyOperation operation;

        TINY_TEST_CHECK(PropertyOperation_Construct(&operation) == TINY_RET_OK);
        strncpy(operation.pid.did, "did-4", IOT_DEVICE_ID_LENGTH);
        operation.pid.siid = 5;
        operation.pid.iid = 7;
        operation.status = IOT_STATUS_PROPERTY_CANNOT_READ;

        stanza = ResultGetProperty_New("id-gp-e", &operation);
        TINY_TEST_CHECK(stanza != NULL);
        if (stanza != NULL)
        {
            document = StanzaCodec_Encode(stanza);
            TINY_TEST_CHECK(document != NULL);
            if (document != NULL)
            {
                Stanza *decoded = StanzaCodec_Decode(document);
                TINY_TEST_CHECK(decoded != NULL);
                if (decoded != NULL)
                {
                    PropertyOperation *value = &decoded->data.iq.content.result.content.getProperty.property;
                    TINY_TEST_CHECK(value->status == IOT_STATUS_PROPERTY_CANNOT_READ);
                    TINY_TEST_CHECK(value->value == NULL);
                    Stanza_Delete(decoded);
                }

                JsonObject_Delete(document);
            }

            Stanza_Delete(stanza);
        }

        PropertyOperation_Dispose(&operation);
    }

    /* set-property result */
    {
        PropertyOperation operation;

        TINY_TEST_CHECK(PropertyOperation_Construct(&operation) == TINY_RET_OK);
        strncpy(operation.pid.did, "did-4", IOT_DEVICE_ID_LENGTH);
        operation.pid.siid = 5;
        operation.pid.iid = 8;

        stanza = ResultSetProperty_New("id-sp-r", &operation);
        TINY_TEST_CHECK(stanza != NULL);
        if (stanza != NULL)
        {
            TINY_TEST_CHECK(stanza->data.iq.content.result.method == IQ_METHOD_SET_PROPERTY);

            document = StanzaCodec_Encode(stanza);
            TINY_TEST_CHECK(document != NULL);
            if (document != NULL)
            {
                JsonObject_Delete(document);
            }

            Stanza_Delete(stanza);
        }

        PropertyOperation_Dispose(&operation);
    }

    /* invoke-action result: only the encode side is implemented */
    {
        ActionOperation action;

        TINY_TEST_CHECK(ActionOperation_Construct(&action) == TINY_RET_OK);
        strncpy(action.aid.did, "did-5", IOT_DEVICE_ID_LENGTH);
        action.aid.siid = 6;
        action.aid.iid = 1;
        TINY_TEST_CHECK(JsonArray_AddInteger(&action.out, 3) == TINY_RET_OK);
        TINY_TEST_CHECK(JsonArray_AddString(&action.out, "done") == TINY_RET_OK);

        stanza = ResultInvokeAction_New("id-ia", &action);
        TINY_TEST_CHECK(stanza != NULL);
        if (stanza != NULL)
        {
            document = StanzaCodec_Encode(stanza);
            TINY_TEST_CHECK(document != NULL);
            if (document != NULL)
            {
                JsonObject *iq = JsonObject_GetObject(document, "iq");
                if (iq != NULL)
                {
                    TINY_TEST_CHECK(strcmp(JsonObject_GetStringValue(iq, "method"), "urn:xiot:invoke-action") == 0);
                }

                JsonObject_Delete(document);
            }

            Stanza_Delete(stanza);
        }

        ActionOperation_Dispose(&action);
    }

    TINY_TEST_CHECK(ResultGetProperty_New("id", NULL) == NULL);
    TINY_TEST_CHECK(ResultSetProperty_New("id", NULL) == NULL);
    TINY_TEST_CHECK(ResultInvokeAction_New("id", NULL) == NULL);

    PropertyOperations_Dispose(&operations);
}

static void test_encode_errors(void)
{
    Stanza *stanza = NULL;

    /* an IQ stanza without a type can not be encoded */
    stanza = Stanza_New(STANZA_IQ);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        TINY_TEST_CHECK(stanza->data.iq.type == IQ_TYPE_UNDEFINED);
        TINY_TEST_CHECK(StanzaCodec_Encode(stanza) == NULL);
        Stanza_Delete(stanza);
    }

    TINY_TEST_CHECK(StanzaCodec_Encode(NULL) == NULL);

    /* a stanza whose type is neither IQ nor message encodes to an empty document */
    stanza = Stanza_New((StanzaType) 99);
    TINY_TEST_CHECK(stanza != NULL);
    if (stanza != NULL)
    {
        JsonObject *document = StanzaCodec_Encode(stanza);
        TINY_TEST_CHECK(document != NULL);
        JsonObject_Delete(document);
        Stanza_Delete(stanza);
    }
}

static void test_decode_errors(void)
{
    JsonObject *document = NULL;

    /* the iq value is not an object */
    document = JsonObject_New();
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        TINY_TEST_CHECK(JsonObject_PutInteger(document, "iq", 1) == TINY_RET_OK);
        TINY_TEST_CHECK(StanzaCodec_Decode(document) == NULL);
        JsonObject_Delete(document);
    }

    /* no id */
    document = JsonObject_New();
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        JsonObject *iq = JsonObject_New();
        TINY_TEST_CHECK(iq != NULL);
        if (iq != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutString(iq, "type", "query") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutObject(document, "iq", iq) == TINY_RET_OK);
        }
        TINY_TEST_CHECK(StanzaCodec_Decode(document) == NULL);
        JsonObject_Delete(document);
    }

    /* no type */
    document = JsonObject_New();
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        JsonObject *iq = JsonObject_New();
        TINY_TEST_CHECK(iq != NULL);
        if (iq != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutString(iq, "id", "id-1") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutObject(document, "iq", iq) == TINY_RET_OK);
        }
        TINY_TEST_CHECK(StanzaCodec_Decode(document) == NULL);
        JsonObject_Delete(document);
    }

    /* unknown type */
    document = JsonObject_New();
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        JsonObject *iq = JsonObject_New();
        TINY_TEST_CHECK(iq != NULL);
        if (iq != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutString(iq, "id", "id-1") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutString(iq, "type", "unknown") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutObject(document, "iq", iq) == TINY_RET_OK);
        }
        TINY_TEST_CHECK(StanzaCodec_Decode(document) == NULL);
        JsonObject_Delete(document);
    }

    /* unknown method */
    document = JsonObject_New();
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        JsonObject *iq = JsonObject_New();
        TINY_TEST_CHECK(iq != NULL);
        if (iq != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutString(iq, "id", "id-1") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutString(iq, "type", "query") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutString(iq, "method", "urn:xiot:unknown") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutObject(document, "iq", iq) == TINY_RET_OK);
        }
        TINY_TEST_CHECK(StanzaCodec_Decode(document) == NULL);
        JsonObject_Delete(document);
    }

    /* no method */
    document = JsonObject_New();
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        JsonObject *iq = JsonObject_New();
        TINY_TEST_CHECK(iq != NULL);
        if (iq != NULL)
        {
            TINY_TEST_CHECK(JsonObject_PutString(iq, "id", "id-1") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutString(iq, "type", "result") == TINY_RET_OK);
            TINY_TEST_CHECK(JsonObject_PutObject(document, "iq", iq) == TINY_RET_OK);
        }
        TINY_TEST_CHECK(StanzaCodec_Decode(document) == NULL);
        JsonObject_Delete(document);
    }

    /* an empty document is not a stanza */
    document = JsonObject_New();
    TINY_TEST_CHECK(document != NULL);
    if (document != NULL)
    {
        TINY_TEST_CHECK(StanzaCodec_Decode(document) == NULL);
        JsonObject_Delete(document);
    }

    TINY_TEST_CHECK(StanzaCodec_Decode(NULL) == NULL);
}

int main(void)
{
    TINY_TEST_BEGIN("stanza");

    test_method_strings();
    test_iq_lifecycle();
    test_basic_factories();
    test_error_stanza();
    test_key_stanzas();
    test_verify_stanzas();
    test_control_queries();
    test_control_results();
    test_encode_errors();
    test_decode_errors();

    return TINY_TEST_FINISH();
}
