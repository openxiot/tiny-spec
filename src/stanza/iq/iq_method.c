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

static IQMethod_String _method_string_mappings[] =
        {
                {IQ_METHOD_HELLO, "urn:xiot:hello"},
                {IQ_METHOD_BYEBYE, "urn:xiot:bye-bye"},
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
                {IQ_METHOD_VERIFY_FINISH, "urn:xiot:verify-finish"},
        };

const char * IQMethod_ToString(IQMethod method)
{
    uint32_t size = DIM(_method_string_mappings);

    for (uint32_t i = 0; i < size; ++i)
    {
        if (_method_string_mappings[i].method == method)
        {
            return _method_string_mappings[i].string;
        }
    }

    return NULL;
}

IQMethod IQMethod_FromString(const char *string)
{
    uint32_t size = DIM(_method_string_mappings);

    for (uint32_t i = 0; i < size; ++i)
    {
        if (STR_EQUAL(_method_string_mappings[i].string, string))
        {
            return _method_string_mappings[i].method;
        }
    }

    return IQ_METHOD_UNDEFINED;
}
