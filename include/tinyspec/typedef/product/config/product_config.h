/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_PRODUCT_CONFIG_PRODUCT_CONFIG_H
#define TINYSPEC_IOT_TYPEDEF_PRODUCT_CONFIG_PRODUCT_CONFIG_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    char                        did[IOT_DEVICE_ID_LENGTH + 1];
    char                        serialNumber[IOT_SERIAL_NUMBER_LENGTH + 1];
    char                        type[IOT_DEVICE_TYPE_LENGTH + 1];
    uint16_t                    productId;
    uint16_t                    productVersion;
} ProductConfig;

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_PRODUCT_CONFIG_PRODUCT_CONFIG_H */
