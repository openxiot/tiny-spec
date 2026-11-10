/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <tinyspec/iot_typedef/notice/owner/owner_notice.h>

#define STRING_OWNER_NOTICE_UNDEFINED               "undefined"
#define STRING_OWNER_NOTICE_OWNERSHIP_TAKEN         "ownership-taken"
#define STRING_OWNER_NOTICE_OWNERSHIP_DISCLAIMED    "ownership-disclaimed"

TINYCORE_RET OwnerOwnership_Construct(OwnerOwnership *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(OwnerOwnership));

    return TINY_RET_OK;
}

void OwnerOwnership_Dispose(OwnerOwnership *thiz)
{
    RETURN_IF_FAIL(thiz);

    memset(thiz, 0, sizeof(OwnerOwnership));
}

OwnerNoticeType owner_notice_type_from_string(const char *string)
{
    RETURN_VAL_IF_FAIL(string, OWNER_NOTICE_UNDEFINED);

    if (STR_EQUAL(STRING_OWNER_NOTICE_OWNERSHIP_TAKEN, string))
    {
        return OWNER_NOTICE_OWNERSHIP_TAKEN;
    }

    if (STR_EQUAL(STRING_OWNER_NOTICE_OWNERSHIP_DISCLAIMED, string))
    {
        return OWNER_NOTICE_OWNERSHIP_DISCLAIMED;
    }

    return OWNER_NOTICE_UNDEFINED;
}

const char * owner_notice_type_to_string(OwnerNoticeType type)
{
    switch (type)
    {
        case OWNER_NOTICE_OWNERSHIP_TAKEN:
            return STRING_OWNER_NOTICE_OWNERSHIP_TAKEN;

        case OWNER_NOTICE_OWNERSHIP_DISCLAIMED:
            return STRING_OWNER_NOTICE_OWNERSHIP_DISCLAIMED;

        default:
            return STRING_OWNER_NOTICE_UNDEFINED;
    }
}
