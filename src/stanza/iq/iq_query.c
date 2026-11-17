/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_log.h>
#include <tinyspec/stanza/iq/iq_query.h>

TINYCORE_RET IQQuery_Initialize(IQQuery *thiz)
{
    TINYCORE_RET ret = TINY_RET_OK;

    switch (thiz->method)
    {
        // basic
        case IQ_METHOD_HELLO:
        case IQ_METHOD_BYE:
        case IQ_METHOD_PING:
            break;

        // device/control
        case IQ_METHOD_GET_PROPERTIES:
            ret = QueryGetProperties_Construct(&thiz->content.getProperties);
            break;

        case IQ_METHOD_SET_PROPERTIES:
            ret = QuerySetProperties_Construct(&thiz->content.setProperties);
            break;

        case IQ_METHOD_GET_PROPERTY:
            ret = QueryGetProperty_Construct(&thiz->content.getProperty);
            break;

        case IQ_METHOD_SET_PROPERTY:
            ret = QuerySetProperty_Construct(&thiz->content.setProperty);
            break;

        case IQ_METHOD_INVOKE_ACTION:
            ret = QueryInvokeAction_Construct(&thiz->content.invokeAction);
            break;

        // device/key
        case IQ_METHOD_GET_ACCESS_KEY:
            break;

        case IQ_METHOD_SET_ACCESS_KEY:
            ret = QuerySetAccessKey_Construct(&thiz->content.setAccessKey);
            break;

        // device/verify
        case IQ_METHOD_INITIALIZE:
            ret = QueryInitialize_Construct(&thiz->content.initialize);
            break;

        case IQ_METHOD_VERIFY_START:
            ret = QueryVerifyStart_Construct(&thiz->content.verifyStart);
            break;

        case IQ_METHOD_VERIFY_FINISH:
            ret = QueryVerifyFinish_Construct(&thiz->content.verifyFinish);
            break;

        default:
            TINY_LOG_ERROR("method not supported: %d", thiz->method);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}

void IQQuery_Dispose(IQQuery *thiz)
{
    switch (thiz->method)
    {
        case IQ_METHOD_HELLO:
        case IQ_METHOD_BYE:
        case IQ_METHOD_PING:
            break;

        // device/control
        case IQ_METHOD_GET_PROPERTIES:
            QueryGetProperties_Dispose(&thiz->content.getProperties);
            break;

        case IQ_METHOD_SET_PROPERTIES:
            QuerySetProperties_Dispose(&thiz->content.setProperties);
            break;

        case IQ_METHOD_GET_PROPERTY:
            QueryGetProperty_Dispose(&thiz->content.getProperty);
            break;

        case IQ_METHOD_SET_PROPERTY:
            QuerySetProperty_Dispose(&thiz->content.setProperty);
            break;

        case IQ_METHOD_INVOKE_ACTION:
            QueryInvokeAction_Dispose(&thiz->content.invokeAction);
            break;

        // device/key
        case IQ_METHOD_GET_ACCESS_KEY:
            break;

        case IQ_METHOD_SET_ACCESS_KEY:
            QuerySetAccessKey_Dispose(&thiz->content.setAccessKey);
            break;

        // device/verify
        case IQ_METHOD_INITIALIZE:
            QueryInitialize_Dispose(&thiz->content.initialize);
            break;

        case IQ_METHOD_VERIFY_FINISH:
            QueryVerifyFinish_Dispose(&thiz->content.verifyFinish);
            break;

        case IQ_METHOD_VERIFY_START:
            QueryVerifyStart_Dispose(&thiz->content.verifyStart);
            break;

        case IQ_METHOD_INVOKE_ACTIONS:
            QueryInvokeActions_Dispose(&thiz->content.invokeActions);
            break;

        case IQ_METHOD_GET_CHILDREN:
            QueryGetChildren_Dispose(&thiz->content.getChildren);
            break;

        case IQ_METHOD_PROPERTIES_CHANGED:
            QueryPropertiesChanged_Dispose(&thiz->content.propertiesChanged);
            break;

        default:
            TINY_LOG_ERROR("method not supported: %d", thiz->method);
            break;
    }
}
