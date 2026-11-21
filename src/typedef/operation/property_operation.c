/*
 * TinySpec - device framework for xiot
 *
 * Copyright (c) 2017-2020 jxfengzi@gmail.com
 * Copyright (c) 2024-2026 openxiot contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <tinycore/tiny_log.h>
#include <tinycore/tiny_alloc.h>
#include <tinyspec/typedef/operation/combination_value.h>
#include <tinyspec/typedef/operation/property_operation.h>

TinyRet PropertyOperation_SetWireValue(PropertyOperation *thiz, const JsonValue *wire)
{
    JsonValue *decoded = NULL;
    JsonValue *newValue = NULL;
    bool compact = false;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(wire, TINY_RET_E_NULL_PTR);

    decoded = CombinationValue_Decode(wire, &compact);
    if (decoded != NULL)
    {
        newValue = decoded;
    }
    else
    {
        newValue = JsonValue_NewFrom(wire);
        if (newValue == NULL)
        {
            return TINY_RET_E_NO_MEMORY;
        }
        compact = false;
    }

    if (thiz->value != NULL)
    {
        JsonValue_Delete(thiz->value);
    }

    thiz->value = newValue;
    thiz->argumentsCompact = compact;

    return TINY_RET_OK;
}

JsonValue * PropertyOperation_NewWireValue(const PropertyOperation *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, NULL);

    if (thiz->value == NULL)
    {
        return NULL;
    }

    return CombinationValue_Encode(thiz->value, thiz->argumentsCompact);
}

TinyRet PropertyOperation_Construct(PropertyOperation *thiz)
{
    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);

    memset(thiz, 0, sizeof(PropertyOperation));
    thiz->status = 0;

    return TINY_RET_OK;
}

void PropertyOperation_Dispose(PropertyOperation *thiz)
{
    RETURN_IF_FAIL(thiz);

    if (thiz->value != NULL)
    {
        JsonValue_Delete(thiz->value);
        thiz->value = NULL;
    }
}

PropertyOperation * PropertyOperation_New(void)
{
    PropertyOperation *thiz = NULL;

    do
    {
        thiz = (PropertyOperation *)tiny_malloc(sizeof(PropertyOperation));
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        if (RET_FAILED(PropertyOperation_Construct(thiz)))
        {
            PropertyOperation_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

PropertyOperation * PropertyOperation_NewFrom(const char *did, uint16_t siid, uint16_t piid)
{
    PropertyOperation *thiz = PropertyOperation_New();
    if (thiz != NULL)
    {
        strncpy(thiz->pid.did, did, IOT_DEVICE_ID_LENGTH);
        thiz->status = 0;
        thiz->pid.siid = siid;
        thiz->pid.iid = piid;
    }

    return thiz;
}

PropertyOperation * PropertyOperation_NewValue(const char *did, uint16_t siid, uint16_t piid, JsonValue *value)
{
    PropertyOperation *thiz = NULL;

    do
    {
        thiz = PropertyOperation_New();
        if (thiz == NULL)
        {
            break;
        }

        strncpy(thiz->pid.did, did, IOT_DEVICE_ID_LENGTH);
        thiz->status = 0;
        thiz->pid.siid = siid;
        thiz->pid.iid = piid;

        if (RET_FAILED(PropertyOperation_SetWireValue(thiz, value)))
        {
            PropertyOperation_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

PropertyOperation * PropertyOperation_CopyFrom(PropertyOperation *other)
{
    PropertyOperation * thiz = NULL;

    RETURN_VAL_IF_FAIL(other, NULL);

    do
    {
        thiz = PropertyOperation_New();
        if (thiz == NULL)
        {
            TINY_LOG_DEBUG("tiny_malloc FAILED");
            break;
        }

        TinyRet ret = PropertyOperation_Copy(thiz, other);
        if (RET_FAILED(ret))
        {
            PropertyOperation_Delete(thiz);
            thiz = NULL;
            break;
        }
    } while (false);

    return thiz;
}

TinyRet PropertyOperation_Copy(PropertyOperation *thiz, PropertyOperation *other)
{
    TinyRet ret = TINY_RET_OK;

    RETURN_VAL_IF_FAIL(thiz, TINY_RET_E_NULL_PTR);
    RETURN_VAL_IF_FAIL(other, TINY_RET_E_NULL_PTR);

    do
    {
        if (thiz == other)
        {
            break;
        }

        strncpy(thiz->pid.did, other->pid.did, IOT_DEVICE_ID_LENGTH);
        thiz->pid.siid = other->pid.siid;
        thiz->pid.iid = other->pid.iid;
        thiz->status = other->status;
        thiz->argumentsCompact = other->argumentsCompact;
        strncpy(thiz->description, other->description, IOT_DESCRIPTION_LENGTH);

        if (thiz->value != NULL)
        {
            JsonValue_Delete(thiz->value);
            thiz->value = NULL;
        }

        if (other->value == NULL)
        {
            break;
        }

        thiz->value = JsonValue_NewFrom(other->value);
        if (thiz->value == NULL)
        {
            ret = TINY_RET_E_NO_MEMORY;
            break;
        }
    } while (false);

    return ret;
}

void PropertyOperation_Delete(PropertyOperation *thiz)
{
    RETURN_IF_FAIL(thiz);

    PropertyOperation_Dispose(thiz);
    tiny_free(thiz);
}
