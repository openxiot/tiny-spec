/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_DEVICE_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_DEVICE_H

#include <tinyspec/tinyspec_base.h>
#include <tinycore/tiny_list.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    uint16_t                    index;
    TinyList                    services;
} Device;

TINYSPEC_API
Device* Device_New(void);

TINYSPEC_API
void Device_Delete(Device *thiz);

TINYSPEC_API
TinyRet Device_Construct(Device *thiz);

TINYSPEC_API
void Device_Dispose(Device *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_DEVICE_H */
