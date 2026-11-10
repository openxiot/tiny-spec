/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <tinycore/json_number.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/iot_typedef/notice/owner/owner_notice_codec.h>

static TINYCORE_RET copy_string_field(char *dst, size_t dst_size,
                                      JsonObject *payload, const char *key,
                                      bool required)
{
    JsonString *value = JsonObject_GetString(payload, key);

    if (value == NULL)
    {
        if (required)
        {
            TINY_LOG_ERROR("%s not found", key);
            return TINY_RET_E_PARAM_INVALID;
        }
        return TINY_RET_OK;
    }

    if (strlen(value->value) > dst_size - 1)
    {
        TINY_LOG_ERROR("%s too long", key);
        return TINY_RET_E_PARAM_INVALID;
    }

    strncpy(dst, value->value, dst_size - 1);
    dst[dst_size - 1] = '\0';

    return TINY_RET_OK;
}

static TINYCORE_RET OwnerOwnershipCodec_Encode(JsonObject *payload,
                                               OwnerOwnership *ownership)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(ownership, TINY_RET_E_NULL_PTR);

    do
    {
        ret = JsonObject_PutString(payload, "appId", ownership->appId);
        if (RET_FAILED(ret))
        {
            break;
        }

        ret = JsonObject_PutString(payload, "ownerId", ownership->ownerId);
        if (RET_FAILED(ret))
        {
            break;
        }

        if (ownership->userId[0] != '\0')
        {
            ret = JsonObject_PutString(payload, "userId", ownership->userId);
            if (RET_FAILED(ret))
            {
                break;
            }
        }

        ret = JsonObject_PutString(payload, "did", ownership->did);
        if (RET_FAILED(ret))
        {
            break;
        }

        if (ownership->timestamp != 0)
        {
            ret = JsonObject_PutInteger(payload, "timestamp",
                                        (int32_t) ownership->timestamp);
        }
    } while (false);

    return ret;
}

static TINYCORE_RET OwnerOwnershipCodec_Decode(OwnerOwnership *ownership,
                                               JsonObject *payload)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(ownership, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);

    do
    {
        ret = copy_string_field(ownership->appId, sizeof(ownership->appId),
                                payload, "appId", true);
        if (RET_FAILED(ret))
        {
            break;
        }

        ret = copy_string_field(ownership->ownerId, sizeof(ownership->ownerId),
                                payload, "ownerId", true);
        if (RET_FAILED(ret))
        {
            break;
        }

        ret = copy_string_field(ownership->did, sizeof(ownership->did),
                                payload, "did", true);
        if (RET_FAILED(ret))
        {
            break;
        }

        (void) copy_string_field(ownership->userId, sizeof(ownership->userId),
                                 payload, "userId", false);

        JsonNumber *timestamp = JsonObject_GetNumber(payload, "timestamp");
        if (timestamp != NULL && JsonNumber_IsInteger(timestamp))
        {
            ownership->timestamp = (int64_t) JsonNumber_GetInteger(timestamp);
        }
    } while (false);

    return ret;
}

TINYCORE_RET OwnerNoticeCodec_Encode(JsonObject *payload, int type,
                                     OwnerNotice *notice)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(notice, TINY_RET_E_NULL_PTR);

    switch (type)
    {
        case OWNER_NOTICE_OWNERSHIP_TAKEN:
        case OWNER_NOTICE_OWNERSHIP_DISCLAIMED:
            ret = OwnerOwnershipCodec_Encode(payload, &notice->payload.ownership);
            break;

        default:
            TINY_LOG_ERROR("type not supported: %d", type);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}

TINYCORE_RET OwnerNoticeCodec_Decode(OwnerNotice *notice, int type,
                                     JsonObject *payload)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(notice, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(payload, TINY_RET_E_NULL_PTR);

    switch (type)
    {
        case OWNER_NOTICE_OWNERSHIP_TAKEN:
        case OWNER_NOTICE_OWNERSHIP_DISCLAIMED:
            ret = OwnerOwnershipCodec_Decode(&notice->payload.ownership, payload);
            break;

        default:
            TINY_LOG_ERROR("type not supported: %d", type);
            ret = TINY_RET_E_PARAM_INVALID;
            break;
    }

    return ret;
}
