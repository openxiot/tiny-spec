/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_IQ_QUERY_H
#define TINYSPEC_STANZA_IQ_IQ_QUERY_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/tiny_list.h>
#include <tinyspec/stanza/iq/device/verify/initialize.h>
#include <tinyspec/stanza/iq/device/verify/verify_finish.h>
#include <tinyspec/stanza/iq/device/verify/verify_start.h>
#include <tinyspec/stanza/iq/device/key/set_access_key.h>
#include <tinyspec/stanza/iq/device/key/get_access_key.h>
#include <tinyspec/stanza/iq/device/control/get_properties.h>
#include <tinyspec/stanza/iq/device/control/set_properties.h>
#include <tinyspec/stanza/iq/device/control/get_property.h>
#include <tinyspec/stanza/iq/device/control/set_property.h>
#include <tinyspec/stanza/iq/device/control/invoke_action.h>
#include <tinyspec/stanza/iq/device/control/invoke_actions.h>
#include <tinyspec/stanza/iq/device/manager/get_children.h>
#include <tinyspec/stanza/iq/device/manager/properties_changed.h>

#include <tinyspec/stanza/iq/iq_method.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    IQMethod method;

    union
    {
        QueryGetProperties              getProperties;
        QuerySetProperties              setProperties;
        QueryGetProperty                getProperty;
        QuerySetProperty                setProperty;
        QueryInvokeAction               invokeAction;
        QueryInvokeActions              invokeActions;
        QueryGetChildren                getChildren;
        QueryPropertiesChanged          propertiesChanged;

        QuerySetAccessKey               setAccessKey;

        QueryInitialize                 initialize;
        QueryVerifyFinish               verifyFinish;
        QueryVerifyStart                verifyStart;
    } content;
} IQQuery;

TINYSPEC_API
TINYCORE_RET IQQuery_Initialize(IQQuery *thiz);

TINYSPEC_API
void IQQuery_Dispose(IQQuery *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_IQ_QUERY_H */
