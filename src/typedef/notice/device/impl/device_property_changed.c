/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinyspec/typedef/notice/device/impl/device_property_changed.h>

TinyRet DevicePropertyChanged_Construct(DevicePropertyChanged *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(DevicePropertyChanged));

    return TINY_RET_OK;
}

void DevicePropertyChanged_Dispose(DevicePropertyChanged *thiz)
{
    RETURN_IF_FAIL(thiz);

    if (thiz->value != NULL)
    {
        JsonValue_Delete(thiz->value);
    }

    memset(thiz, 0, sizeof(DevicePropertyChanged));
}
