/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_STANZA_H
#define TINYSPEC_STANZA_STANZA_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/stanza/iq/iq.h>
#include <tinyspec/stanza/message/message.h>

TINYSPEC_BEGIN_DECLS

typedef enum
{
    STANZA_IQ           = 1,
    STANZA_MESSAGE      = 2,
} StanzaType;

typedef struct
{
    StanzaType                  type;

    union
    {
        IQ                      iq;
        Message                 message;
    } data;
} Stanza;

TINYSPEC_API
Stanza * Stanza_New(StanzaType type);

TINYSPEC_API
void Stanza_Delete(Stanza *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_STANZA_H */
