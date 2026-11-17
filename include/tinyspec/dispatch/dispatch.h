/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_TYPEDEF_DISPATCH_H
#define TINYSPEC_TYPEDEF_DISPATCH_H

#include <tinyspec/stanza/iq/iq.h>
#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/product/product.h>

TINYSPEC_BEGIN_DECLS

void dispatch_iq(Product *thiz, IQ *iq);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_TYPEDEF_DISPATCH_H */
