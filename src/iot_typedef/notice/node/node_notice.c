/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/iot_typedef/notice/node/node_notice.h>

#define STRING_NODE_NOTICE_UNDEFINED           "undefined"
#define STRING_NODE_NOTICE_NODE_FOUND          "node-found"
#define STRING_NODE_NOTICE_NODE_LOST           "node-lost"

NodeNoticeType node_notice_type_from_string(const char *string)
{
    RETURN_VAL_IF_FAIL(string, NODE_NOTICE_UNDEFINED);

    if (STR_EQUAL(STRING_NODE_NOTICE_NODE_FOUND, string))
    {
        return NODE_NOTICE_NODE_FOUND;
    }

    if (STR_EQUAL(STRING_NODE_NOTICE_NODE_LOST, string))
    {
        return NODE_NOTICE_NODE_LOST;
    }

    return NODE_NOTICE_UNDEFINED;
}

const char * node_notice_type_to_string(NodeNoticeType type)
{
    switch (type)
    {
        case NODE_NOTICE_NODE_FOUND:
            return STRING_NODE_NOTICE_NODE_FOUND;

        case NODE_NOTICE_NODE_LOST:
            return STRING_NODE_NOTICE_NODE_LOST;

        default:
            return STRING_NODE_NOTICE_UNDEFINED;
    }
}
