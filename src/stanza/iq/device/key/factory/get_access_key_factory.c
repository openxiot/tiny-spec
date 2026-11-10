/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/key/factory/get_access_key_factory.h>

Stanza * QueryGetAccessKey_New(const char *id)
{
    Stanza *thiz = NULL;

    do
    {
        thiz = Stanza_New(STANZA_IQ);
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(IQ_InitializeQuery(&thiz->data.iq, id, IQ_METHOD_GET_ACCESS_KEY)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}
