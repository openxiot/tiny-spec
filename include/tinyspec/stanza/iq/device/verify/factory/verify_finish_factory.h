/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_DEVICE_VERIFY_FACTORY_VERIFY_FINISH_FACTORY_H
#define TINYSPEC_STANZA_IQ_DEVICE_VERIFY_FACTORY_VERIFY_FINISH_FACTORY_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/constant/typedef_constant.h>
#include <tinyspec/stanza/stanza_constant.h>
#include <tinyspec/stanza/stanza.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
Stanza *QueryVerifyFinish_New(const char *id, const char *did, const char *type, const char *signature, uint8_t codec);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_DEVICE_VERIFY_FACTORY_VERIFY_FINISH_FACTORY_H */
