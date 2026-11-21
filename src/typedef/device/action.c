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

        if (RET_FAILED(Urn_Set(&thiz->type, ns, ACTION, name, uuid, vendor)))
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

static TinyRet Action_AddArgument(Action *thiz, TinyList *arguments, uint16_t iid, uint16_t min, uint16_t max)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(arguments, TINY_RET_E_NULL_PTR);

    do
    {
        Argument * argument = NULL;

        if (min > max)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        argument = Argument_NewValue(iid);
        if (argument == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }

        argument->minRepeat = min;
        argument->maxRepeat = max;

        if (TinyList_AddTail(arguments, argument) == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            Argument_Delete(argument);
            break;
        }
    } while(false);

    return ret;
}

TinyRet Action_InAdd(Action *thiz, uint16_t iid)
{
    return Action_InAddEx(thiz, iid, 1, 1);
}

TinyRet Action_InAddEx(Action *thiz, uint16_t iid, uint16_t min, uint16_t max)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    return Action_AddArgument(thiz, &thiz->in, iid, min, max);
}

TinyRet Action_OutAdd(Action *thiz, uint16_t iid)
{
    return Action_OutAddEx(thiz, iid, 1, 1);
}

TinyRet Action_OutAddEx(Action *thiz, uint16_t iid, uint16_t min, uint16_t max)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    return Action_AddArgument(thiz, &thiz->out, iid, min, max);
}

static TinyRet Action_CheckArguments(Action *thiz,
                                          TinyList *definitions,
                                          ActionOperation *o,
                                          bool in,
                                          int missingStatus,
                                          int invalidStatus)
{
    uint32_t size = 0;
    uint32_t i = 0;
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(definitions, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(o, TINY_RET_E_NULL_PTR);

    size = TinyList_GetSize(definitions);
    for (i = 0; i < size; ++i)
    {
        Argument *argument = (Argument *) TinyList_GetAt(definitions, i);
        ArgumentOperation *operation = NULL;
        Property *property = NULL;
        uint32_t count = 0;
        uint32_t j = 0;

        if (argument == NULL)
        {
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        operation = in ? ActionOperation_GetIn(o, argument->iid) : ActionOperation_GetOut(o, argument->iid);
        property = Service_GetProperty(thiz->service, argument->iid);

        if (property == NULL)
        {
            TINY_LOG_ERROR("property not found: %u", (unsigned int) argument->iid);
            o->status = missingStatus;
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        if (operation == NULL)
        {
            if (argument->minRepeat > 0)
            {
                TINY_LOG_ERROR("argument missing: %u", (unsigned int) argument->iid);
                o->status = missingStatus;
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }

            continue;
        }

        count = JsonArray_GetSize(&operation->values);
        if (count < (uint32_t) argument->minRepeat || count > (uint32_t) argument->maxRepeat)
        {
            TINY_LOG_ERROR("argument count invalid: %u, count: %u, expected: [%u, %u]",
                           (unsigned int) argument->iid,
                           (unsigned int) count,
                           (unsigned int) argument->minRepeat,
                           (unsigned int) argument->maxRepeat);
            o->status = missingStatus;
            ret = TINY_RET_E_PARAM_INVALID;
            break;
        }

        for (j = 0; j < count; ++j)
        {
            JsonValue *value = JsonArray_GetValue(&operation->values, j);

            if (! Property_CheckValue(property, value))
            {
                TINY_LOG_ERROR("argument value invalid: %u", (unsigned int) argument->iid);
                o->status = invalidStatus;
                ret = TINY_RET_E_PARAM_INVALID;
                break;
            }
        }

        if (RET_FAILED(ret))
        {
            break;
        }
    }

    return ret;
}

void Action_TryInvoke(Action *thiz, ActionOperation *o)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    if (RET_FAILED(Action_CheckArguments(thiz,
                                         &thiz->in,
                                         o,
                                         true,
                                         IOT_STATUS_ACTION_IN_ERROR,
                                         IOT_STATUS_ACTION_IN_VALUE_INVALID)))
    {
        TINY_LOG_ERROR("action in invalid");
    }
}

void Action_CheckResult(Action *thiz, ActionOperation *o)
{
    RETURN_IF_FAIL(thiz);
    RETURN_IF_FAIL(o);

    if (RET_FAILED(Action_CheckArguments(thiz,
                                         &thiz->out,
                                         o,
                                         false,
                                         IOT_STATUS_ACTION_OUT_ERROR,
                                         IOT_STATUS_ACTION_OUT_VALUE_INVALID)))
    {
        TINY_LOG_ERROR("action out invalid");
    }
}
