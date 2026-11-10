/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/control/factory/set_property_factory.h>

Stanza * ResultSetProperty_New(const char *id, PropertyOperation *property)
{
    Stanza *thiz = NULL;

    RETURN_VAL_IF_FAIL(property, NULL);

    do
    {
        thiz = Stanza_New(STANZA_IQ);
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(IQ_InitializeResult(&thiz->data.iq, id, IQ_METHOD_SET_PROPERTY)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }

        if (RET_FAILED(PropertyOperation_Copy(&thiz->data.iq.content.result.content.setProperty.property, property)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}
