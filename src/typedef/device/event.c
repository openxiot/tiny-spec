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
#include <tinyspec/typedef/status/iot_status.h>
#include <tinycore/json_string.h>
#include <tinyspec/typedef/device/event.h>
#include <tinyspec/typedef/device/property.h>
#include <tinyspec/typedef/device/argument.h>
#include <tinyspec/typedef/device/service.h>

static TINYCORE_RET Event_Construct(Event *thiz, uint16_t iid);

static void Event_Dispose(Event *thiz);

static void on_argument_delete(void *data, void *ctx)
{
    (void) ctx;

    Argument_Delete(data);
}

Event* Event_New(uint16_t iid)
{
    Event *thiz = NULL;

    do
    {
        thiz = (Event *)tiny_malloc(sizeof(Event));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(Event_Construct(thiz, iid)))
        {
            Event_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

Event* Event_NewInstance(uint16_t iid, const char *ns, const char *name, uint32_t uuid, const char *vendor)
{
    Event * thiz = NULL;

    do
    {
        thiz = Event_New(iid);
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("Event_New FAILED");
            break;
        }

        if (RET_FAILED(Urn_Set(&thiz->type, ns, EVENT, name, uuid, vendor)))
        {
            Event_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

static TINYCORE_RET Event_Construct(Event *thiz, uint16_t iid)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    do
    {
        memset(thiz, 0, sizeof(Event));
        thiz->iid = iid;

        ret = Urn_Construct(&thiz->type);
        if (RET_FAILED(ret))
        {
            TINY_LOG_DEBUG("Urn_Construct FAILED: %s", TinyRet_GetDescription( ret));
            break;
        }

        TinyList_Construct(&thiz->arguments);
        TinyList_SetDeleteListener(&thiz->arguments, on_argument_delete, thiz);
    } while (false);

    return ret;
}

static void Event_Dispose(Event *thiz)
{
    RETURN_IF_FAIL(thiz);

    Urn_Dispose(&thiz->type);
    TinyList_Dispose(&thiz->arguments);
}

void Event_Delete(Event *thiz)
{
    RETURN_IF_FAIL(thiz);

    TINY_LOG_DEBUG("Event_Delete (iid: %d)", thiz->iid);

    Event_Dispose(thiz);
    tiny_free(thiz);
}

TINYCORE_RET Event_ArgumentAdd(Event *thiz, uint16_t iid)
{
    TINYCORE_RET ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    do
    {
        Argument * argument = Argument_NewValue(iid);
        if (argument == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        if (TinyList_AddTail(&thiz->arguments, argument) == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            Argument_Delete(argument);
            break;
        }
    } while(false);

    return ret;
}

static Property* Event_GetProperty(Event *thiz, TinyList *arguments, uint32_t index)
{
    Argument * argument = NULL;

    RETURN_VAL_IF_FAIL(thiz, NULL);
    RETURN_VAL_IF_FAIL(arguments, NULL);

    argument = (Argument *) TinyList_GetAt(arguments, index);
    if (argument == NULL)
    {
        return NULL;
    }

    return Service_GetProperty(thiz->service, argument->iid);
}

void Event_TryProduce(Event *thiz, EventOperation *o)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    do
    {
        uint32_t count = (o->arguments != NULL) ? o->arguments->values.size : 0;

        if (thiz->arguments.size != count)
        {
            o->status = IOT_STATUS_ACTION_IN_ERROR;
            break;
        }

        for (uint32_t i = 0; i < thiz->arguments.size; ++i)
        {
            JsonValue * value = (JsonValue *)TinyList_GetAt(&o->arguments->values, i);
            Property *property = Event_GetProperty(thiz, &thiz->arguments, i);
            if (property == NULL)
            {
                TINY_LOG_ERROR("property not found");
                o->status = IOT_STATUS_INTERNAL_ERROR;
                break;
            }

            if (! Property_CheckValue(property, value))
            {
                TINY_LOG_ERROR("property value invalid");
                o->status = IOT_STATUS_EVENT_ARGUMENTS_VALUE_ERROR;
                break;
            }
        }
    } while (false);
}
