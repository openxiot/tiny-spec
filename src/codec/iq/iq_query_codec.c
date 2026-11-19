/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/tinyspec_base.h>
#include <tinycore/tiny_log.h>
#include <tinycore/json_object.h>
#include <tinyspec/stanza/iq/iq_query.h>
#include <tinyspec/codec/iq/iq_query_codec.h>
#include <tinyspec/codec/iq/device/control/get_properties_codec.h>
#include <tinyspec/codec/iq/device/control/set_properties_codec.h>
#include <tinyspec/codec/iq/device/control/get_property_codec.h>
#include <tinyspec/codec/iq/device/control/set_property_codec.h>
#include <tinyspec/codec/iq/device/control/invoke_action_codec.h>
#include <tinyspec/codec/iq/device/control/invoke_actions_codec.h>
#include <tinyspec/codec/iq/device/manager/get_children_codec.h>
#include <tinyspec/codec/iq/device/manager/properties_changed_codec.h>
#include <tinyspec/codec/iq/device/key/set_access_key_codec.h>
#include <tinyspec/codec/iq/device/verify/initialize_codec.h>
#include <tinyspec/codec/iq/device/verify/verify_finish_codec.h>
#include <tinyspec/codec/iq/device/verify/verify_start_codec.h>

TINYSPEC_BEGIN_DECLS

static bool addQueryContent(IQQuery *query, JsonObject *content)
{
    bool done = false;

    switch (query->method)
    {
        // basic
        case IQ_METHOD_HELLO:
        case IQ_METHOD_BYE:
        case IQ_METHOD_PING:
            break;

            // device/control
        case IQ_METHOD_GET_PROPERTIES:
            done = GetPropertiesCodec_EncodeQuery(&query->content.getProperties, content);
            break;

        case IQ_METHOD_SET_PROPERTIES:
            done = SetPropertiesCodec_EncodeQuery(&query->content.setProperties, content);
            break;

        case IQ_METHOD_GET_PROPERTY:
            done = GetPropertyCodec_EncodeQuery(&query->content.getProperty, content);
            break;

        case IQ_METHOD_SET_PROPERTY:
            done = SetPropertyCodec_EncodeQuery(&query->content.setProperty, content);
            break;

        case IQ_METHOD_INVOKE_ACTION:
            done = InvokeActionCodec_EncodeQuery(&query->content.invokeAction, content);
            break;

        case IQ_METHOD_INVOKE_ACTIONS:
            done = InvokeActionsCodec_EncodeQuery(&query->content.invokeActions, content);
            break;

        case IQ_METHOD_GET_CHILDREN:
            done = GetChildrenCodec_EncodeQuery(&query->content.getChildren, content);
            break;

        case IQ_METHOD_PROPERTIES_CHANGED:
            done = PropertiesChangedCodec_EncodeQuery(&query->content.propertiesChanged, content);
            break;

            // device/key
        case IQ_METHOD_GET_ACCESS_KEY:
            break;

        case IQ_METHOD_SET_ACCESS_KEY:
            done = SetAccessKeyCodec_EncodeQuery(&query->content.setAccessKey, content);
            break;

            // device/verify
        case IQ_METHOD_INITIALIZE:
            done = InitializeCodec_EncodeQuery(&query->content.initialize, content);
            break;

        case IQ_METHOD_VERIFY_START:
            done = VerifyStartCodec_EncodeQuery(&query->content.verifyStart, content);
            break;

        case IQ_METHOD_VERIFY_FINISH:
            done = VerifyFinishCodec_EncodeQuery(&query->content.verifyFinish, content);
            break;

        default:
            TINY_LOG_DEBUG("not support method: %s", IQMethod_ToString(query->method));
            done = false;
            break;
    }

    return done;
}

static TinyRet decodeQueryContent(IQQuery *query, JsonObject *content)
{
    TinyRet ret = TINY_RET_OK;

    switch (query->method)
    {
        // basic
        case IQ_METHOD_HELLO:
        case IQ_METHOD_BYE:
        case IQ_METHOD_PING:
            break;

            // device/control
        case IQ_METHOD_GET_PROPERTIES:
            ret = GetPropertiesCodec_DecodeQuery(content, &query->content.getProperties);
            break;

        case IQ_METHOD_SET_PROPERTIES:
            ret = SetPropertiesCodec_DecodeQuery(content, &query->content.setProperties);
            break;

        case IQ_METHOD_GET_PROPERTY:
            ret = GetPropertyCodec_DecodeQuery(content, &query->content.getProperty);
            break;

        case IQ_METHOD_SET_PROPERTY:
            ret = SetPropertyCodec_DecodeQuery(content, &query->content.setProperty);
            break;

        case IQ_METHOD_INVOKE_ACTION:
            ret = InvokeActionCodec_DecodeQuery(content, &query->content.invokeAction);
            break;

        case IQ_METHOD_INVOKE_ACTIONS:
            ret = InvokeActionsCodec_DecodeQuery(content, &query->content.invokeActions);
            break;

        case IQ_METHOD_GET_CHILDREN:
            ret = GetChildrenCodec_DecodeQuery(content, &query->content.getChildren);
            break;

        case IQ_METHOD_PROPERTIES_CHANGED:
            ret = PropertiesChangedCodec_DecodeQuery(content, &query->content.propertiesChanged);
            break;

            // device/key
        case IQ_METHOD_GET_ACCESS_KEY:
            break;

        case IQ_METHOD_SET_ACCESS_KEY:
            ret = SetAccessKeyCodec_DecodeQuery(content, &query->content.setAccessKey);
            break;

            // device/verify
        case IQ_METHOD_INITIALIZE:
            ret = InitializeCodec_DecodeQuery(content, &query->content.initialize);
            break;

        case IQ_METHOD_VERIFY_START:
            ret = VerifyStartCodec_DecodeQuery(content, &query->content.verifyStart);
            break;

        case IQ_METHOD_VERIFY_FINISH:
            ret = VerifyFinishCodec_DecodeQuery(content, &query->content.verifyFinish);
            break;

        default:
            TINY_LOG_DEBUG("not support method: %d", query->method);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}

TinyRet IQQueryCodec_Encode(JsonObject *root, IQQuery *query)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(root, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(query, TINY_RET_E_NULL_PTR);

    do
    {
        ret = JsonObject_PutString(root, "method", IQMethod_ToString(query->method));
        if (RET_FAILED(ret))
        {
            TINY_LOG_ERROR("JsonObject_PutString");
            break;
        }

        JsonObject *content = JsonObject_New();
        if (content == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            TINY_LOG_ERROR("JsonObject_New");
            break;
        }

        if (! addQueryContent(query, content))
        {
            JsonObject_Delete(content);
            break;
        }

        ret = JsonObject_PutObject(root, "content", content);
        if (RET_FAILED(ret))
        {
            JsonObject_Delete(content);
            break;
        }
    } while (false);

    return ret;
}

TinyRet IQQueryCodec_Decode(IQQuery* query, JsonObject *object)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(query, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(object, TINY_RET_E_NULL_PTR);

    do
    {
        JsonString *method = JsonObject_GetString(object, "method");
        JsonObject *content = JsonObject_GetObject(object, "content");

        if (method == NULL)
        {
            TINY_LOG_DEBUG("method is null");
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        query->method = IQMethod_FromString(method->value);

        ret = decodeQueryContent(query, content);
    } while (false);

    return ret;
}
