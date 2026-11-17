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
#include <tinyspec/typedef/device/service.h>
#include <tinyspec/typedef/device/property.h>
#include <tinyspec/typedef/device/action.h>
#include <tinyspec/typedef/device/event.h>

static TinyRet Service_Construct(Service *thiz, uint16_t iid);

static void Service_Dispose(Service *thiz);

static void property_release_handler(void *data, void *ctx)
{
    (void) ctx;

    Property_Delete((Property *) data);
}

static void action_release_handler(void *data, void *ctx)
{
    (void) ctx;

    Action_Delete((Action *) data);
}

static void event_release_handler(void *data, void *ctx)
{
    (void) ctx;

    Event_Delete((Event *) data);
}

Service* Service_New(uint16_t iid)
{
    Service *thiz = NULL;

    do
    {
        thiz = (Service *) tiny_malloc(sizeof(Service));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(Service_Construct(thiz, iid)))
        {
            Service_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

Service* Service_NewInstance(uint16_t iid, const char *ns, const char *name, uint32_t uuid, const char *vendor)
{
    Service * thiz = NULL;

    do
    {
        thiz = Service_New(iid);
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("Service_New FAILED");
            break;
        }

        if (RET_FAILED(Urn_Set(&thiz->type, ns, SERVICE, name, uuid, vendor)))
        {
            Service_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

static TinyRet Service_Construct(Service *thiz, uint16_t iid)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    do
    {
        memset(thiz, 0, sizeof(Service));
        thiz->iid = iid;

        ret = Urn_Construct(&thiz->type);
        if (RET_FAILED(ret))
        {
            TINY_LOG_DEBUG("Urn_Construct FAILED: %s", TinyRet_GetDescription(ret));
            break;
        }

        /**
         * Properties
         */
        TinyList_Construct(&thiz->properties);
        TinyList_SetDeleteListener(&thiz->properties, property_release_handler, thiz);

        /**
         * Actions
         */
        TinyList_Construct(&thiz->actions);
        TinyList_SetDeleteListener(&thiz->actions, action_release_handler, thiz);

        /**
         * Events
         */
        TinyList_Construct(&thiz->events);
        TinyList_SetDeleteListener(&thiz->events, event_release_handler, thiz);
    } while (false);

    return ret;
}

static void Service_Dispose(Service *thiz)
{
    RETURN_IF_FAIL(thiz);

    Urn_Dispose(&thiz->type);
    TinyList_Dispose(&thiz->properties);
    TinyList_Dispose(&thiz->actions);
    TinyList_Dispose(&thiz->events);
}

void Service_Delete(Service *thiz)
{
    RETURN_IF_FAIL(thiz);

    TINY_LOG_DEBUG("Service_Delete (iid: %d)", thiz->iid);

    Service_Dispose(thiz);
    tiny_free(thiz);
}

Property *Service_GetProperty(Service *thiz, uint16_t iid)
{
    RETURN_VAL_IF_FAIL(thiz, NULL);

    for (uint32_t i = 0; i < thiz->properties.size; ++i)
    {
        Property *p = (Property *) TinyList_GetAt(&thiz->properties, i);
        if (p->iid == iid)
        {
            return p;
        }
    }

    return NULL;
}

Action *Service_GetAction(Service *thiz, uint16_t iid)
{
    RETURN_VAL_IF_FAIL(thiz, NULL);

    for (uint32_t i = 0; i < thiz->actions.size; ++i)
    {
        Action *a = (Action *) TinyList_GetAt(&thiz->actions, i);
        if (a->iid == iid)
        {
            return a;
        }
    }

    return NULL;
}

Event *Service_GetEvent(Service *thiz, uint16_t iid)
{
    RETURN_VAL_IF_FAIL(thiz, NULL);

    for (uint32_t i = 0; i < thiz->events.size; ++i)
    {
        Event *e = (Event *) TinyList_GetAt(&thiz->events, i);
        if (e->iid == iid)
        {
            return e;
        }
    }

    return NULL;
}

bool Service_CheckValue(Service *thiz, PropertyOperation * o)
{
    Property *property = NULL;

    RETURN_VAL_IF_FAIL(thiz, false);
    RETURN_VAL_IF_FAIL(o, false);

    property = Service_GetProperty(thiz, o->pid.iid);
    if (property == NULL)
    {
        return false;
    }

    return Property_CheckValue(property, o->value);
}

bool Service_CheckResult(Service *thiz, ActionOperation * o)
{
    Action *action = NULL;

    RETURN_VAL_IF_FAIL(thiz, false);
    RETURN_VAL_IF_FAIL(o, false);

    action = Service_GetAction(thiz, o->aid.iid);
    if (action != NULL)
    {
        Action_CheckResult(action, o);
    }
    else
    {
        o->status = IOT_STATUS_ACTION_NOT_FOUND;
    }

    return true;
}

void Service_TryRead(Service *thiz, PropertyOperation *o)
{
    Property *property = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    property = Service_GetProperty(thiz, o->pid.iid);
    if (property != NULL)
    {
        Property_TryRead(property, o);
    }
    else
    {
        o->status = IOT_STATUS_PROPERTY_NOT_FOUND;
    }
}

void Service_TryWrite(Service *thiz, PropertyOperation *o)
{
    Property *property = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    property = Service_GetProperty(thiz, o->pid.iid);
    if (property != NULL)
    {
        Property_TryWrite(property, o);
    }
    else
    {
        o->status = IOT_STATUS_PROPERTY_NOT_FOUND;
    }
}

void Service_TryInvoke(Service *thiz, ActionOperation *o)
{
    Action *action = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    action = Service_GetAction(thiz, o->aid.iid);
    if (action != NULL)
    {
        action->service = thiz;
        Action_TryInvoke(action, o);
    }
    else
    {
        o->status = IOT_STATUS_ACTION_NOT_FOUND;
    }
}

void Service_TryChange(Service *thiz, PropertyOperation *o, bool save)
{
    Property *property = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    property = Service_GetProperty(thiz, o->pid.iid);
    if (property != NULL)
    {
        Property_TryChange(property, o, save);
    }
    else
    {
        o->status = IOT_STATUS_PROPERTY_NOT_FOUND;
    }
}

void Service_TryProduce(Service *thiz, EventOperation *o)
{
    Event *event = NULL;

    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    event = Service_GetEvent(thiz, o->eid.iid);
    if (event != NULL)
    {
        event->service = thiz;
        Event_TryProduce(event, o);
    }
    else
    {
        o->status = IOT_STATUS_EVENT_NOT_FOUND;
    }
}
