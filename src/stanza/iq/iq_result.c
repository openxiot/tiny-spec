/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_log.h>
#include <tinyspec/stanza/iq/node/control/get_nodes.h>
#include <tinyspec/stanza/iq/iq_result.h>

TINYCORE_RET IQResult_Initialize(IQResult *thiz)
{
    TINYCORE_RET ret = TINY_RET_OK;

    switch (thiz->method)
    {
        // basic
        case IQ_METHOD_HELLO:
        case IQ_METHOD_BYEBYE:
        case IQ_METHOD_PING:
            break;

        // device/control
        case IQ_METHOD_GET_PROPERTIES:
            ret = ResultGetProperties_Construct(&thiz->content.getProperties);
            break;

        case IQ_METHOD_SET_PROPERTIES:
            ret = ResultSetProperties_Construct(&thiz->content.setProperties);
            break;

        case IQ_METHOD_GET_PROPERTY:
            ret = ResultGetProperty_Construct(&thiz->content.getProperty);
            break;

        case IQ_METHOD_SET_PROPERTY:
            ret = ResultSetProperty_Construct(&thiz->content.setProperty);
            break;

        case IQ_METHOD_INVOKE_ACTION:
            ret = ResultInvokeAction_Construct(&thiz->content.invokeAction);
            break;

        // device/key
        case IQ_METHOD_GET_ACCESS_KEY:
            ret = ResultGetAccessKey_Construct(&thiz->content.getAccessKey);
            break;

        case IQ_METHOD_SET_ACCESS_KEY:
            break;

        // device/verify
        case IQ_METHOD_INITIALIZE:
            break;

        case IQ_METHOD_VERIFY_START:
            ret = ResultVerifyStart_Construct(&thiz->content.verifyStart);
            break;

        case IQ_METHOD_VERIFY_FINISH:
            break;

        // node/control
        case IQ_METHOD_GET_NODES:
            ret = ResultGetNodes_Construct(&thiz->content.getNodes);
            break;

        default:
            TINY_LOG_ERROR("method not supported: %d", thiz->method);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}

void IQResult_Dispose(IQResult *thiz)
{
    switch (thiz->method)
    {
        // basic
        case IQ_METHOD_HELLO:
        case IQ_METHOD_BYEBYE:
        case IQ_METHOD_PING:
            break;

        // device/control
        case IQ_METHOD_GET_PROPERTIES:
            ResultGetProperties_Dispose(&thiz->content.getProperties);
            break;

        case IQ_METHOD_SET_PROPERTIES:
            ResultSetProperties_Dispose(&thiz->content.setProperties);
            break;

        case IQ_METHOD_GET_PROPERTY:
            ResultGetProperty_Dispose(&thiz->content.getProperty);
            break;

        case IQ_METHOD_SET_PROPERTY:
            ResultSetProperty_Dispose(&thiz->content.setProperty);
            break;

        case IQ_METHOD_INVOKE_ACTION:
            ResultInvokeAction_Dispose(&thiz->content.invokeAction);
            break;

        // device/key
        case IQ_METHOD_GET_ACCESS_KEY:
            ResultGetAccessKey_Dispose(&thiz->content.getAccessKey);
            break;

        case IQ_METHOD_SET_ACCESS_KEY:
            break;

        // device/verify
        case IQ_METHOD_INITIALIZE:
            break;

        case IQ_METHOD_VERIFY_START:
            ResultVerifyStart_Dispose(&thiz->content.verifyStart);
            break;

        case IQ_METHOD_VERIFY_FINISH:
            break;

        // node/control
        case IQ_METHOD_GET_NODES:
            ResultGetNodes_Dispose(&thiz->content.getNodes);
            break;

        default:
            TINY_LOG_ERROR("method not supported: %d", thiz->method);
            break;
    }
}
