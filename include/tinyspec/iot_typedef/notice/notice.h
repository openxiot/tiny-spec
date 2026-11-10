/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_NOTICE_NOTICE_H
#define TINYSPEC_IOT_TYPEDEF_NOTICE_NOTICE_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/iot_typedef/notice/device/device_notice.h>
#include <tinyspec/iot_typedef/notice/node/node_notice.h>

TINYSPEC_BEGIN_DECLS

typedef union
{
    union
    {
        DeviceNotice      device;
        NodeNotice        node;
    } data;

} Notice;

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_NOTICE_NOTICE_H */
