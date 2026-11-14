/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_DEVICE_ARGUMENT_H
#define TINYSPEC_IOT_TYPEDEF_DEVICE_ARGUMENT_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    uint16_t            iid;
} Argument;

TINYSPEC_API
Argument* Argument_New(void);

TINYSPEC_API
Argument* Argument_NewValue(uint16_t iid);

TINYSPEC_API
void Argument_Delete(Argument *thiz);

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_DEVICE_ARGUMENT_H */
