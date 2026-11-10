/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_PRODUCT_CONFIG_IPV4_CONFIG_H
#define TINYSPEC_IOT_TYPEDEF_PRODUCT_CONFIG_IPV4_CONFIG_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef struct
{
    char                        ip[TINY_IP_LEN];
    uint16_t                    port;
} IPv4Config;

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_PRODUCT_CONFIG_IPV4_CONFIG_H */
