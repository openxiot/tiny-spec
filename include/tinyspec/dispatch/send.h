/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_DISPATCH_SEND_H
#define TINYSPEC_DISPATCH_SEND_H

#include <tinyspec/stanza/stanza.h>
#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/product/product.h>

TINYSPEC_BEGIN_DECLS

TinyRet send_stanza(Product *thiz, Stanza *stanza);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_DISPATCH_SEND_H */
