/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_DEVICE_VERIFY_VERIFY_FINISH_H
#define TINYSPEC_STANZA_IQ_DEVICE_VERIFY_VERIFY_FINISH_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/iot_typedef/constant/typedef_constant.h>
#include <tinyspec/stanza/stanza_constant.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    char                did[IOT_DEVICE_ID_LENGTH];
    char                type[IOT_DEVICE_TYPE_LENGTH];
    char                signature[STANZA_SIGNATURE_LENGTH];
    uint8_t             codec;
} QueryVerifyFinish;

TINYSPEC_API
TINYCORE_RET QueryVerifyFinish_Construct(QueryVerifyFinish *thiz);
TINYSPEC_API
void QueryVerifyFinish_Dispose(QueryVerifyFinish *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_VERIFY_VERIFY_FINISH_H */
