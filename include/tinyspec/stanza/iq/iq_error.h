/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_IQ_ERROR_H
#define TINYSPEC_STANZA_IQ_IQ_ERROR_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/stanza/stanza_constant.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    int status;
    char description[STANZA_DESCRIPTION_LENGTH];
} IQError;

TINYSPEC_API
void IQError_Dispose(IQError *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_IQ_ERROR_H */
