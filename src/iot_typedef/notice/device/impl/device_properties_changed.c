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
#include <tinyspec/iot_typedef/notice/device/impl/device_properties_changed.h>

static void OnPropertyChangedDelete(void *data, void *ctx)
{
    (void) ctx;

    DevicePropertyChanged_Delete((DevicePropertyChanged *) data);
}

DevicePropertiesChanged * DevicePropertiesChanged_New(void)
{
    DevicePropertiesChanged *thiz = NULL;

    do
    {
        thiz = (DevicePropertiesChanged *) tiny_malloc(sizeof(DevicePropertiesChanged));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(DevicePropertiesChanged_Construct(thiz)))
        {
            DevicePropertiesChanged_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

void DevicePropertiesChanged_Delete(DevicePropertiesChanged *thiz)
{
    RETURN_IF_FAIL(thiz);

    DevicePropertiesChanged_Dispose(thiz);
    tiny_free(thiz);
}

TINYCORE_RET DevicePropertiesChanged_Construct(DevicePropertiesChanged *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    TinyList_Construct(&thiz->properties);
    TinyList_SetDeleteListener(&thiz->properties, OnPropertyChangedDelete, NULL);
    thiz->timestamp = 0;

    return TINY_RET_OK;
}

void DevicePropertiesChanged_Dispose(DevicePropertiesChanged *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyList_Dispose(&thiz->properties);
    thiz->timestamp = 0;
}

TINYCORE_RET DevicePropertiesChanged_Add(DevicePropertiesChanged *thiz,
                                         DevicePropertyChanged *item)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(item, TINY_RET_E_NULL_PTR);

    if (TinyList_AddTail(&thiz->properties, item) == NULL)
    {
        return TINY_RET_E_OUT_OF_MEMORY;
    }

    return TINY_RET_OK;
}
