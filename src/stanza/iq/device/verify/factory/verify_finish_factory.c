/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/verify/factory/verify_finish_factory.h>

Stanza *QueryVerifyFinish_New(const char *id, const char *did, const char *type, const char *signature, uint8_t codec)
{
    Stanza *thiz = NULL;

    RETURN_VAL_IF_FAIL(did, NULL);
    RETURN_VAL_IF_FAIL(signature, NULL);

    do
    {
        thiz = Stanza_New(STANZA_IQ);
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(IQ_InitializeQuery(&thiz->data.iq, id, IQ_METHOD_VERIFY_FINISH)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }

        strncpy(thiz->data.iq.content.query.content.verifyFinish.did, did, IOT_DEVICE_ID_LENGTH);
        strncpy(thiz->data.iq.content.query.content.verifyFinish.type, type, IOT_DEVICE_TYPE_LENGTH);
        strncpy(thiz->data.iq.content.query.content.verifyFinish.signature, signature, STANZA_SIGNATURE_LENGTH);
        thiz->data.iq.content.query.content.verifyFinish.codec = codec;
    } while (false);

    return thiz;
}
