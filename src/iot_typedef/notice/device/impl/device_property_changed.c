/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_alloc.h>
#include <tinycore/tiny_log.h>
#include <tinyspec/iot_typedef/notice/device/impl/device_property_changed.h>

DevicePropertyChanged * DevicePropertyChanged_New(void)
{
    DevicePropertyChanged *thiz = NULL;

    do
    {
        thiz = (DevicePropertyChanged *) tiny_malloc(sizeof(DevicePropertyChanged));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(DevicePropertyChanged_Construct(thiz)))
        {
            DevicePropertyChanged_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void DevicePropertyChanged_Delete(DevicePropertyChanged *thiz)
{
    RETURN_IF_FAIL(thiz);

    DevicePropertyChanged_Dispose(thiz);
    tiny_free(thiz);
}

TINYCORE_RET DevicePropertyChanged_Construct(DevicePropertyChanged *thiz)
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
