/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_log.h>
#include <tinycore/json_string.h>
#include <tinyspec/stanza_codec/iq/device/control/get_property_codec.h>
#include <tinyspec/stanza_codec/iq/device/control/set_property_codec.h>
#include <tinyspec/stanza_codec/iq/device/control/get_properties_codec.h>
#include <tinyspec/stanza_codec/iq/device/control/set_properties_codec.h>
#include <tinyspec/stanza_codec/iq/device/control/invoke_action_codec.h>
#include <tinyspec/stanza_codec/iq/device/key/get_access_key_codec.h>
#include <tinyspec/stanza_codec/iq/device/verify/verify_start_codec.h>
#include <tinyspec/stanza_codec/iq/iq_result_codec.h>

static bool addResultContent(IQResult *result, JsonObject *content)
{
    bool done = false;

    switch (result->method)
    {
        // basic
        case IQ_METHOD_HELLO:
        case IQ_METHOD_BYEBYE:
        case IQ_METHOD_PING:
            break;

            // device/control
        case IQ_METHOD_GET_PROPERTIES:
            done = GetPropertiesCodec_EncodeResult(&result->content.getProperties, content);
            break;

        case IQ_METHOD_SET_PROPERTIES:
            done = SetPropertiesCodec_EncodeResult(&result->content.setProperties, content);
            break;

        case IQ_METHOD_GET_PROPERTY:
            done = GetPropertyCodec_EncodeResult(&result->content.getProperty, content);
            break;

        case IQ_METHOD_SET_PROPERTY:
            done = SetPropertyCodec_EncodeResult(&result->content.setProperty, content);
            break;

        case IQ_METHOD_INVOKE_ACTION:
            done = InvokeActionCodec_EncodeResult(&result->content.invokeAction, content);
            break;

            // device/key
        case IQ_METHOD_GET_ACCESS_KEY:
            done = GetAccessKeyCodec_EncodeResult(&result->content.getAccessKey, content);
            break;

        case IQ_METHOD_SET_ACCESS_KEY:
            break;

            // device/verify
        case IQ_METHOD_INITIALIZE:
            break;

        case IQ_METHOD_VERIFY_START:
            done = VerifyStartCodec_EncodeResult(&result->content.verifyStart, content);
            break;

        case IQ_METHOD_VERIFY_FINISH:
            break;

        default:
            TINY_LOG_DEBUG("not support method: %s", IQMethod_ToString(result->method));
            done = false;
            break;
    }

    return done;
}

static TINYCORE_RET decodeResultContent(IQResult *result, JsonObject *content)
{
    TINYCORE_RET ret = TINY_RET_OK;

    switch (result->method)
    {
        // basic
        case IQ_METHOD_HELLO:
        case IQ_METHOD_BYEBYE:
        case IQ_METHOD_PING:
            break;

            // device/control
        case IQ_METHOD_GET_PROPERTIES:
            ret = GetPropertiesCodec_DecodeResult(content, &result->content.getProperties);
            break;

        case IQ_METHOD_SET_PROPERTIES:
            ret = SetPropertiesCodec_DecodeResult(content, &result->content.setProperties);
            break;

        case IQ_METHOD_GET_PROPERTY:
            ret = GetPropertyCodec_DecodeResult(content, &result->content.getProperty);
            break;

        case IQ_METHOD_SET_PROPERTY:
            ret = SetPropertyCodec_DecodeResult(content, &result->content.setProperty);
            break;

        case IQ_METHOD_INVOKE_ACTION:
            ret = InvokeActionCodec_DecodeResult(content, &result->content.invokeAction);
            break;

            // device/key
        case IQ_METHOD_GET_ACCESS_KEY:
            ret = GetAccessKeyCodec_DecodeResult(content, &result->content.getAccessKey);
            break;

        case IQ_METHOD_SET_ACCESS_KEY:
            break;

            // device/verify
        case IQ_METHOD_INITIALIZE:
            break;

        case IQ_METHOD_VERIFY_START:
            ret = VerifyStartCodec_DecodeResult(content, &result->content.verifyStart);
            break;

        case IQ_METHOD_VERIFY_FINISH:
            break;

        default:
            TINY_LOG_DEBUG("not support method: %d", result->method);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}

TINYCORE_RET IQResultCodec_Encode(JsonObject *root, IQResult *result)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(root, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(result, TINY_RET_E_NULL_PTR);

    do
    {
        ret = JsonObject_PutString(root, "method", IQMethod_ToString(result->method));
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

        if (! addResultContent(result, content))
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

TINYCORE_RET IQResultCodec_Decode(IQResult* result, JsonObject *object)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(result, TINY_RET_E_NULL_PTR);
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

        result->method = IQMethod_FromString(method->value);

        ret = decodeResultContent(result, content);
    } while (false);

    return ret;
}
