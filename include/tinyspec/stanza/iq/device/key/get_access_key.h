/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_DEVICE_KEY_GET_ACCESS_KEY_H
#define TINYSPEC_STANZA_IQ_DEVICE_KEY_GET_ACCESS_KEY_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/stanza/stanza_constant.h>

TINYSPEC_BEGIN_DECLS

/**
 * Query
 */

/**
 * Result
 */

typedef struct
{
    char  key[STANZA_ACCESS_KEY_LEN];
} ResultGetAccessKey;

TINYSPEC_API
TINYCORE_RET ResultGetAccessKey_Construct(ResultGetAccessKey *thiz);
TINYSPEC_API
void ResultGetAccessKey_Dispose(ResultGetAccessKey *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_KEY_GET_ACCESS_KEY_H */
