/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/key/factory/set_access_key_factory.h>

Stanza * QuerySetAccessKey_New(const char *id, const char *key)
{
    Stanza *thiz = NULL;

    RETURN_VAL_IF_FAIL(key, NULL);

    do
    {
        thiz = Stanza_New(STANZA_IQ);
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(IQ_InitializeQuery(&thiz->data.iq, id, IQ_METHOD_SET_ACCESS_KEY)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }

        strncpy(thiz->data.iq.content.query.content.setAccessKey.key, key, STANZA_ACCESS_KEY_LEN);
    } while (false);

    return thiz;
}
