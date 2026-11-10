/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <string.h>
#include <tinyspec/iot_typedef/notice/connection/connection_notice.h>

#define STRING_CONNECTION_NOTICE_UNDEFINED          "undefined"
#define STRING_CONNECTION_NOTICE_STATUS_CHANGED     "status-changed"

TINYCORE_RET ConnectionStatus_Construct(ConnectionStatus *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(ConnectionStatus));

    return TINY_RET_OK;
}

void ConnectionStatus_Dispose(ConnectionStatus *thiz)
{
    RETURN_IF_FAIL(thiz);

    memset(thiz, 0, sizeof(ConnectionStatus));
}

ConnectionNoticeType connection_notice_type_from_string(const char *string)
{
    RETURN_VAL_IF_FAIL(string, CONNECTION_NOTICE_UNDEFINED);

    if (STR_EQUAL(STRING_CONNECTION_NOTICE_STATUS_CHANGED, string))
    {
        return CONNECTION_NOTICE_STATUS_CHANGED;
    }

    return CONNECTION_NOTICE_UNDEFINED;
}

const char * connection_notice_type_to_string(ConnectionNoticeType type)
{
    switch (type)
    {
        case CONNECTION_NOTICE_STATUS_CHANGED:
            return STRING_CONNECTION_NOTICE_STATUS_CHANGED;

        default:
            return STRING_CONNECTION_NOTICE_UNDEFINED;
    }
}
