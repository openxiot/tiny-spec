/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_CONNECTION_CONNECTION_NOTICE_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_CONNECTION_CONNECTION_NOTICE_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef enum
{
    CONNECTION_NOTICE_UNDEFINED = 0,
    CONNECTION_NOTICE_STATUS_CHANGED = 1
} ConnectionNoticeType;

/* Payload of a "status-changed" notice on topic urn:xiot:connection.
 * The access point sends it once the session is ready; ready is the only
 * field the device has to look at.  The wire timestamp is in milliseconds
 * and does not fit an int32, so it is only kept when it does. */
typedef struct
{
    bool    ready;
    int64_t timestamp;
} ConnectionStatus;

typedef struct
{
    ConnectionNoticeType    type;

    union
    {
        ConnectionStatus    status;
    } payload;
} ConnectionNotice;

TINYSPEC_API
TINYCORE_RET ConnectionStatus_Construct(ConnectionStatus *thiz);

TINYSPEC_API
void ConnectionStatus_Dispose(ConnectionStatus *thiz);

TINYSPEC_API
ConnectionNoticeType connection_notice_type_from_string(const char *string);

TINYSPEC_API
const char * connection_notice_type_to_string(ConnectionNoticeType type);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_CONNECTION_CONNECTION_NOTICE_H */
