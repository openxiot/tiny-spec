/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/stanza/iq/iq_error_factory.h>

Stanza * IQError_New(const char *id, int32_t status, const char *description)
{
    Stanza * thiz = Stanza_New(STANZA_IQ);
    if (thiz != NULL)
    {
        strncpy(thiz->data.iq.id, id, IQ_ID_LENGTH);
        thiz->data.iq.type = IQ_TYPE_ERROR;
        thiz->data.iq.content.error.status = status;
        strncpy(thiz->data.iq.content.error.description, description, STANZA_DESCRIPTION_LENGTH);
    }

    return thiz;
}
