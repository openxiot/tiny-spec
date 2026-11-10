/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_NODE_NODE_NOTICE_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_NODE_NODE_NOTICE_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/iot_typedef/node/node.h>

TINYSPEC_BEGIN_DECLS

typedef enum
{
    NODE_NOTICE_UNDEFINED      = 0,
    NODE_NOTICE_NODE_FOUND     = 1,
    NODE_NOTICE_NODE_LOST      = 2
} NodeNoticeType;

typedef struct
{
    NodeNoticeType     type;

    union
    {
        Node                node;
    } payload;
} NodeNotice;

TINYSPEC_API
NodeNoticeType node_notice_type_from_string(const char *string);
TINYSPEC_API
const char * node_notice_type_to_string(NodeNoticeType type);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_NODE_NODE_NOTICE_H */
