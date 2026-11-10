/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_OWNER_OWNER_NOTICE_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_OWNER_OWNER_NOTICE_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

#define OWNER_FIELD_LENGTH      64

typedef enum
{
    OWNER_NOTICE_UNDEFINED = 0,
    OWNER_NOTICE_OWNERSHIP_TAKEN = 1,
    OWNER_NOTICE_OWNERSHIP_DISCLAIMED = 2
} OwnerNoticeType;

typedef struct
{
    char    appId[OWNER_FIELD_LENGTH + 1];
    char    ownerId[OWNER_FIELD_LENGTH + 1];
    char    did[OWNER_FIELD_LENGTH + 1];
    /* Optional on the wire: only encoded, never required when decoding. */
    char    userId[OWNER_FIELD_LENGTH + 1];
    int64_t timestamp;
} OwnerOwnership;

typedef struct
{
    OwnerNoticeType     type;

    union
    {
        OwnerOwnership  ownership;
    } payload;
} OwnerNotice;

TINYSPEC_API
TINYCORE_RET OwnerOwnership_Construct(OwnerOwnership *thiz);

TINYSPEC_API
void OwnerOwnership_Dispose(OwnerOwnership *thiz);

TINYSPEC_API
OwnerNoticeType owner_notice_type_from_string(const char *string);

TINYSPEC_API
const char * owner_notice_type_to_string(OwnerNoticeType type);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_OWNER_OWNER_NOTICE_H */
