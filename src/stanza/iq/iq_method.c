/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/iq_method.h>

typedef struct
{
    IQMethod        method;
    const char    * string;
} IQMethod_String;

static IQMethod_String method_string_mappings[] =
        {
                {.method = IQ_METHOD_HELLO, .string = "urn:xiot:hello"},
                {.method = IQ_METHOD_BYE, .string = "urn:xiot:bye-bye"},
                {.method = IQ_METHOD_PING, .string = "urn:xiot:ping"},

                {.method = IQ_METHOD_GET_PROPERTIES, .string = "urn:xiot:get-properties"},
                {.method = IQ_METHOD_SET_PROPERTIES, .string = "urn:xiot:set-properties"},
                {.method = IQ_METHOD_GET_PROPERTY, .string = "urn:xiot:get-property"},
                {.method = IQ_METHOD_SET_PROPERTY, .string = "urn:xiot:set-property"},
                {.method = IQ_METHOD_INVOKE_ACTION, .string = "urn:xiot:invoke-action"},
                {.method = IQ_METHOD_INVOKE_ACTIONS, .string = "urn:xiot:invoke-actions"},
                {.method = IQ_METHOD_PROPERTIES_CHANGED, .string = "urn:xiot:properties-changed"},
                {.method = IQ_METHOD_GET_CHILDREN, .string = "urn:xiot:get-children"},

                {.method = IQ_METHOD_GET_ACCESS_KEY, .string = "urn:xiot:get-access-key"},
                {.method = IQ_METHOD_SET_ACCESS_KEY, .string = "urn:xiot:set-access-key"},

                {.method = IQ_METHOD_INITIALIZE, .string = "urn:xiot:initialize"},
                {.method = IQ_METHOD_VERIFY_START, .string = "urn:xiot:verify-start"},
                {.method = IQ_METHOD_VERIFY_FINISH, .string = "urn:xiot:verify-finish"},
        };

const char * IQMethod_ToString(IQMethod method)
{
    const uint32_t size = DIM(method_string_mappings);

    for (uint32_t i = 0; i < size; ++i)
    {
        if (method_string_mappings[i].method == method)
        {
            return method_string_mappings[i].string;
        }
    }

    return NULL;
}

IQMethod IQMethod_FromString(const char *string)
{
    const uint32_t size = DIM(method_string_mappings);

    for (uint32_t i = 0; i < size; ++i)
    {
        if (STR_EQUAL(method_string_mappings[i].string, string))
        {
            return method_string_mappings[i].method;
        }
    }

    return IQ_METHOD_UNDEFINED;
}
