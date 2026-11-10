/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/control/factory/get_properties_factory.h>

Stanza * ResultGetProperties_New(const char *id, PropertyOperations *operations)
{
    Stanza *thiz = NULL;

    RETURN_VAL_IF_FAIL(operations, NULL);

    do
    {
        thiz = Stanza_New(STANZA_IQ);
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(IQ_InitializeResult(&thiz->data.iq, id, IQ_METHOD_GET_PROPERTIES)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }

        if (RET_FAILED(PropertyOperations_Copy(&thiz->data.iq.content.result.content.getProperties.properties, operations)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}
