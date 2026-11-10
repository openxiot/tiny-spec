/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/iot_typedef/notice/node/node_notice_codec.h>
#include <tinyspec/iot_typedef/notice/node/impl/node_changed_codec.h>

TINYCORE_RET NodeNoticeCodec_Encode(JsonObject *payload, int type, NodeNotice *notice)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(notice, TINY_RET_E_NULL_PTR);

    switch (type)
    {
        case NODE_NOTICE_NODE_FOUND:
            ret = NodeInfoCodec_Encode(payload, & notice->payload.node);
            break;

        case NODE_NOTICE_NODE_LOST:
            ret = NodeInfoCodec_Encode(payload, &notice->payload.node);
            break;

        default:
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}

TINYCORE_RET NodeNoticeCodec_Decode(NodeNotice *notice, int type, JsonObject *payload)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(notice, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);

    switch (type)
    {
        case NODE_NOTICE_NODE_FOUND:
            ret = NodeInfoCodec_Decode(&notice->payload.node, payload);
            break;

        case NODE_NOTICE_NODE_LOST:
            ret = NodeInfoCodec_Decode(&notice->payload.node, payload);
            break;

        default:
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}
