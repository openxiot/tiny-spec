/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/device/verify/factory/initialize_factory.h>

Stanza * QueryInitialize_New(const char *id, const char *version, const char *authentication)
{
    Stanza *thiz = NULL;

    RETURN_VAL_IF_FAIL(version, NULL);
    RETURN_VAL_IF_FAIL(authentication, NULL);

    do
    {
        thiz = Stanza_New(STANZA_IQ);
        if (thiz == NULL)
        {
            break;
        }

        if (RET_FAILED(IQ_InitializeQuery(&thiz->data.iq, id, IQ_METHOD_INITIALIZE)))
        {
            Stanza_Delete(thiz);
            thiz = NULL;
            break;
        }

        strncpy(thiz->data.iq.content.query.content.initialize.version, version, STANZA_INITIALIZE_VERSION_LEN);
        strncpy(thiz->data.iq.content.query.content.initialize.authentication, authentication, STANZA_AUTHENTICATION_LEN);
    } while (false);

    return thiz;
}
