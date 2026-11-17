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
#include <tinyspec/typedef/device/action.h>
#include <tinyspec/typedef/device/property.h>
#include <tinyspec/typedef/device/argument.h>
#include <tinyspec/typedef/device/service.h>

static TinyRet Action_Construct(Action *thiz, uint16_t iid);

static void Action_Dispose(Action *thiz);

static void in_release_handler(void *data, void *ctx)
{
    (void) ctx;

    Argument_Delete(data);
}

static void out_release_handler(void *data, void *ctx)
{
    (void) ctx;

    Argument_Delete(data);
}

Action* Action_New(uint16_t iid)
{
    Action *thiz = NULL;

    do
    {
        thiz = (Action *)tiny_malloc(sizeof(Action));
        if (thiz == NULL)
        {
            TINY_LOG_ERROR("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(Action_Construct(thiz, iid)))
        {
            Action_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

Action* Action_NewInstance(uint16_t iid, const char *ns, const char *name, uint32_t uuid, const char *vendor)
{
    Action * thiz = NULL;

    do
    {
        thiz = Action_New(iid);
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("Action_New FAILED");
            break;
        }

        if (RET_FAILED(Urn_Set(&thiz->type, ns, PROPERTY, name, uuid, vendor)))
        {
            Action_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

static TinyRet Action_Construct(Action *thiz, uint16_t iid)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    do
    {
        memset(thiz, 0, sizeof(Action));
        thiz->iid = iid;

        ret = Urn_Construct(&thiz->type);
        if (RET_FAILED(ret))
        {
            TINY_LOG_DEBUG("Urn_Construct FAILED: %s", TinyRet_GetDescription( ret));
            break;
        }

        TinyList_Construct(&thiz->in);
        TinyList_SetDeleteListener(&thiz->in, in_release_handler, thiz);

        TinyList_Construct(&thiz->out);
        TinyList_SetDeleteListener(&thiz->out, out_release_handler, thiz);
    } while (false);

    return ret;
}

static void Action_Dispose(Action *thiz)
{
    RETURN_IF_FAIL(thiz);

    Urn_Dispose(&thiz->type);
    TinyList_Dispose(&thiz->in);
    TinyList_Dispose(&thiz->out);
}

void Action_Delete(Action *thiz)
{
    RETURN_IF_FAIL(thiz);

    TINY_LOG_DEBUG("Action_Delete (iid: %d)", thiz->iid);

    Action_Dispose(thiz);
    tiny_free(thiz);
}

TinyRet Action_InAdd(Action *thiz, uint16_t iid)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    do
    {
        Argument * argument = Argument_NewValue(iid);
        if (argument == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        if (TinyList_AddTail(&thiz->in, argument) == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            Argument_Delete(argument);
            break;
        }
    } while(false);

    return ret;
}

TinyRet Action_OutAdd(Action *thiz, uint16_t iid)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    do
    {
        Argument * argument = Argument_NewValue(iid);
        if (argument == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        if (TinyList_AddTail(&thiz->out, argument) == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            Argument_Delete(argument);
            break;
        }
    } while(false);

    return ret;
}

static Property* Action_GetProperty(Action *thiz, TinyList *arguments, uint32_t index)
{
    Argument * argument = NULL;

    RETURN_VAL_IF_FAIL(thiz, NULL);
    RETURN_VAL_IF_FAIL(arguments, NULL);

    argument = (Argument *) TinyList_GetAt(arguments, index);
    if (argument == NULL)
    {
        TINY_LOG_ERROR("argument not found: %u", index);
        return NULL;
    }

    return Service_GetProperty(thiz->service, argument->iid);
}

void Action_TryInvoke(Action *thiz, ActionOperation *o)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    do
    {
        if (thiz->in.size != o->in.values.size)
        {
            TINY_LOG_ERROR("action in size invalid: %d, expected: %d", (int) (o->in.values.size), (int) (thiz->in.size));
            o->status = IOT_STATUS_ACTION_IN_ERROR;
            break;
        }

        for (uint32_t i = 0; i < thiz->in.size; ++i)
        {
            JsonValue * value = (JsonValue *)TinyList_GetAt(&o->in.values, i);
            Property *property = Action_GetProperty(thiz, &thiz->in, i);
            if (property == NULL)
            {
                TINY_LOG_ERROR("property not found");
                o->status = IOT_STATUS_INTERNAL_ERROR;
                break;
            }

            if (! Property_CheckValue(property, value))
            {
                TINY_LOG_ERROR("property value invalid, value type: %d", value->type);
                o->status = IOT_STATUS_ACTION_IN_VALUE_INVALID;
                break;
            }
        }
    } while (false);
}

void Action_CheckResult(Action *thiz, ActionOperation *o)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    do
    {
        if (thiz->out.size != o->out.values.size)
        {
            o->status = IOT_STATUS_ACTION_OUT_ERROR;
            break;
        }

        if (thiz->out.size == 0)
        {
            break;
        }

        for (uint32_t i = 0; i < thiz->out.size; ++i)
        {
            JsonValue * value = (JsonValue *)TinyList_GetAt(&o->out.values, i);
            Property *property = Action_GetProperty(thiz, &thiz->out, i);
            if (property == NULL)
            {
                TINY_LOG_ERROR("property not found");
                o->status = IOT_STATUS_INTERNAL_ERROR;
                break;
            }

            if (! Property_CheckValue(property, value))
            {
                TINY_LOG_ERROR("property value invalid");
                o->status = IOT_STATUS_ACTION_OUT_VALUE_INVALID;
                break;
            }
        }
    } while (false);
}
