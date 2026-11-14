/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/json_array.h>
#include <tinyspec/typedef/notice/device/impl/device_event_occurred.h>

TINYCORE_RET DeviceEventOccurred_Construct(DeviceEventOccurred *thiz)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(DeviceEventOccurred));

    return ret;
}

void DeviceEventOccurred_Dispose(DeviceEventOccurred *thiz)
{
    RETURN_IF_FAIL(thiz);

    if (thiz->argument != NULL)
    {
        JsonArray_Delete(thiz->argument);
    }

    memset(thiz, 0, sizeof(DeviceEventOccurred));
}
