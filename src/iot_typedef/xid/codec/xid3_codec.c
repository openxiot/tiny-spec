/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_str_split.h>
#include <tinycore/tiny_snprintf.h>
#include <tinyspec/iot_typedef/xid/codec/xid3_codec.h>

TINYCORE_RET XID3_FromString(XID3 *xid, const char *string)
{
    TINYCORE_RET ret = TINY_RET_OK;
    uint32_t count = 0;
    char group[3][128];
    char *p = NULL;

    do
    {
        memset(group, 0, 3 * 128);

        count = tiny_str_split(string, ".", group, 3);
        if (count != 3)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        strncpy(xid->did, group[0], IOT_DEVICE_ID_LENGTH);
        xid->siid = (uint16_t) strtol(group[1], &p, 10);
        xid->iid = (uint16_t) strtol(group[2], &p, 10);
    } while (false);

    return ret;
}

void XID3_ToString(char string[IOT_XID_LENGTH], XID3 *xid)
{
    memset(string, 0, IOT_XID_LENGTH);
    tiny_snprintf(string, IOT_XID_LENGTH, "%s.%d.%d", xid->did, xid->siid, xid->iid);
}
