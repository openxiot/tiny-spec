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
#include <tinyspec/typedef/device/device.h>
#include <tinyspec/typedef/device/service.h>

static void service_release_handler(void *data, void *ctx)
{
    (void) ctx;

    Service_Delete((Service *)data);
}

Device* Device_New(void)
{
    Device *thiz = NULL;

    do
    {
        thiz = (Device *)tiny_malloc(sizeof(Device));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(Device_Construct(thiz)))
        {
            Device_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

TINYCORE_RET Device_Construct(Device *thiz)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    do
    {
        memset(thiz, 0, sizeof(Device));

        TinyList_Construct(&thiz->services);
        TinyList_SetDeleteListener(&thiz->services, service_release_handler, thiz);
    } while (false);

    return ret;
}

void Device_Dispose(Device *thiz)
{
    RETURN_IF_FAIL(thiz);

    TinyList_Dispose(&thiz->services);
}

void Device_Delete(Device *thiz)
{
    RETURN_IF_FAIL(thiz);

    Device_Dispose(thiz);
    tiny_free(thiz);
}
