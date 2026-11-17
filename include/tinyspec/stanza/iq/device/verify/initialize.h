/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_DEVICE_VERIFY_INITIALIZE_H
#define TINYSPEC_STANZA_IQ_DEVICE_VERIFY_INITIALIZE_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/stanza/stanza_constant.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    char version[STANZA_INITIALIZE_VERSION_LEN];
    char authentication[STANZA_AUTHENTICATION_LEN];
} QueryInitialize;

TINYSPEC_API
TinyRet QueryInitialize_Construct(QueryInitialize *thiz);
TINYSPEC_API
void QueryInitialize_Dispose(QueryInitialize *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_VERIFY_INITIALIZE_H */
