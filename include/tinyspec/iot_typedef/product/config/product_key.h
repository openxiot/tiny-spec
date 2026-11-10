/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_PRODUCT_CONFIG_PRODUCT_KEY_H
#define TINYSPEC_IOT_TYPEDEF_PRODUCT_CONFIG_PRODUCT_KEY_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    char                        ltsk[IOT_DEVICE_LTSK_BASE64_LENGTH + 1];
    char                        ltpk[IOT_DEVICE_LTPK_BASE64_LENGTH + 1];
} ProductKey;

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_PRODUCT_CONFIG_PRODUCT_KEY_H */
