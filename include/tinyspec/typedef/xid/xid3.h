/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_XID_XID3_H
#define TINYSPEC_IOT_TYPEDEF_XID_XID3_H

#include <tinyspec/tinyspec_base.h>
#include <tinyspec/typedef/constant/typedef_constant.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    char                did[IOT_DEVICE_ID_LENGTH];
    uint16_t            siid;
    uint16_t            iid;
    uint16_t            aid;                            /** homekit accessory id */
} XID3;

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_XID_XID3_H */
