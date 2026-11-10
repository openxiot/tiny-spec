/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NODE_NODE_H
#define TINYSPEC_IOT_TYPEDEF_NODE_NODE_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

#define NODE_SERIAL_NUMBER_LENGTH   64

typedef struct
{
    char    serialNumber[NODE_SERIAL_NUMBER_LENGTH];
    int     productId;
    int     productVersion;
} Node;

TINYSPEC_API
Node * Node_New(void);

TINYSPEC_API
void Node_Delete(Node *thiz);

TINYSPEC_API
TINYCORE_RET Node_Construct(Node *thiz);

TINYSPEC_API
void Node_Dispose(Node *thiz);

TINYSPEC_API
TINYCORE_RET Node_Copy(Node *dst, Node *src);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NODE_NODE_H */
