/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_STANZA_IQ_NODE_CONTROL_GET_CHILDREN_H
#define TINYSPEC_STANZA_IQ_NODE_CONTROL_GET_CHILDREN_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/tiny_list.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    TinyList children;
} ResultGetChildren;

TINYSPEC_API
TINYCORE_RET ResultGetChildren_Construct(ResultGetChildren *thiz);
TINYSPEC_API
void ResultGetChildren_Dispose(ResultGetChildren *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_STANZA_IQ_NODE_CONTROL_GET_CHILDREN_H */
