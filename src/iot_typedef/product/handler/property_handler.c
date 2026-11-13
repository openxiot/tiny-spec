/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/iot_typedef/product/handler/property_handler.h>
#include <tinyspec/iot_typedef/status/iot_status.h>

/*
 * 单例：属性物理读写 listener。getter 负责把物理值填进 operation->value；
 * setter 负责把 operation->value 落到物理层。
 */
static PropertyListener listener_getter = NULL;
static PropertyListener listener_setter = NULL;

void PropertyHandler_SetListener(PropertyListener getter, PropertyListener setter)
{
    listener_getter = getter;
    listener_setter = setter;
}

void PropertyHandler_NotifyGet(PropertyOperation *operation, void *ctx)
{
    RETURN_IF_FAIL(operation);

    if (listener_getter == NULL)
    {
        operation->status = IOT_STATUS_INTERNAL_ERROR;
        return;
    }

    listener_getter(operation, ctx);
}

void PropertyHandler_NotifySet(PropertyOperation *operation, void *ctx)
{
    RETURN_IF_FAIL(operation);

    if (listener_setter == NULL)
    {
        operation->status = IOT_STATUS_INTERNAL_ERROR;
        return;
    }

    listener_setter(operation, ctx);
}