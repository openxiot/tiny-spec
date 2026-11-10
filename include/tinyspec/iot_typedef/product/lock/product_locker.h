/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#ifndef TINYSPEC_IOT_TYPEDEF_PRODUCT_LOCK_PRODUCT_LOCKER_H
#define TINYSPEC_IOT_TYPEDEF_PRODUCT_LOCK_PRODUCT_LOCKER_H

#include <tinyspec/tinyspec_base.h>

TINYSPEC_BEGIN_DECLS

typedef void (*PropertyLock)(void);
typedef void (*PropertyUnlock)(void);

typedef struct _ProductLocker
{
    PropertyLock              lock;
    PropertyUnlock            unlock;
} ProductLocker;

TINYSPEC_END_DECLS

#endif /* TINYSPEC_IOT_TYPEDEF_PRODUCT_LOCK_PRODUCT_LOCKER_H */
