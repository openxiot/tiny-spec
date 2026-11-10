/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/verify/factory/verify_start_factory.h>

Stanza * QueryVerifyStart_New(const char *id, const char * publicKey)
{
    Stanza *thiz = NULL;

    RETURN_VAL_IF_FAIL(publicKey, NULL);

    do
    {
        thiz = Stanza_New(STANZA_IQ);
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(IQ_InitializeQuery(&thiz->data.iq, id, IQ_METHOD_VERIFY_START)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }

        strncpy(thiz->data.iq.content.query.content.verifyStart.publicKey, publicKey, STANZA_PUBLIC_KEY_LENGTH);
    } while (false);

    return thiz;
}
