/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_IQ_H
#define TINYSPEC_STANZA_IQ_IQ_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/tiny_list.h>
#include <tinyspec/stanza/iq/iq_query.h>
#include <tinyspec/stanza/iq/iq_result.h>
#include <tinyspec/stanza/iq/iq_error.h>

TINYSPEC_BEGIN_DECLS

typedef enum
{
    IQ_TYPE_UNDEFINED           = 0x00,
    IQ_TYPE_QUERY               = 0x01,
    IQ_TYPE_RESULT              = 0x02,
    IQ_TYPE_ERROR               = 0x03,
} IQType;

#define IQ_ID_LENGTH       64

typedef struct
{
    char                id[IQ_ID_LENGTH + 1];
    IQType              type;
    union
    {
        IQQuery         query;
        IQResult        result;
        IQError         error;
    } content;
} IQ;

TINYSPEC_API
TINYCORE_RET IQ_InitializeQuery(IQ *thiz, const char *id, IQMethod method);

TINYSPEC_API
TINYCORE_RET IQ_InitializeResult(IQ *thiz, const char *id, IQMethod method);

TINYSPEC_API
void IQ_Dispose(IQ *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_IQ_H */
