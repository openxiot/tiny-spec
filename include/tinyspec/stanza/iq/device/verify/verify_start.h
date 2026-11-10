/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_DEVICE_VERIFY_VERIFY_START_H
#define TINYSPEC_STANZA_IQ_DEVICE_VERIFY_VERIFY_START_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/stanza/stanza_constant.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    char        publicKey[STANZA_PUBLIC_KEY_LENGTH];
} QueryVerifyStart;

TINYSPEC_API
TINYCORE_RET QueryVerifyStart_Construct(QueryVerifyStart *thiz);
TINYSPEC_API
void QueryVerifyStart_Dispose(QueryVerifyStart *thiz);

typedef struct
{
    char        publicKey[STANZA_PUBLIC_KEY_LENGTH];
    char        signature[STANZA_SIGNATURE_LENGTH];
} ResultVerifyStart;

TINYSPEC_API
TINYCORE_RET ResultVerifyStart_Construct(ResultVerifyStart *thiz);
TINYSPEC_API
void ResultVerifyStart_Dispose(ResultVerifyStart *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_VERIFY_VERIFY_START_H */
