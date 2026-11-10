/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/control/factory/invoke_action_factory.h>

Stanza * ResultInvokeAction_New(const char *id, ActionOperation *operation)
{
    Stanza *thiz = NULL;

    RETURN_VAL_IF_FAIL(operation, NULL);

    do
    {
        thiz = Stanza_New(STANZA_IQ);
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(IQ_InitializeResult(&thiz->data.iq, id, IQ_METHOD_INVOKE_ACTION)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }

        if (RET_FAILED(ActionOperation_Copy(&thiz->data.iq.content.result.content.invokeAction.action, operation)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}
