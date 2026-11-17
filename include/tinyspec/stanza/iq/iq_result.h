/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_IQ_RESULT_H
#define TINYSPEC_STANZA_IQ_IQ_RESULT_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/stanza/iq/device/control/get_properties.h>
#include <tinyspec/stanza/iq/device/control/set_properties.h>
#include <tinyspec/stanza/iq/device/control/invoke_action.h>
#include <tinyspec/stanza/iq/device/control/invoke_actions.h>
#include <tinyspec/stanza/iq/device/manager/get_children.h>
#include <tinyspec/stanza/iq/device/manager/properties_changed.h>
#include <tinyspec/stanza/iq/device/control/get_property.h>
#include <tinyspec/stanza/iq/device/control/set_property.h>
#include <tinyspec/stanza/iq/device/verify/verify_start.h>
#include <tinyspec/stanza/iq/device/key/get_access_key.h>
#include <tinyspec/stanza/iq/iq_method.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    IQMethod                    method;

    union
    {
        ResultGetProperties             getProperties;
        ResultSetProperties             setProperties;

        ResultGetProperty               getProperty;
        ResultSetProperty               setProperty;
        ResultInvokeAction              invokeAction;
        ResultInvokeActions              invokeActions;
        ResultGetChildren                getChildren;
        ResultPropertiesChanged          propertiesChanged;

        ResultGetAccessKey              getAccessKey;

        ResultVerifyStart               verifyStart;
    } content;
} IQResult;

TINYSPEC_API
TINYCORE_RET IQResult_Initialize(IQResult *thiz);

TINYSPEC_API
void IQResult_Dispose(IQResult *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_IQ_RESULT_H */
