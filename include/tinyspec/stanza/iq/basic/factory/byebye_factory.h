/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_BASIC_FACTORY_BYEBYE_FACTORY_H
#define TINYSPEC_STANZA_IQ_BASIC_FACTORY_BYEBYE_FACTORY_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/stanza/stanza.h>

TINYSPEC_BEGIN_DECLS

TINYSPEC_API
Stanza * QueryByebye_New(const char *id);

TINYSPEC_API
Stanza * ResultByebye_New(const char *id);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_BASIC_FACTORY_BYEBYE_FACTORY_H */
