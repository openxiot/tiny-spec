/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/dispatch/dispatch.h>
#include <tinyspec/dispatch/send.h>

#include "tinyspec/stanza/iq/basic/factory/hello_factory.h"
#include "tinyspec/stanza/iq/basic/factory/ping_factory.h"
#include "tinyspec/stanza/iq/device/control/factory/get_properties_factory.h"
#include "tinyspec/stanza/iq/device/control/factory/get_property_factory.h"
#include "tinyspec/stanza/iq/device/control/factory/invoke_action_factory.h"
#include "tinyspec/stanza/iq/device/control/factory/invoke_actions_factory.h"
#include "tinyspec/stanza/iq/device/control/factory/set_properties_factory.h"
#include "tinyspec/stanza/iq/device/control/factory/set_property_factory.h"
#include "tinyspec/stanza/iq/device/manager/factory/get_children_factory.h"
#include "tinyspec/typedef/product/operator/product_operator.h"

void dispatch_iq(Product *thiz, IQ *iq)
{
    Stanza *result = NULL;

    if (iq->type != IQ_TYPE_QUERY)
    {
        return; /* RESULT / ERROR 直接忽略 */
    }

    switch (iq->content.query.method)
    {
    case IQ_METHOD_HELLO:
        result = ResultHello_New(iq->id);
        break;

    case IQ_METHOD_PING:
        result = Ping_New(iq->id);
        break;

    case IQ_METHOD_GET_PROPERTIES:
    {
        PropertyOperations *ops = &iq->content.query.content.getProperties.properties;
        Product_TryReadProperties(thiz, ops);
        result = ResultGetProperties_New(iq->id, ops);
        break;
    }

    case IQ_METHOD_SET_PROPERTIES:
    {
        PropertyOperations *ops = &iq->content.query.content.setProperties.properties;
        Product_TryWriteProperties(thiz, ops);
        result = ResultSetProperties_New(iq->id, ops);
        break;
    }

    case IQ_METHOD_GET_PROPERTY:
    {
        PropertyOperation *op = &iq->content.query.content.getProperty.property;
        PropertyOperations ops;
        PropertyOperations_Construct(&ops);
        TinyList_AddTail(&ops.properties, op);
        Product_TryReadProperties(thiz, &ops);
        result = ResultGetProperty_New(iq->id, op);
        break;
    }

    case IQ_METHOD_SET_PROPERTY:
    {
        PropertyOperation *op = &iq->content.query.content.setProperty.property;
        PropertyOperations ops;
        PropertyOperations_Construct(&ops);
        TinyList_AddTail(&ops.properties, op);
        Product_TryWriteProperties(thiz, &ops);
        result = ResultSetProperty_New(iq->id, op);
        break;
    }

    case IQ_METHOD_INVOKE_ACTION:
    {
        ActionOperation *a = &iq->content.query.content.invokeAction.action;
        Product_TryInvokeAction(thiz, a);
        result = ResultInvokeAction_New(iq->id, a);
        break;
    }

    case IQ_METHOD_GET_CHILDREN:
    {
        ResultGetChildren getChildren;
        ResultGetChildren_Construct(&getChildren);
        Product_TryGetChildren(thiz, &getChildren);
        result = ResultGetChildren_New(iq->id, &getChildren);
        ResultGetChildren_Dispose(&getChildren);
        break;
    }

    case IQ_METHOD_INVOKE_ACTIONS:
    {
        ActionOperations *ops = &iq->content.query.content.invokeActions.actions;
        Product_TryInvokeActions(thiz, ops);
        result = ResultInvokeActions_New(iq->id, ops);
        break;
    }

    default:
        if (iq->content.query.method == IQ_METHOD_UNDEFINED)
        {
            TINY_LOG_WARN("接收到未识别的方法(可能是 get-children 等自定义URN)");
        }
        else
        {
            TINY_LOG_WARN("未处理的方法: %s", IQMethod_ToString(iq->content.query.method));
        }
        break;
    }

    if (result != NULL)
    {
        send_stanza(thiz, result);
        Stanza_Delete(result);
    }
}
