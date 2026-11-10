/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_IQ_ERROR_FACTORY_H
#define TINYSPEC_STANZA_IQ_IQ_ERROR_FACTORY_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/stanza/stanza.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
Stanza * IQError_New(const char *id, int32_t status, const char *description);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_IQ_ERROR_FACTORY_H */
